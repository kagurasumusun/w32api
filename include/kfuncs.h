/*
 * The implementation is from the WCE library
 * The description that this is supposed to be "kfuncs.h" is from pages
 * such as :
 *
 * http://msdn.microsoft.com/library/default.asp?url=/library/en-us/wcekernl/html/_wcesdk_win32_exitthread.asp
 * http://msdn.microsoft.com/library/default.asp?url=/library/en-us/wcesdkr/html/_wcesdk_win32_pulseevent.asp
 */
#ifndef	_W32API_KFUNCS_H_
#define	_W32API_KFUNCS_H_

#ifndef _WIN32_WCE
#error "_WIN32_WCE is not defined."
#endif

/*
 * http://hubpages.com/hub/hackingwindowsce :
 *	"The value of PUserKData is fixed as 0xFFFFC800 on the ARM processor,
 *	and 0x00005800 on other CPUs."
 * http://www.ddj.com/architect/184405459
 * http://andy-embed.blogspot.com/
 * http://arsouyes.org/phrack/phrack63/phrack63_0x06.html
 */
#if defined(__arm__)
#define	PUserKData		((LPBYTE)0xFFFFC800)
#else
#define	PUserKData		((LPBYTE)0x00005800)
#endif

#define	SYSHANDLE_OFFSET	0x004
#define	SYS_HANDLE_BASE		64
#define SH_WIN32                0
#define SH_CURTHREAD            1
#define SH_CURPROC              2
/*
 * ---------------------------------------------------------------------------
 * Windows CE 6.0 kernel API-set indices and kernel-data accessor.
 *
 * Source: the CE 6.0 R1 SDK, PUBLIC/COMMON/SDK/INC/kfuncs.h (authoritative).
 *
 * EVERY definition in this block is gated on _WIN32_WCE >= 0x0600 on purpose.
 * The API-set numbering is NOT stable across CE releases:
 *
 *   CE 6.0 (R1)  SH_GDI=80  SH_WMGR=81  SH_WNET=82  SH_COMM=83
 *                SH_FILESYS_APIS=84  SH_SHELL=85  SH_DEVMGR_APIS=86
 *                SH_TAPI=87  SH_SERVICES=90  SH_DDRAW=91  SH_D3DM=92
 *   CE 5.0       SH_GDI=16  SH_WMGR=17  SH_WNET=18  SH_COMM=19
 *                SH_FILESYS_APIS=20  SH_SHELL=21  SH_DEVMGR_APIS=22
 *                SH_TAPI=23  SH_SERVICES=26
 *
 * The CE 5.0 numbering above comes from a community source only -- no CE 5.0
 * SDK header was available to verify it -- so it is deliberately NOT emitted
 * here.  A wrong API-set index silently dispatches into an unrelated PSL
 * instead of failing to link, which is far worse than a missing constant.
 * CE 4.x values are unknown.  See notes/audit/W32API-KFUNCS-GAP.md.
 *
 * Likewise, CE 6.0 R3 / Windows Embedded Compact 2013 extend this table
 * (HT_NAMESPACE, HT_POLICY, HT_SECLOADER, SH_CPROG, SH_GWEUSER,
 * SH_CONNMGR_LEGACY, SH_DMSRV, SH_INPUT, SH_COMPOSITOR, SH_NETCF).  Those are
 * R3-only and are NOT defined here, because the vendored wince-source tree is
 * CE 6.0 R1; adding them would contaminate the R1 surface.
 * ---------------------------------------------------------------------------
 */
#if (_WIN32_WCE >= 0x0600)

/* 128 API sets: 0 = kernel Win32; 1-63 OS handle-based; 64-79 partner
 * handle-based; 80-111 OS non-handle-based; 112-127 partner non-handle-based.
 * Non-handle-based sets (80-127) receive PSL notifications. */
#define SH_FIRST_OS_HAPI_SET    1       /* 1st OS, handle based API set */
#define SH_FIRST_EXT_HAPI_SET   64      /* 1st partner handle based API set */
#define SH_FIRST_OS_API_SET     80      /* 1st OS non-handle based API set */
#define SH_FIRST_EXT_API_SET    112     /* 1st partner non-handle based set */

#define SH_CURTOKEN             3       /* pseudo-handle: current token */

/* Special handle indices used for "typed" handle calls.  These moved into
 * kfuncs.h in CE 6.0; on CE 4.x/5.x they lived in the OEM psyscall.h. */
#define HT_EVENT                4       /* Event handle type */
#define HT_MUTEX                5       /* Mutex handle type */
#define HT_APISET               6       /* kernel API set handle type */
#define HT_FILE                 7       /* open file handle type */
#define HT_FIND                 8       /* FindFirst handle type */
#define HT_DBFILE               9       /* open database handle type */
#define HT_DBFIND               10      /* database find handle type */
#define HT_SOCKET               11      /* WinSock open socket handle type */
#define HT_CRITSEC              12      /* Critical section */
#define HT_SEMAPHORE            13      /* Semaphore handle type */
#define HT_FSMAP                14      /* mapped files */
#define HT_WNETENUM             15      /* Net Resource Enumeration */
#define HT_AFSVOLUME            16      /* file system volume handle type */

/* Last set notified on thread/process termination. */
#define SH_LAST_NOTIFY          SH_FIRST_OS_API_SET

#define SH_GDI                  (SH_LAST_NOTIFY+0)
#define SH_WMGR                 (SH_LAST_NOTIFY+1)
#define SH_WNET                 (SH_LAST_NOTIFY+2)  /* network redirector */
#define SH_COMM                 (SH_LAST_NOTIFY+3)  /* communications, not COM */
#define SH_FILESYS_APIS         (SH_LAST_NOTIFY+4)  /* file system APIs */
#define SH_SHELL                (SH_LAST_NOTIFY+5)
#define SH_DEVMGR_APIS          (SH_LAST_NOTIFY+6)  /* file system device mgr */
#define SH_TAPI                 (SH_LAST_NOTIFY+7)
#define SH_SERVICES             (SH_LAST_NOTIFY+10)
#define SH_DDRAW                (SH_LAST_NOTIFY+11)
#define SH_D3DM                 (SH_LAST_NOTIFY+12)
#define SH_LASTRESERVED         (SH_FIRST_EXT_API_SET-1)

/*
 * __GetUserKData reads a DWORD out of the user-mode kernel data page.  It is a
 * genuine COREDLL export on CE 6.0 (core_common.def @2528, and both
 * coredll6.def / coredll6-x86.def) but is NOT exported by CE 4.x or CE 5.x
 * COREDLL -- hence the version gate.  Without it this declaration would create
 * an unresolvable import on CE 4.x/5.x.
 */
DWORD __GetUserKData (DWORD dwOfst);

#endif /* _WIN32_WCE >= 0x0600 */

/* Process/Thread ID Methods */
static inline HANDLE GetCurrentProcess()
{
  return ((HANDLE)(SH_CURPROC+SYS_HANDLE_BASE));

}

static inline HANDLE GetCurrentThread()
{
  return ((HANDLE)(SH_CURTHREAD+SYS_HANDLE_BASE));
}

static inline DWORD GetCurrentThreadId()
{
  return ((DWORD)(((HANDLE *)(PUserKData+SYSHANDLE_OFFSET))[SH_CURTHREAD]));
}

static inline DWORD GetCurrentProcessId()
{
  return ((DWORD)(((HANDLE *)(PUserKData+SYSHANDLE_OFFSET))[SH_CURPROC]));
}
#if (_WIN32_WCE >= 0x0600)
/* CE 6.0 added a token-based security model.  Like GetCurrentProcess() and
 * GetCurrentThread() above this returns a pseudo-handle derived from the
 * API-set index; it is not a COREDLL import. */
static inline HANDLE GetCurrentToken()
{
  return ((HANDLE)(SH_CURTOKEN+SYS_HANDLE_BASE));
}
#endif /* _WIN32_WCE >= 0x0600 */

/* EventModify signature hinted on:
   http://msdn.microsoft.com/library/default.asp?url=/library/en-us/wcehardware5/html/wce50lrfCeLogImportTable.asp

   Event Constants and EventModify signature in the c# example at:
   http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnnetcomp/html/PISAPICF.asp  */
WINBASEAPI BOOL WINAPI EventModify(HANDLE h, DWORD e);

#define	EVENT_PULSE	1
#define	EVENT_RESET	2
#define	EVENT_SET	3

static inline BOOL PulseEvent (HANDLE x)
{
  return EventModify(x, EVENT_PULSE);
}

static inline BOOL ResetEvent (HANDLE x)
{
  return EventModify(x, EVENT_RESET);
}

static inline BOOL SetEvent (HANDLE x)
{
  return EventModify(x, EVENT_SET);
}

/* TLS Constants and Constructs */
#define TLS_FUNCALLOC   0
#define TLS_FUNCFREE    1

WINBASEAPI DWORD WINAPI TlsCall(DWORD func, DWORD val);

static inline DWORD TlsAlloc (void)
{
  return (TlsCall(TLS_FUNCALLOC, 0));
}

static inline BOOL WINAPI TlsFree(DWORD x)
{
  return (TlsCall(TLS_FUNCFREE, x));
}

/*
 * Take the special cases out of winbase.h
 */
#if defined (__arm__)
# define DebugBreak() __asm__( ".word 0xe6000010" )
#elif defined (__i386__) || defined (__x86_64__)
# define DebugBreak() __asm__( ".byte 0xcc" )
#else
  /* externally supplied for an unsupported architecture */
  extern void DebugBreak(void);
#endif

/* Windows CE COREDLL exports (signatures from the Windows CE SDK
 * kfuncs.h). */
#ifdef _WIN32_WCE
DWORD CeGetCallerTrust(VOID);
DWORD CeGetCurrentTrust(VOID);
DWORD CeGetProcessTrust(HANDLE hProc);
#endif /* _WIN32_WCE */
#if (_WIN32_WCE >= 0x0600)
/* The CE 6.0 R1 SDK kfuncs.h defines this as an inline (added "for porting
 * Alchemy"); it is not a COREDLL export.  CE 6 does not support multiple
 * processors, so the result is a constant. */
static inline DWORD WINAPI CeGetTotalProcessors (void)
{
  return 1;
}
#endif /* _WIN32_WCE >= 0x0600 */
#endif
