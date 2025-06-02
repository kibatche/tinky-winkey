#include "svc.h"

int main(int ac, char *av[])
{
    if (ac != 2)
    {
        printf(SVC_USAGE);
        exit(1);
    }
    SC_HANDLE SCManager = NULL;
    if (!strcmp(av[1], SVC_INSTALL))
    {
        OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (SCManager == NULL) exit(PrintError());
        InstallSvc(SCManager);
    }
    else if (!strcmp(av[1], SVC_START))
    {
        OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (SCManager == NULL) exit(PrintError());
        StartSvc(SCManager);
    }
    else if (!strcmp(av[1], SVC_STOP))
    {
        OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (SCManager == NULL) exit(PrintError());
        StopSvc(SCManager);
    }
    else if (!strcmp(av[1], SVC_DELETE))
    {
        OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (SCManager == NULL) exit(PrintError());
        DeleteSvc(SCManager);
    }
    else
    {
        printf(SVC_USAGE);
        exit(1);
    }
    exit(0);
}