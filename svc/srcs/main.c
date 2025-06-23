#include "svc.h"

SERVICE_STATUS svcStatus;
SERVICE_STATUS_HANDLE svcStatusHandle;
HANDLE svcStopEvt = NULL; 

int main(int ac, char *av[])
{
    if (ac == 1)
    {
        SERVICE_TABLE_ENTRY DispatchTable[] = {
            {
                SVC_NAME, (LPSERVICE_MAIN_FUNCTION)SvcMain
            },
            {
                NULL, NULL
            }
        };
        if (!StartServiceCtrlDispatcher(DispatchTable))
        {
            log("[Unable to connect to SCM.]\n");
            exit(1);
        }
    }
    else if (ac == 2)
    {
        SC_HANDLE SCManager = NULL;
        if (!strcmp(av[1], SVC_INSTALL))
        {
            SCManager =  OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (SCManager == NULL) exit(PrintError());
            InstallSvc(SCManager);
        }
        else if (!strcmp(av[1], SVC_START))
        {
            SCManager =  OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (SCManager == NULL) exit(PrintError());
            StartSvc(SCManager);
        }
        else if (!strcmp(av[1], SVC_STOP))
        {
            SCManager =  OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (SCManager == NULL) exit(PrintError());
            StopSvc(SCManager);
        }
        else if (!strcmp(av[1], SVC_DELETE))
        {
            SCManager =  OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (SCManager == NULL) exit(PrintError());
            DeleteSvc(SCManager);
        }
        else
        {
            printf(SVC_USAGE);
            exit(1);
        }
    }
    else
    {
        printf(SVC_USAGE);
        exit(1);
    }  
    exit(0);
}