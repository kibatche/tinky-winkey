#include "winkey.h"

INT PrintError(void)
{
    int err = GetLastError();
    int sz;
    char buf[512];

    sz = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        0,
        buf,
        512,
        NULL );
    if (!sz)//the error is not known
    {
        printf("Unknown error.\n");
        return ERROR;
    }
    printf(u8"Error winkey.exe %d : %hs\n", err, buf);
    return ERROR;
}