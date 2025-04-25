#include "svc.h"

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

BOOL SetDebugPrivilege()
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
    {
        print_err();
        return FALSE;
    }
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(GetCurrentProcess(), FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PWORD)NULL))
    {
        print_err();
        return FALSE;
    }
    printf("%s enabled.\n", SE_DEBUG_NAME);
    return TRUE;
}

void ImpersonateSystemToken()
{
    HANDLE sysToken = NULL;
    HANDLE procHandle = NULL;
    HANDLE newSysTok = NULL;

    if (!SetDebugPrivilege())
        exit(1);
    procHandle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, TRUE, GetPIDByProcName());
    if (procHandle == NULL)
    {
        printf("Impossible to get the process handle.\n");
        exit(1);
    }
    BOOL success = OpenProcessToken(procHandle, MAXIMUM_ALLOWED, &sysToken);
    if (!success)
    {
        printf("Could not open the process token.\n");
        exit(1);
    }
    success = DuplicateTokenEx(sysToken, MAXIMUM_ALLOWED, NULL, SecurityImpersonation,  TokenPrimary, &newSysTok);
    if (!success)
    {
        printf("Could not duplicqte the NT/AUTHORITY SYSTEM token.\n");
        exit(1);
    }
    
}