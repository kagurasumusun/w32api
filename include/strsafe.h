/**
 * strsafe.h - Safe String handling for Win32 and Windows CE.
 *
 * On Windows CE every routine below is a real COREDLL export (the CE
 * shared-source coredll.def lists them all), so unlike the desktop
 * strsafe.h they are prototypes, not inline wrappers.
 *
 * Placed in the public domain.
 */
#ifndef _STRSAFE_H_INCLUDED
#define _STRSAFE_H_INCLUDED

#if __GNUC__ >= 3
#pragma GCC system_header
#endif

#include <windows.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STRSAFE_MAX_CCH 2147483647
#define STRSAFE_MAX_LENGTH (STRSAFE_MAX_CCH - 1)

#define STRSAFE_FILL_BYTE(X) ((DWORD)((BYTE)(X)))
#define STRSAFE_FILL_BEHIND_NULL 0x200
#define STRSAFE_FILL_ON_FAILURE 0x020
#define STRSAFE_NULL_ON_FAILURE 0x100
#define STRSAFE_NO_TRUNCATION 0x001
#define STRSAFE_IGNORE_NULLS 0x100

/* StringCb routines (sizes in bytes) */

HRESULT StringCbCatA(LPSTR pszDest,size_t cbDest,LPCSTR pszSrc);
HRESULT StringCbCatW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszSrc);
HRESULT StringCbCatExA(LPSTR pszDest,size_t cbDest,LPCSTR pszSrc,LPSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags);
HRESULT StringCbCatExW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszSrc,LPWSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags);
HRESULT StringCbCatNA(LPSTR pszDest,size_t cbDest,LPCSTR pszSrc,size_t cbToAppend);
HRESULT StringCbCatNW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszSrc,size_t cbToAppend);
HRESULT StringCbCatNExA(LPSTR pszDest,size_t cbDest,LPCSTR pszSrc,size_t cbToAppend,LPSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags);
HRESULT StringCbCatNExW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszSrc,size_t cbToAppend,LPWSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags);
HRESULT StringCbCopyA(LPSTR pszDest,size_t cbDest,LPCSTR pszSrc);
HRESULT StringCbCopyW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszSrc);
HRESULT StringCbCopyExA(LPSTR pszDest,size_t cbDest,LPCSTR pszSrc,LPSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags);
HRESULT StringCbCopyExW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszSrc,LPWSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags);
HRESULT StringCbCopyNA(LPSTR pszDest,size_t cbDest,LPCSTR pszSrc,size_t cbToCopy);
HRESULT StringCbCopyNW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszSrc,size_t cbToCopy);
HRESULT StringCbCopyNExA(LPSTR pszDest,size_t cbDest,LPCSTR pszSrc,size_t cbToCopy,LPSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags);
HRESULT StringCbCopyNExW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszSrc,size_t cbToCopy,LPWSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags);
HRESULT StringCbLengthA(LPCSTR psz,size_t cbMax,size_t *pcbStr);
HRESULT StringCbLengthW(LPCWSTR psz,size_t cbMax,size_t *pcbStr);
HRESULT __cdecl StringCbPrintfA(LPSTR pszDest,size_t cbDest,LPCSTR pszFormat,...);
HRESULT __cdecl StringCbPrintfW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszFormat,...);
HRESULT __cdecl StringCbPrintfExA(LPSTR pszDest,size_t cbDest,LPSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags,LPCSTR pszFormat,...);
HRESULT __cdecl StringCbPrintfExW(LPWSTR pszDest,size_t cbDest,LPWSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags,LPCWSTR pszFormat,...);
HRESULT __cdecl StringCbVPrintfA(LPSTR pszDest,size_t cbDest,LPCSTR pszFormat,va_list argList);
HRESULT __cdecl StringCbVPrintfW(LPWSTR pszDest,size_t cbDest,LPCWSTR pszFormat,va_list argList);
HRESULT __cdecl StringCbVPrintfExA(LPSTR pszDest,size_t cbDest,LPSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags,LPCSTR pszFormat,va_list argList);
HRESULT __cdecl StringCbVPrintfExW(LPWSTR pszDest,size_t cbDest,LPWSTR *ppszDestEnd,size_t *pcbRemaining,DWORD dwFlags,LPCWSTR pszFormat,va_list argList);

/* StringCch routines (lengths in characters) */

HRESULT StringCchCatA(LPSTR pszDest,size_t cchDest,LPCSTR pszSrc);
HRESULT StringCchCatW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszSrc);
HRESULT StringCchCatExA(LPSTR pszDest,size_t cchDest,LPCSTR pszSrc,LPSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags);
HRESULT StringCchCatExW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszSrc,LPWSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags);
HRESULT StringCchCatNA(LPSTR pszDest,size_t cchDest,LPCSTR pszSrc,size_t cchToAppend);
HRESULT StringCchCatNW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszSrc,size_t cchToAppend);
HRESULT StringCchCatNExA(LPSTR pszDest,size_t cchDest,LPCSTR pszSrc,size_t cchToAppend,LPSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags);
HRESULT StringCchCatNExW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszSrc,size_t cchToAppend,LPWSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags);
HRESULT StringCchCopyA(LPSTR pszDest,size_t cchDest,LPCSTR pszSrc);
HRESULT StringCchCopyW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszSrc);
HRESULT StringCchCopyExA(LPSTR pszDest,size_t cchDest,LPCSTR pszSrc,LPSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags);
HRESULT StringCchCopyExW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszSrc,LPWSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags);
HRESULT StringCchCopyNA(LPSTR pszDest,size_t cchDest,LPCSTR pszSrc,size_t cchToCopy);
HRESULT StringCchCopyNW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszSrc,size_t cchToCopy);
HRESULT StringCchCopyNExA(LPSTR pszDest,size_t cchDest,LPCSTR pszSrc,size_t cchToCopy,LPSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags);
HRESULT StringCchCopyNExW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszSrc,size_t cchToCopy,LPWSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags);
HRESULT StringCchLengthA(LPCSTR psz,size_t cchMax,size_t *pcchStr);
HRESULT StringCchLengthW(LPCWSTR psz,size_t cchMax,size_t *pcchStr);
HRESULT __cdecl StringCchPrintfA(LPSTR pszDest,size_t cchDest,LPCSTR pszFormat,...);
HRESULT __cdecl StringCchPrintfW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszFormat,...);
HRESULT __cdecl StringCchPrintfExA(LPSTR pszDest,size_t cchDest,LPSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags,LPCSTR pszFormat,...);
HRESULT __cdecl StringCchPrintfExW(LPWSTR pszDest,size_t cchDest,LPWSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags,LPCWSTR pszFormat,...);
HRESULT __cdecl StringCchVPrintfA(LPSTR pszDest,size_t cchDest,LPCSTR pszFormat,va_list argList);
HRESULT __cdecl StringCchVPrintfW(LPWSTR pszDest,size_t cchDest,LPCWSTR pszFormat,va_list argList);
HRESULT __cdecl StringCchVPrintfExA(LPSTR pszDest,size_t cchDest,LPSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags,LPCSTR pszFormat,va_list argList);
HRESULT __cdecl StringCchVPrintfExW(LPWSTR pszDest,size_t cchDest,LPWSTR *ppszDestEnd,size_t *pcchRemaining,DWORD dwFlags,LPCWSTR pszFormat,va_list argList);

#ifdef UNICODE
#define StringCbCat StringCbCatW
#define StringCbCatEx StringCbCatExW
#define StringCbCatN StringCbCatNW
#define StringCbCatNEx StringCbCatNExW
#define StringCbCopy StringCbCopyW
#define StringCbCopyEx StringCbCopyExW
#define StringCbCopyN StringCbCopyNW
#define StringCbCopyNEx StringCbCopyNExW
#define StringCbLength StringCbLengthW
#define StringCbPrintf StringCbPrintfW
#define StringCbPrintfEx StringCbPrintfExW
#define StringCbVPrintf StringCbVPrintfW
#define StringCbVPrintfEx StringCbVPrintfExW
#define StringCchCat StringCchCatW
#define StringCchCatEx StringCchCatExW
#define StringCchCatN StringCchCatNW
#define StringCchCatNEx StringCchCatNExW
#define StringCchCopy StringCchCopyW
#define StringCchCopyEx StringCchCopyExW
#define StringCchCopyN StringCchCopyNW
#define StringCchCopyNEx StringCchCopyNExW
#define StringCchLength StringCchLengthW
#define StringCchPrintf StringCchPrintfW
#define StringCchPrintfEx StringCchPrintfExW
#define StringCchVPrintf StringCchVPrintfW
#define StringCchVPrintfEx StringCchVPrintfExW
#else
#define StringCbCat StringCbCatA
#define StringCbCatEx StringCbCatExA
#define StringCbCatN StringCbCatNA
#define StringCbCatNEx StringCbCatNExA
#define StringCbCopy StringCbCopyA
#define StringCbCopyEx StringCbCopyExA
#define StringCbCopyN StringCbCopyNA
#define StringCbCopyNEx StringCbCopyNExA
#define StringCbLength StringCbLengthA
#define StringCbPrintf StringCbPrintfA
#define StringCbPrintfEx StringCbPrintfExA
#define StringCbVPrintf StringCbVPrintfA
#define StringCbVPrintfEx StringCbVPrintfExA
#define StringCchCat StringCchCatA
#define StringCchCatEx StringCchCatExA
#define StringCchCatN StringCchCatNA
#define StringCchCatNEx StringCchCatNExA
#define StringCchCopy StringCchCopyA
#define StringCchCopyEx StringCchCopyExA
#define StringCchCopyN StringCchCopyNA
#define StringCchCopyNEx StringCchCopyNExA
#define StringCchLength StringCchLengthA
#define StringCchPrintf StringCchPrintfA
#define StringCchPrintfEx StringCchPrintfExA
#define StringCchVPrintf StringCchVPrintfA
#define StringCchVPrintfEx StringCchVPrintfExA
#endif

#ifdef __cplusplus
}
#endif
#endif /* _STRSAFE_H_INCLUDED */
