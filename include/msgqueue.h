/*
 * msgqueue.h
 */
#ifndef	_MSGQUEUE_H_
#define	_MSGQUEUE_H_

#if __GNUC__ >= 3
#pragma GCC system_header
#endif


#ifdef  __cplusplus
extern "C" {
#endif

typedef struct MSGQUEUEOPTIONS_OS {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwMaxMessages;
	DWORD cbMaxMessage;
	BOOL bReadAccess;
} MSGQUEUEOPTIONS, *LPMSGQUEUEOPTIONS, *PMSGQUEUEOPTIONS;

typedef struct MSGQUEUEINFO {
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwMaxMessages;
	DWORD cbMaxMessage;
	DWORD dwCurrentMessages;
	DWORD dwMaxQueueMessages;
	WORD wNumReaders;
	WORD wNumWriters;
} MSGQUEUEINFO, *PMSGQUEUEINFO, *LPMSGQUEUEINFO;

HANDLE CreateMsgQueue(LPCWSTR lpszName, LPMSGQUEUEOPTIONS lpOptions);
BOOL CloseMsgQueue(HANDLE hMsgQ);
BOOL ReadMsgQueue(HANDLE hMsgQ, LPVOID lpBuffer,
		DWORD cbBufferSize, LPDWORD lpNumberOfBytesRead,
		DWORD dwTimeout, DWORD *pdwFlags);

#define	MSGQUEUE_NOPRECOMMIT	1	/* ?? */
#define	MSGQUEUE_ALLOW_BROKEN	2	/* ?? */





#if (_WIN32_WCE >= 0x0600)
BOOL WINAPI ReadMsgQueueEx(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbBufferSize,
                LPDWORD lpNumberOfBytesRead, DWORD dwTimeout, DWORD *pdwFlags, PHANDLE phTok);
#endif
BOOL WINAPI WriteMsgQueue(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbDataSize,
                DWORD dwTimeout, DWORD dwFlags);
BOOL WINAPI GetMsgQueueInfo(HANDLE hMsgQ, LPMSGQUEUEINFO lpInfo);

#ifdef  __cplusplus
}
#endif

#ifdef _WIN32_WCE
/* Message-queue and device-notification compatibility interface. */
HANDLE WINAPI OpenMsgQueue(HANDLE hSrcProc, HANDLE hMsgQ, LPMSGQUEUEOPTIONS lpOptions);
HANDLE WINAPI RequestDeviceNotifications(const GUID *devclass, HANDLE hMsgQ, BOOL fAll);
BOOL WINAPI StopDeviceNotifications(HANDLE h);
#endif
#endif  /* _MSGQUEUE_H_ */
