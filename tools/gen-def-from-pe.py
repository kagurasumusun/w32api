#!/usr/bin/env python3
"""
gen-def-from-pe.py -- derive a GNU dlltool .def file from a Windows PE binary.

CLEAN-ROOM METHODOLOGY NOTE
---------------------------
This tool intentionally consumes ONLY the PE export table of an already-built
binary (coredll.dll, ole32.dll, ...).  The export name table, the ordinal
table and the export address table are *functional interface facts* of the
binary, not expressive source text.  Nothing is taken from any vendor's
source .def, .lib, header or SDK file.

By deriving the import surface from the shipped binary rather than from a
copyrighted source listing, the resulting .def is a black-box extraction of
behavior (what symbols the DLL exports, and at which ordinals) -- the same
facts one would obtain by running `dumpbin /exports` or `objdump -p` -- and
not a transcription of a source file.

Usage:
    python3 gen-def-from-pe.py PATH/TO/module.dll [-o out.def] \
        [--name NAME] [--min-ordinal N] [--strip-underscore]

The tool is arch/object-format agnostic (PE32 and PE32+).  It prints the
def to stdout unless -o is given.  A Windows target (e.g. coredll.dll) must
be supplied by the user; it is obtained from the vendor's redistributable
OS image / device and is not redistributable here.
"""
import sys
import struct
import argparse


class Export:
    __slots__ = ("name", "ordinal", "rva")

    def __init__(self, name, ordinal, rva):
        self.name = name
        self.ordinal = ordinal
        self.rva = rva


def read_pe_exports(data):
    """Parse a PE image and return (list_of_Export, dll_name_str)."""
    if data[:2] != b"MZ":
        raise ValueError("not a PE file (missing MZ header)")
    e_lfanew = struct.unpack_from("<I", data, 0x3C)[0]
    if data[e_lfanew:e_lfanew + 4] != b"PE\x00\x00":
        raise ValueError("not a PE file (missing PE signature)")
    coff = e_lfanew + 4
    machine, nsec, _t, _s, _c, opt_size, _c2 = struct.unpack_from(
        "<HHIIIHH", data, coff)
    opt = coff + 20
    magic = struct.unpack_from("<H", data, opt)[0]
    if magic == 0x10B:      # PE32
        dd = opt + 96
        num_dir = 16
    elif magic == 0x20B:    # PE32+
        dd = opt + 112
        num_dir = 16
    else:
        raise ValueError("unknown optional header magic %#x" % magic)

    # Export directory is index 0 of the data directories.
    export_rva, export_size = struct.unpack_from("<II", data, dd)
    if export_rva == 0:
        return [], None

    def rva_to_off(rva):
        # Find which section contains rva and compute the file offset.
        sec_tab = opt + opt_size
        for i in range(nsec):
            _n, vsz, va, rawsz, rawp = struct.unpack_from(
                "<IIIIIIHHI", data, sec_tab + i * 40)[0:5]
            # fields: name, virt_size, virt_addr, raw_size, raw_ptr
            _name = data[sec_tab + i * 40: sec_tab + i * 40 + 8]
            vsz, va, rawsz, rawp = struct.unpack_from(
                "<IIII", data, sec_tab + i * 40 + 8)
            if va <= rva < va + max(vsz, rawsz):
                return rawp + (rva - va)
        raise ValueError("RVA %#x not mapped in any section (import/export stub?)"
                         % rva)

    eoff = rva_to_off(export_rva)

    # IMAGE_EXPORT_DIRECTORY layout (offsets are relative to the export dir):
    #   +0 Characteristics, +4 TimeDateStamp, +8 Major/MinorVersion,
    #   +12 Name (RVA), +16 Base, +20 NumberOfFunctions,
    #   +24 NumberOfNames, +28 AddressOfFunctions, +32 AddressOfNames,
    #   +36 AddressOfNameOrdinals.
    n_func = struct.unpack_from("<I", data, eoff + 20)[0]
    n_name = struct.unpack_from("<I", data, eoff + 24)[0]
    _base = struct.unpack_from("<I", data, eoff + 16)[0]
    func_tab_rva = struct.unpack_from("<I", data, eoff + 28)[0]
    name_tab_rva = struct.unpack_from("<I", data, eoff + 32)[0]
    ord_tab_rva = struct.unpack_from("<I", data, eoff + 36)[0]

    dll_name = None
    name_rva = struct.unpack_from("<I", data, eoff + 12)[0]  # Name field
    if name_rva:
        noff = rva_to_off(name_rva)
        end = data.index(b"\x00", noff)
        dll_name = data[noff:end].decode("ascii", "replace")

    funcs_by_ord = {}
    for i in range(n_func):
        frva = struct.unpack_from("<I", data, rva_to_off(func_tab_rva) + i * 4)[0]
        funcs_by_ord[i + 1] = (frva, None)

    for i in range(n_name):
        nrva = struct.unpack_from("<I", data, rva_to_off(name_tab_rva) + i * 4)[0]
        noff = rva_to_off(nrva)
        end = data.index(b"\x00", noff)
        nm = data[noff:end].decode("ascii", "replace")
        ord_idx = struct.unpack_from("<H", data,
                                     rva_to_off(ord_tab_rva) + i * 2)[0]
        # AddressOfNameOrdinals stores 1-based ordinals, matching the
        # 1-based keys of funcs_by_ord (which were indexed ordinal = i+1).
        if ord_idx in funcs_by_ord:
            frva = funcs_by_ord[ord_idx][0]
            funcs_by_ord[ord_idx] = (frva, nm)

    exports = []
    for ordinal in sorted(funcs_by_ord):
        frva, nm = funcs_by_ord[ordinal]
        exports.append(Export(nm, ordinal, frva))
    return exports, dll_name


def def_name(name, dll_name, explicit):
    if explicit:
        return explicit
    if dll_name:
        base = dll_name.rsplit(".", 1)[0]
        return base.lower() if base else "MODULE"
    # fall back to the file stem
    return name.rsplit("/", 1)[-1].rsplit("\\", 1)[-1].rsplit(".", 1)[0]


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("pe", help="path to the target PE binary (DLL)")
    ap.add_argument("-o", "--out", help="write the .def to this file")
    ap.add_argument("--name", help="LIBRARY name (default: derived from the DLL)")
    ap.add_argument("--subsystem", default=None)
    ap.add_argument("--min-ordinal", type=int, default=1,
                    help="lowest ordinal to emit (default 1)")
    ap.add_argument("--strip-underscore", action="store_true",
                    help="drop a leading underscore from symbol names")
    args = ap.parse_args()

    with open(args.pe, "rb") as fh:
        data = fh.read()
    exports, dll_name = read_pe_exports(data)
    if not exports:
        sys.stderr.write("warning: %s has no export directory\n" % args.pe)
        exports = []

    libname = def_name(args.pe, dll_name, args.name)
    lines = []
    lines.append("; .def derived from the PE export table of %s" % args.pe)
    lines.append("; (black-box functional-interface extraction, not a source copy)")
    if dll_name and dll_name.lower() not in (libname + ".dll", libname):
        lines.append("; internal DLL name: %s" % dll_name)
    lines.append("LIBRARY %s" % libname)
    lines.append("EXPORTS")
    for e in exports:
        if e.ordinal < args.min_ordinal:
            continue
        nm = e.name
        if nm is None:
            lines.append("    #%d NONAME" % e.ordinal)
            continue
        if args.strip_underscore and nm.startswith("_"):
            nm = nm[1:]
        lines.append("    %s @%d" % (nm, e.ordinal))
    text = "\n".join(lines) + "\n"

    if args.out:
        with open(args.out, "w", encoding="utf-8", newline="\n") as fh:
            fh.write(text)
        sys.stderr.write("wrote %d exports -> %s\n" % (len(exports), args.out))
    else:
        sys.stdout.write(text)


if __name__ == "__main__":
    main()
