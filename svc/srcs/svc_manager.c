#include "svc.h"

void installSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = CreateService(SCManager,
    SVC_NAME,
    SVC_NAME,
    SC_MANAGER_ALL_ACCESS,
    SERVICE_WIN32_OWN_PROCESS,
    SERVICE_DEMAND_START,
    SERVICE_ERROR_IGNORE,
    SVC_BIN,
    NULL, NULL, NULL, NULL, "");
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(print_err());
    }
    printf("Service %s installed.\n", SVC_NAME);
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}

void startSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, SERVICE_START);
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(print_err());
    }
    BOOL success = StartService(handlerSvc, 0, NULL);
    if (!success)
    {
        CloseServiceHandle(SCManager);
        CloseServiceHandle(handlerSvc);
        exit(print_err());
    }
    printf("Service %s started successfully.\n", SVC_NAME);
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}

void stopSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, SERVICE_STOP);
    LPSERVICE_STATUS stat = NULL;
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(print_err());
    }
    BOOL success = ControlService(handlerSvc, SERVICE_CONTROL_STOP, stat);
    if (!success)
    {
        CloseServiceHandle(SCManager);
        CloseServiceHandle(handlerSvc);
        exit(print_err());
    }
    printf("Service %s stopped successfully.\n", SVC_NAME);
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}

void deleteSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, DELETE);
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(print_err());
    }
    BOOL success = DeleteService(handlerSvc);
    if (!success)
    {
        CloseServiceHandle(SCManager);
        CloseServiceHandle(handlerSvc);
        exit(print_err());
    }
    printf("Service %s deleted successfully.\n", SVC_NAME);
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}