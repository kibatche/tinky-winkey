#include "winkey.h"

SERVICE_STATUS svcStatus;
SERVICE_STATUS_HANDLE svcStatusHandle;
HANDLE svcStopEvt = NULL; 


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
    FILE *f = fopen("C:\\service_log.txt", "a+");
    fprintf(f, "Service started\n");
    fclose(f);
    if (!StartServiceCtrlDispatcher(DispatchTable))
        printf("Impossible de connecter le service au Gestionnaire de service principal.\n");
}
