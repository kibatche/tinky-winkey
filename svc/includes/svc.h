#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include "consts.h"

void InstallSvc(SC_HANDLE SCManager);
void StartSvc(SC_HANDLE SCManager);
void StopSvc(SC_HANDLE SCManager);
void DeleteSvc(SC_HANDLE SCManager);

void ImpersonateSystemToken(void);
void PrintUserNameByProc(void);
void PrintUserNameByThread(void);
void PrintPrivileges(HANDLE hToken);
DWORD GetPIDByProcName(void);
BOOL EnableAllPrivilege(HANDLE currentToken);
void ImpersonateSystemToken(void);

int PrintError(void);