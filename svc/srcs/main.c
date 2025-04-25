#include "svc.h"

int main(int ac, char *av[])
{
    if (ac != 2)
    {
        printf(SVC_USAGE);
        exit(1);
    }
    SC_HANDLE SCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (SCManager == NULL)
        exit(print_err());
    if (!strcmp(av[1], SVC_INSTALL))
        installSvc(SCManager);
    else if (!strcmp(av[1], SVC_START))
        startSvc(SCManager);
    else if (!strcmp(av[1], SVC_STOP))
        stopSvc(SCManager);
    else if (!strcmp(av[1], SVC_DELETE))
        deleteSvc(SCManager);
    else
    {
        printf(SVC_USAGE);
        exit(1);
    }
    exit(0);
}