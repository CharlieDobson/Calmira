/***************************************************************************
 *
 *  TASK.CPP - Taskbar window
 *  Translated from TASK.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Taskbar layout (bottom-of-screen, single row):
 *
 *   [Start][Btn1][Btn2]...[BtnN]         [Clock]
 *   ^3px   ^button area                  ^right
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include "TASK.H"
#include "SETTINGS.H"
#include "RESOURCE.H"
#include "HOOKS.H"
#include "HELPERS.H"
#include "START.H"
#include "SHUTDWN.H"
#include "TASKMAN.H"
#include "STRTPROP.H"
#include "..\CALMSGS.H"

extern HINSTANCE g_hInst;

/* Clock width in pixels */
#define CLOCK_W     64
/* Start button width (read from INI at Configure time) */
#define START_MIN_W 48

/* Per-taskbar state */
typedef struct tagTASKSTATE {
    HINSTANCE hInst;
    HWND      hwndMain;         /* CALMIRA_CLASS main window */
    TASKBTN   aBtns[MAX_TASKS];
    int       nBtns;
    int       nStartW;          /* start button pixel width  */
    int       nBarHeight;       /* total bar height in pixels */
    HBITMAP   hbmStart;         /* normal start button bitmap */
    HBITMAP   hbmStartPush;     /* pressed start button bitmap */
    BOOL      bStartPressed;
    BOOL      bVisible;
    BOOL      bAutoHide;        /* StayVisible == FALSE        */
} TASKSTATE;

#define TASK_EXTRA  sizeof(LONG)
#define GET_TSTATE(hwnd)  ((TASKSTATE FAR *)GetWindowLong(hwnd, 0))

/* -----------------------------------------------------------------------
 * Utility: truncate window title and optionally strip path
 * ----------------------------------------------------------------------- */
static void BuildButtonCaption(HWND hwndApp, LPSTR buf, int bufLen)
{
    char title[128];
    GetWindowText(hwndApp, title, sizeof(title));

    if (g_DocNameFirst || g_ShortWinCaptions) {
        /* Attempt to extract "filename - appname" -> "filename" */
        char *dash = title;
        while (*dash && *dash != '-') dash++;
        if (*dash == '-' && dash > title) {
            /* trim trailing spaces before dash */
            char *p = dash - 1;
            while (p > title && *p == ' ') p--;
            *(p+1) = '\0';
        }
    }
    if (g_DocNameLower) {
        AnsiLower(title);
    }
    lstrcpyn(buf, title, bufLen);
}

/* -----------------------------------------------------------------------
 * Find a task button index by app window handle (-1 if not found)
 * ----------------------------------------------------------------------- */
static int FindButton(TASKSTATE FAR *ps, HWND hwndApp)
{
    int i;
    for (i = 0; i < ps->nBtns; i++)
        if (ps->aBtns[i].hwnd == hwndApp) {
            return i;
        }
    return -1;
}

/* -----------------------------------------------------------------------
 * Compute the bounding rect for a task button (0-based index)
 * ----------------------------------------------------------------------- */
static RECT ButtonRect(TASKSTATE FAR *ps, HWND hwndTask, int idx)
{
    RECT rc, client;
    int  btnArea, btnW;
    GetClientRect(hwndTask, &client);
    btnArea = client.right - ps->nStartW - CLOCK_W - 4;
    btnW    = (ps->nBtns > 0) ? (btnArea / ps->nBtns) : btnArea;
    if (btnW < 20) {
        btnW = 20;
    }
    rc.left   = ps->nStartW + idx * btnW;
    rc.top    = 2;
    rc.right  = rc.left + btnW - 2;
    rc.bottom = client.bottom - 2;
    return rc;
}

/* -----------------------------------------------------------------------
 * Paint the taskbar
 * ----------------------------------------------------------------------- */
static void PaintTaskbar(HWND hwnd, TASKSTATE FAR *ps)
{
    PAINTSTRUCT ps2;
    HDC   hdc = BeginPaint(hwnd, &ps2);
    RECT  client;
    int   i;
    HFONT hFont, hOld;
    TEXTMETRIC tm;

    if (!ps) {
        EndPaint(hwnd, &ps2);
        return;
    }

    GetClientRect(hwnd, &client);
    FillRect(hdc, &client, GetSysColorBrush(COLOR_BTNFACE));
    Draw3dRect(hdc, 0, 0, client.right, client.bottom,
               GetSysColor(COLOR_BTNHIGHLIGHT),
               GetSysColor(COLOR_BTNSHADOW));

    hFont = CreateFont(-11, 0, 0, 0, FW_NORMAL, 0, 0, 0,
                       ANSI_CHARSET, 0, 0, DEFAULT_QUALITY,
                       DEFAULT_PITCH | FF_DONTCARE, "MS Sans Serif");
    hOld = (HFONT)SelectObject(hdc, hFont);
    GetTextMetrics(hdc, &tm);
    SetBkMode(hdc, TRANSPARENT);

    /* Draw Start button */
    {
        RECT rcS;
        rcS.left = 3; rcS.top = 2;
        rcS.right = ps->nStartW - 1; rcS.bottom = client.bottom - 2;
        DrawButtonFace(hdc, &rcS, ps->bStartPressed, "Start");
    }

    /* Draw task buttons */
    for (i = 0; i < ps->nBtns; i++) {
        TASKBTN FAR *pb = &ps->aBtns[i];
        RECT rc = ButtonRect(ps, hwnd, i);
        BOOL bActive = pb->bActive;
        DrawButtonFace(hdc, &rc, bActive, pb->szCaption);
        if (pb->hIcon) {
            int ix = rc.left + 4;
            int iy = rc.top + (rc.bottom - rc.top - 16) / 2;
            DrawIcon(hdc, ix, iy, pb->hIcon);
        }
    }

    /* Draw clock */
    {
        SYSTEMTIME st;
        char szTime[16];
        RECT rcClk;
        GetLocalTime(&st);
        if (g_Clock24) {
            wsprintf(szTime, "%02d:%02d", (int)st.wHour, (int)st.wMinute);
        }
        else {
            int h = st.wHour % 12;
            if (!h) {
                h = 12;
            }
            wsprintf(szTime, "%d:%02d%s", h, (int)st.wMinute,
                     st.wHour < 12 ? "a" : "p");
        }
        rcClk.left   = client.right - CLOCK_W;
        rcClk.top    = 2;
        rcClk.right  = client.right - 2;
        rcClk.bottom = client.bottom - 2;
        DrawEdge(hdc, &rcClk, EDGE_SUNKEN, BF_RECT);
        DrawText(hdc, szTime, -1, &rcClk,
                 DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    SelectObject(hdc, hOld);
    DeleteObject(hFont);
    EndPaint(hwnd, &ps2);
}

/* -----------------------------------------------------------------------
 * EnumWindows callback - seeds the taskbar with pre-existing windows
 * ----------------------------------------------------------------------- */
BOOL CALLBACK __export SeedWindowsProc(HWND hwndApp, LPARAM lParam)
{
    Taskbar_AddWindow((HWND)lParam, hwndApp);
    return TRUE;   /* continue enumeration */
}

/* -----------------------------------------------------------------------
 * Taskbar window procedure
 * ----------------------------------------------------------------------- */
LRESULT CALLBACK __export TaskbarWndProc(HWND hwnd, UINT msg,
                                          WPARAM wParam, LPARAM lParam)
{
    TASKSTATE FAR *ps = GET_TSTATE(hwnd);

    switch (msg) {
        case WM_CREATE: {
            TASKSTATE FAR *pNew =
                (TASKSTATE FAR *)GlobalAllocPtr(GHND, sizeof(TASKSTATE));
            if (!pNew) {
                return -1;
            }
            pNew->hInst    = ((LPCREATESTRUCT)lParam)->hInstance;
            pNew->hwndMain = ((LPCREATESTRUCT)lParam)->hwndParent;
            pNew->nBtns    = 0;
            pNew->nStartW  = Profile_GetInt(&g_Ini, "Start Button", "Width", 56);
            pNew->nBarHeight = TASKBAR_HEIGHT * g_RowCount;
            pNew->bVisible = TRUE;
            SetWindowLong(hwnd, 0, (LONG)pNew);

            /* Start clock timer */
            SetTimer(hwnd, TIMER_CLOCK, 10000, NULL);

            /* Install hooks */
            if (HOOKS_OK()) {
                g_Hooks.SetCallBackWnd(hwnd);
                g_Hooks.StartTaskMonitor();
                g_Hooks.SetWndHook();
                if (!g_DisableTaskbar) {
                    g_Hooks.StartMouseMonitor();
                }
                if (g_ShrinkMax) {
                    int yTop  = g_TopTaskbar ? pNew->nBarHeight : 0;
                    int yLim  = g_TopTaskbar
                                ? GetSystemMetrics(SM_CYSCREEN)
                                : GetSystemMetrics(SM_CYSCREEN) - pNew->nBarHeight;
                    g_Hooks.SetYLimit(yTop, yLim);
                    g_Hooks.SetMaxEnabled(TRUE);
                }
                if (g_Win95Keyboard) {
                    g_Hooks.StartKeyboardHook(hwnd);
                }
                if (g_RightClose) {
                    g_Hooks.SetRCloseEnabled(TRUE);
                }
            }

            /* Seed the task list with windows already open.
               EnumWindows needs a BOOL CALLBACK (HWND, LPARAM) so wrap the call. */
            {
                struct { HWND hwndTask; } seed;
                seed.hwndTask = hwnd;
                /* Cast is safe: we only pass the taskbar HWND as lParam */
                EnumWindows((WNDENUMPROC)SeedWindowsProc, (LPARAM)(UINT)hwnd);
            }
            return 0;
        }

        case WM_DESTROY: {
            KillTimer(hwnd, TIMER_CLOCK);
            KillTimer(hwnd, TIMER_HIDE);
            if (HOOKS_OK()) {
                g_Hooks.StopTaskMonitor();
                g_Hooks.UnhookWndHook();
                g_Hooks.StopMouseMonitor();
                g_Hooks.StopKeyboardHook();
                g_Hooks.ReleaseDesktopHook();
            }
            if (ps) {
                GlobalFreePtr(ps);
                SetWindowLong(hwnd, 0, 0L);
            }
            return 0;
        }

        case WM_PAINT: {
            PaintTaskbar(hwnd, ps);
            return 0;
        }

        case WM_TIMER: {
            if (wParam == TIMER_CLOCK) {
                InvalidateRect(hwnd, NULL, FALSE);
            }
            return 0;
        }

        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            /* Start button? */
            if (x < ps->nStartW) {
                RECT  client;
                POINT pt;
                pt.x = 0; pt.y = 0;
                GetClientRect(hwnd, &client);
                ClientToScreen(hwnd, &pt);
                ps->bStartPressed = TRUE;
                InvalidateRect(hwnd, NULL, FALSE);
                StartMenu_Show(hwnd, pt.x + 3,
                               g_TopTaskbar ? pt.y + client.bottom
                                            : pt.y);
                ps->bStartPressed = FALSE;
                InvalidateRect(hwnd, NULL, FALSE);
                return 0;
            }
            /* Task button */
            {
                int i;
                for (i = 0; i < ps->nBtns; i++) {
                    RECT rc = ButtonRect(ps, hwnd, i);
                    if (x >= rc.left && x <= rc.right) {
                        HWND hwApp = ps->aBtns[i].hwnd;
                        if (IsIconic(hwApp)) {
                            ShowWindow(hwApp, SW_RESTORE);
                        }
                        else if (ps->aBtns[i].bActive) {
                            ShowWindow(hwApp, SW_MINIMIZE);
                        }
                        else {
                            SetForegroundWindow(hwApp);
                        }
                        break;
                    }
                }
            }
            return 0;
        }

        case WM_RBUTTONDOWN: {
            int x = LOWORD(lParam);
            int i;
            for (i = 0; i < ps->nBtns; i++) {
                RECT rc = ButtonRect(ps, hwnd, i);
                if (x >= rc.left && x <= rc.right) {
                    /* Show system menu of the app window */
                    HWND hwApp = ps->aBtns[i].hwnd;
                    if (hwApp) {
                        HMENU hSys = GetSystemMenu(hwApp, FALSE);
                        if (hSys) {
                            POINT pt;
                            pt.x = LOWORD(lParam);
                            pt.y = HIWORD(lParam);
                            ClientToScreen(hwnd, &pt);
                            TrackPopupMenu(hSys,
                                TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                                pt.x, pt.y, 0, hwApp, NULL);
                        }
                    }
                    break;
                }
            }
            return 0;
        }

        /* Messages from WNDHOOKS.DLL */
        case WM_SHELLWNDCREATE: {
            Taskbar_AddWindow(hwnd, (HWND)wParam);
            return 0;
        }

        case WM_SHELLWNDDESTROY: {
            Taskbar_RemoveWindow(hwnd, (HWND)wParam);
            return 0;
        }

        case WM_WINACTIVE: {
            Taskbar_ActivateWindow(hwnd, (HWND)wParam, (BOOL)lParam);
            return 0;
        }

        case WM_MOUSEHOOK: {
            /* Auto-show taskbar when cursor is near it */
            if (!ps->bVisible && !g_StayVisible) {
                ps->bVisible = TRUE;
                Taskbar_Configure(hwnd);
            }
            return 0;
        }

        case WM_HIDEQUERY: {
            /* A window was minimised - potentially hide taskbar */
            return 0;
        }

        case WM_KEYBOARDHOOK: {
            /* Hotkey: could map to taskbar shortcuts */
            return 0;
        }

        case WM_WINDOWSKEY: {
            /* Windows95 Start key: toggle the start menu */
            {
                RECT  client;
                POINT pt;
                GetClientRect(hwnd, &client);
                pt.x = 0; pt.y = 0;
                ClientToScreen(hwnd, &pt);
                StartMenu_Show(hwnd, pt.x + 3,
                               g_TopTaskbar ? pt.y + client.bottom : pt.y);
            }
            return 0;
        }

        case WM_SETTINGSCHANGED: {
            Taskbar_OnSettingsChanged(hwnd, (TSettingChanges)wParam);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

BOOL Taskbar_RegisterClass(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style         = CS_DBLCLKS;
    wc.lpfnWndProc   = TaskbarWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = TASK_EXTRA;
    wc.hInstance     = hInst;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = TASKBAR_CLASS;
    return RegisterClass(&wc) != 0;
}

HWND Taskbar_Create(HINSTANCE hInst, HWND hwndParent)
{
    int sw = GetSystemMetrics(SM_CXSCREEN);
    int sh = GetSystemMetrics(SM_CYSCREEN);
    int h  = TASKBAR_HEIGHT;
    int y  = g_TopTaskbar ? 0 : sh - h;

    if (g_DisableTaskbar) {
        return 0;
    }

    return CreateWindow(TASKBAR_CLASS, "",
                        WS_POPUP | WS_VISIBLE,
                        0, y, sw, h,
                        hwndParent, NULL, hInst, NULL);
}

void Taskbar_Configure(HWND hwndTask)
{
    TASKSTATE FAR *ps;
    int sw, sh, y, h;
    if (!hwndTask) {
        return;
    }
    ps = GET_TSTATE(hwndTask);
    if (!ps) {
        return;
    }

    sw = GetSystemMetrics(SM_CXSCREEN);
    sh = GetSystemMetrics(SM_CYSCREEN);
    h  = TASKBAR_HEIGHT * g_RowCount;
    y  = g_TopTaskbar ? 0 : sh - h;

    SetWindowPos(hwndTask, HWND_TOPMOST, 0, y, sw, h,
                 SWP_NOACTIVATE);

    if (HOOKS_OK() && g_ShrinkMax) {
        int yTop = g_TopTaskbar ? h : 0;
        int yLim = g_TopTaskbar ? sh : sh - h;
        g_Hooks.SetYLimit(yTop, yLim);
        g_Hooks.SetMaxEnabled(TRUE);
    }
    InvalidateRect(hwndTask, NULL, TRUE);
}

void Taskbar_OnSettingsChanged(HWND hwndTask, TSettingChanges changes)
{
    if (!hwndTask) {
        return;
    }
    if (changes & (scTaskbar | scDisplay | scINIFile)) {
        Taskbar_Configure(hwndTask);
    }
    if (HOOKS_OK() && (changes & scDesktop)) {
        g_Hooks.SetRCloseEnabled(g_RightClose);
        g_Hooks.SetRButtonUpClose(g_RButtonUpClose);
    }
    if (HOOKS_OK() && (changes & scTaskbar)) {
        if (g_Win95Keyboard) {
            TASKSTATE FAR *ps = GET_TSTATE(hwndTask);
            if (ps) {
                g_Hooks.StartKeyboardHook(hwndTask);
            }
        } else {
            g_Hooks.StopKeyboardHook();
        }
    }
}

void Taskbar_ShowTaskManager(HWND hwndTask)
{
    TASKSTATE FAR *ps;
    if (!hwndTask) { WinExec("taskman.exe", SW_SHOW); return; }
    ps = GET_TSTATE(hwndTask);
    TaskMan_ShowDialog(ps ? ps->hInst : g_hInst, hwndTask);
}

void Taskbar_ShowStartProp(HWND hwndTask)
{
    TASKSTATE FAR *ps;
    if (!hwndTask) {
        return;
    }
    ps = GET_TSTATE(hwndTask);
    if (ps) {
        StrtProp_ShowDialog(ps->hInst, hwndTask);
    }
}

/* Called by WM_SHELLWNDCREATE and the initial EnumWindows seed */
void Taskbar_AddWindow(HWND hwndTask, HWND hwndApp)
{
    TASKSTATE FAR *ps = GET_TSTATE(hwndTask);
    char cls[64];
    int  idx;

    if (!ps || !IsWindow(hwndApp) || !IsWindowVisible(hwndApp)) {
        return;
    }
    /* Don't add our own windows */
    GetClassName(hwndApp, cls, sizeof(cls));
    if (_fmemcmp(cls, "Calmira", 7) == 0) {
        return;
    }
    /* Only top-level visible windows with captions */
    if (GetParent(hwndApp)) {
        return;
    }
    if (!(GetWindowLong(hwndApp, GWL_STYLE) & WS_CAPTION)) {
        return;
    }

    /* Already listed? */
    if (FindButton(ps, hwndApp) >= 0) {
        return;
    }
    if (ps->nBtns >= MAX_TASKS) {
        return;
    }

    idx = ps->nBtns++;
    ps->aBtns[idx].hwnd    = hwndApp;
    ps->aBtns[idx].bActive = (hwndApp == GetActiveWindow());
    ps->aBtns[idx].hIcon   = (HICON)GetClassWord(hwndApp, GCW_HICONSM);
    if (!ps->aBtns[idx].hIcon) {
        ps->aBtns[idx].hIcon = (HICON)GetClassWord(hwndApp, GCW_HICON);
    }
    BuildButtonCaption(hwndApp, ps->aBtns[idx].szCaption,
                       sizeof(ps->aBtns[idx].szCaption));
    InvalidateRect(hwndTask, NULL, FALSE);
}

void Taskbar_RemoveWindow(HWND hwndTask, HWND hwndApp)
{
    TASKSTATE FAR *ps = GET_TSTATE(hwndTask);
    int idx;
    if (!ps) {
        return;
    }
    idx = FindButton(ps, hwndApp);
    if (idx < 0) {
        return;
    }
    /* Shift remaining buttons down */
    _fmemmove(&ps->aBtns[idx], &ps->aBtns[idx+1],
              (ps->nBtns - idx - 1) * sizeof(TASKBTN));
    ps->nBtns--;
    InvalidateRect(hwndTask, NULL, FALSE);
}

void Taskbar_ActivateWindow(HWND hwndTask, HWND hwndApp, BOOL bActive)
{
    TASKSTATE FAR *ps = GET_TSTATE(hwndTask);
    int i;
    if (!ps) {
        return;
    }
    for (i = 0; i < ps->nBtns; i++) {
        BOOL was = ps->aBtns[i].bActive;
        ps->aBtns[i].bActive = (ps->aBtns[i].hwnd == hwndApp) && bActive;
        if (ps->aBtns[i].bActive != was) {
            InvalidateRect(hwndTask, NULL, FALSE);
        }
    }
}
