#include "winkey.h"

extern HANDLE svcStopEvt;
extern SERVICE_STATUS_HANDLE svcStatusHandle;
extern SERVICE_STATUS svcStatus;

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
    //on previent le gestionnaire de service que le servie a demarre
    ReportSvcStatus(SERVICE_RUNNING, NO_ERROR, 0);

    /**** TEST */
    while (1)
    {
        DWORD stopEvt = WaitForSingleObject(svcStopEvt, 1);
        if (stopEvt == WAIT_OBJECT_0)
        {
            ReportSvcStatus(SERVICE_STOPPED, NO_ERROR, 0);
            exit(0);
        }
        Sleep(1000);
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
    // le checkpoint est utile lors des operation longues, et lorsaue le service
    // doit faire plusieurs operation avant de demarrer
    if (currentState == SERVICE_RUNNING || currentState == SERVICE_STOPPED)
        svcStatus.dwCheckPoint = 0;
    else
        svcStatus.dwCheckPoint = savedCheckPoint++;
    //on set le statut du service pour le gestionnaire de service.
    SetServiceStatus(svcStatusHandle, &svcStatus);
}