#include "svc.h"

VOID log(void *toLog)
{
    FILE *f = fopen("C:\\debug.txt", "a+");
    fprintf(f, "%s", (LPSTR)toLog);
    fclose(f);
}