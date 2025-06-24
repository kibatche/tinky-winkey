#include "svc.h"

int PrintError(void)
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
    printf(u8"Error svc.exe %d : %hs\n", err, buf);
    log(buf);
    return ERROR;
}