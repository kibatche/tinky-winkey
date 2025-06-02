#include "svc.h"

/**
 * @brief Installe le service. Le handler est cree avec differente option necessaires
 * a la creation du service.
 * 
 * @param SCManager 
 */
void InstallSvc(SC_HANDLE SCManager)
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
        exit(PrintError());
    }
    printf("Service %s installed.\n", SVC_NAME);
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}

/**
 * Demarre le service s'il n'est pas deja demarre. Auparavant, usurpe un token system en dupliquant le token de winlogon.exe (cf ImpersonateSystemToken()),
 * de telle facon a ce que le thread courant fonctionne avec les droits NT AUTHORITY/SYSTEM
 */

 // NE PAS OUBLIER DE CHECKER SI LE SERVICE EST UP AVANT DE VOULOIR LE DEMARRER.
void StartSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, SERVICE_START);
    // LPBYTE svcInfoStruct;
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(PrintError());
    }
    // if (!QueryServiceStatusEx(handlerSvc, SC_STATUS_PROCESS_INFO, ))
    ImpersonateSystemToken();
    BOOL success = StartService(handlerSvc, 0, NULL);
    if (!success)
    {
        CloseServiceHandle(SCManager);
        CloseServiceHandle(handlerSvc);
        exit(PrintError());
    }
    printf("Service %s started successfully.\n", SVC_NAME);
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}

/**
 * Stoppe le service
 */
void StopSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, SERVICE_STOP);
    LPSERVICE_STATUS stat = NULL;
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(PrintError());
    }
    BOOL success = ControlService(handlerSvc, SERVICE_CONTROL_STOP, stat);
    if (!success)
    {
        CloseServiceHandle(SCManager);
        CloseServiceHandle(handlerSvc);
        exit(PrintError());
    }
    printf("Service %s stopped successfully.\n", SVC_NAME);
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}

/**
 * Detruit le service
 */
void DeleteSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, DELETE);
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(PrintError());
    }
    BOOL success = DeleteService(handlerSvc);
    if (!success)
    {
        CloseServiceHandle(SCManager);
        CloseServiceHandle(handlerSvc);
        exit(PrintError());
    }
    printf("Service %s deleted successfully.\n", SVC_NAME);
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}