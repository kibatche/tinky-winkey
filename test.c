#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
int main()
{
    HANDLE handleProc = NULL;
    PROCESSENTRY32 pe32;
    handleProc = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (handleProc == INVALID_HANDLE_VALUE)
    {
        printf("Error\n");
        return 1;
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(handleProc, &pe32))
    {
        printf("error\n");
        CloseHandle(handleProc);
        return 1;
    }
    if (!strcmp(pe32.szExeFile, "winlogon.exe"))
    {
        printf("Found winlogon.exe with PID %d\n", pe32.th32ProcessID);
        CloseHandle(handleProc);
        return 1;
    }
    while (Process32Next(handleProc, &pe32))
    {
        if (!strcmp(pe32.szExeFile, "winlogon.exe"))
        {
            printf("Found winlogon.exe with PID %d\n", pe32.th32ProcessID);
            CloseHandle(handleProc);
            return 1;
        }   
    }
}