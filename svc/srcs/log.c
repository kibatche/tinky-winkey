#include "svc.h"

VOID log(void *toLog)
{
    #ifndef DEBUG
     (void)toLog;
    #endif
    #ifdef DEBUG
    FILE *f = fopen("C:\\debug.txt", "a+");
    fprintf(f, "%s\n", (LPSTR)toLog);
    fclose(f);
    #endif
}