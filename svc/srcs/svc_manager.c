#include "svc.h"

/**
 * @brief Installe le service. Le handler est cree avec differente option necessaires
 * a la creation du service.
 * 
 * @param SCManager 
 */
VOID InstallSvc(SC_HANDLE SCManager)
{
    char cwd[MAX_PATH];
    GetCurrentDirectory((DWORD)MAX_PATH, (LPSTR)&cwd);
    size_t totalLenSvcBinPath = strlen(cwd) + (size_t)1 + strlen(SVC_BIN) + (size_t)1; 
    char *binaryPath = malloc(totalLenSvcBinPath);

    if (binaryPath == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(PrintError());
    }
    strcpy_s(binaryPath, totalLenSvcBinPath, cwd);
    strcat_s(binaryPath, totalLenSvcBinPath, "\\");
    strcat_s(binaryPath, totalLenSvcBinPath, SVC_BIN);

    SC_HANDLE handlerSvc = CreateServiceA(SCManager,
    SVC_NAME,
    SVC_NAME,
    SC_MANAGER_ALL_ACCESS,
    SERVICE_WIN32_OWN_PROCESS,
    SERVICE_DEMAND_START,
    SERVICE_ERROR_IGNORE,
    binaryPath,
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
VOID StartSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, SERVICE_START);
    char choosenSessionStr[3];
    const char *av[] = {choosenSessionStr};
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(PrintError());
    }
    DWORD choosenSession = ChooseSessionToLog();
    if (choosenSession == 0)
    {
        CloseServiceHandle(SCManager);
        CloseServiceHandle(handlerSvc);
        exit(PrintError());
    }
    _itoa_s(choosenSession, choosenSessionStr, _countof(choosenSessionStr) ,10);
    if (strlen(choosenSessionStr) == 0)
    {
        printf("Itoa failed.\n");
        CloseServiceHandle(SCManager);
        CloseServiceHandle(handlerSvc);
        exit(PrintError());
    }
    BOOL success = StartServiceA(handlerSvc, 1, av);
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
VOID StopSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, SERVICE_STOP);
    SERVICE_STATUS_PROCESS stat;
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(PrintError());
    }
    BOOL success = ControlService(handlerSvc, SERVICE_CONTROL_STOP, (LPSERVICE_STATUS)&stat);
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
VOID DeleteSvc(SC_HANDLE SCManager)
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

VOID UpdateSvc(SC_HANDLE SCManager)
{
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, SERVICE_CHANGE_CONFIG);
    char displayName[4096];
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(PrintError());
    }
    printf("Write the new display name for the service :\n");
    scanf_s("%100s", &displayName, (unsigned)_countof(displayName));
    printf("%s\n", displayName);
    BOOL success = ChangeServiceConfigA(handlerSvc, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, displayName);
    if (success == FALSE) printf("Failed to update the service display name.\n");
    else printf("Service updated successfully.\n");
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}