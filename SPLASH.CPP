/***************************************************************************
 *
 *  SPLASH.CPP - Splash screen
 *  Translated from SPLASH.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Displays the BANNERCL.BMP bitmap centred on screen during startup.
 *  The window stays alive (off-screen) after hiding so WinMain always
 *  has a visible "main window" handle.
 *
 ***************************************************************************/

#include <windows.h>
#include "SPLASH.H"
#include "RESOURCE.H"

#define SPLASH_CLASS  "CalmiraSplash"

static HINSTANCE s_hInst  = 0;
static HBITMAP   s_hBmp   = 0;
static int       s_bmpW   = 0;
static int       s_bmpH   = 0;
static HWND      s_hwndStatus = 0;

LRESULT CALLBACK __export SplashWndProc(HWND hwnd, UINT msg,
                                         WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (s_hBmp) {
                HDC hdcMem = CreateCompatibleDC(hdc);
                HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, s_hBmp);
                BitBlt(hdc, 0, 0, s_bmpW, s_bmpH, hdcMem, 0, 0, SRCCOPY);
                SelectObject(hdcMem, hOld);
                DeleteDC(hdcMem);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }
        case WM_DESTROY: {
            if (s_hBmp) { DeleteObject(s_hBmp); s_hBmp = 0; }
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

static BOOL RegisterSplashClass(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style         = CS_SAVEBITS;
    wc.lpfnWndProc   = SplashWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInst;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor(NULL, IDC_WAIT);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = SPLASH_CLASS;
    return RegisterClass(&wc) != 0;
}

HWND Splash_Create(HINSTANCE hInst, HWND hwndParent)
{
    HWND   hwnd;
    int    sw, sh, x, y, w, h;
    BITMAP bm;

    s_hInst = hInst;

    RegisterSplashClass(hInst);

    /* Load the banner bitmap */
    s_hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BANNER));
    if (s_hBmp) {
        GetObject(s_hBmp, sizeof(bm), &bm);
        s_bmpW = bm.bmWidth;
        s_bmpH = bm.bmHeight;
    } else {
        s_bmpW = 300;
        s_bmpH = 140;
    }

    /* Status label height */
    h = s_bmpH + 20;
    w = s_bmpW;

    /* Centre on screen */
    sw = GetSystemMetrics(SM_CXSCREEN);
    sh = GetSystemMetrics(SM_CYSCREEN);
    x  = (sw - w) / 2;
    y  = (sh - h) / 2;

    hwnd = CreateWindow(SPLASH_CLASS, "",
                        WS_POPUP | WS_BORDER,
                        x, y, w, h,
                        hwndParent, NULL, hInst, NULL);

    if (hwnd) {
        /* Status text label */
        s_hwndStatus = CreateWindow("STATIC", "Loading Calmira...",
                                    WS_CHILD | WS_VISIBLE | SS_LEFT,
                                    4, s_bmpH + 2, w - 8, 14,
                                    hwnd, NULL, hInst, NULL);
    }
    return hwnd;
}

void Splash_SetStatus(HWND hwndSplash, LPCSTR szText)
{
    if (s_hwndStatus) {
        SetWindowText(s_hwndStatus, szText);
    }
}

void Splash_Hide(HWND hwndSplash)
{
    ShowWindow(hwndSplash, SW_HIDE);
    SetWindowPos(hwndSplash, NULL, -9999, -9999, 1, 1,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
}
