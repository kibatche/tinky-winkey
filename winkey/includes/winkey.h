#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>

#include "consts.h"



VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID SvcInit(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI SvcCtrlHandler(DWORD ctrl);
VOID ReportSvcStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint);