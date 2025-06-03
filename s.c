#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

LRESULT CALLBACK wndproc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message != WM_INPUT)
        return DefWindowProc(window, message, wparam, lparam);
    char rid_buf[64];
    UINT rid_size = sizeof(rid_buf);
    if (GetRawInputData((HRAWINPUT)lparam, RID_INPUT, rid_buf, &rid_size, sizeof(RAWINPUTHEADER)))
    {
        RAWINPUT *raw = (RAWINPUT *)rid_buf;
        if (raw->header.dwType == RIM_TYPEKEYBOARD)
        {
            RAWKEYBOARD *rk = &raw->data.keyboard;
            process_kbd_event(rk->MakeCode,
                              rk->Flags & RI_KEY_E0,
                              rk->Flags & RI_KEY_E1,
                              rk->Flags & RI_KEY_BREAK,
                              rk->VKey);
        }
    }
    return DefWindowProc(window, message, wparam, lparam);
}
int main(void)
{
    // define a window class which is required to receive RAWINPUT events
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = wndproc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "rawkbd_wndclass";
    // register class
    if (!RegisterClassExA(&wc))
        return -1;
    // create window
    HWND rawkbd_wnd = CreateWindowExA(0, wc.lpszClassName, NULL, 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetModuleHandle(NULL), NULL);
    if (!rawkbd_wnd)
        return -2;
    // setup raw input device sink
    RAWINPUTDEVICE devs = {0x01 /* generic */, 0x06 /* keyboard */, RIDEV_INPUTSINK, rawkbd_wnd};
    if (RegisterRawInputDevices(&devs, 1, sizeof(RAWINPUTDEVICE)) == FALSE)
        return -3;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    // cleanup
    DestroyWindow(rawkbd_wnd);
    UnregisterClass(wc.lpszClassName, GetModuleHandle(NULL));
    return 0;
}