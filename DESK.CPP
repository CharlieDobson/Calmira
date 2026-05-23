/***************************************************************************
 *
 *  DESK.CPP - Desktop window
 *  Translated from DESK.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  The Desktop is a hidden top-level window that owns all shortcut icon
 *  windows and acts as the receive point for WNDHOOKS.DLL callbacks
 *  (WM_DESKMENU, WM_DESKACTIVATE).  It also handles window management
 *  operations (cascade, arrange, clear).
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include "DESK.H"
#include "SHORTS.H"
#include "COMPSYS.H"
#include "HELPERS.H"
#include "SETTINGS.H"
#include "RESOURCE.H"
#include "HOOKS.H"
#include "TASK.H"
#include "SHUTDWN.H"
#include "RUNPROG.H"
#include "ABOUT.H"
#include "DESKPROP.H"
#include "..\CALMSGS.H"

/* Per-window data stored in the DESK window's extra bytes */
#define DESK_EXTRA  sizeof(DWORD)   /* pointer to heap-allocated state */

typedef struct tagDESKSTATE {
    HINSTANCE hInst;
    HWND      aShorts[MAX_SHORTS];
    int       nShorts;
    BOOL      bAllMinimized;        /* toggle state for ToggleAll */
} DESKSTATE;

#define GET_STATE(hwnd)  ((DESKSTATE FAR *)GetWindowLong(hwnd, 0))

/* -----------------------------------------------------------------------
 * Cascade all non-iconic top-level windows (excluding Calmira's own)
 * ----------------------------------------------------------------------- */
BOOL CALLBACK __export CascadeEnumProc(HWND hwnd, LPARAM lParam)
{
    char cls[64];
    if (!IsWindowVisible(hwnd) || IsIconic(hwnd)) {
        return TRUE;
    }
    GetClassName(hwnd, cls, sizeof(cls));
    /* Skip Calmira's own windows */
    if (_fmemcmp(cls, "Calmira", 7) == 0) {
        return TRUE;
    }
    /* Skip desktop */
    if (GetDesktopWindow() == hwnd) {
        return TRUE;
    }

    {
        /* lParam packs (offset_x << 16 | offset_y) in the low DWORDs;
           but since LPARAM is 32-bit we use a pointer to a struct. */
        typedef struct { RECT rc; int ox; int oy; } CASC;
        CASC FAR *pc = (CASC FAR *)lParam;
        SetWindowPos(hwnd, NULL,
                     pc->rc.left + pc->ox, pc->rc.top + pc->oy,
                     (pc->rc.right - pc->rc.left) - 40,
                     (pc->rc.bottom - pc->rc.top) - 40,
                     SWP_NOZORDER | SWP_NOACTIVATE);
        pc->ox += 20; pc->oy += 20;
        if (pc->ox > 160) { pc->ox = 0; pc->oy = 0; }
    }
    return TRUE;
}

/* -----------------------------------------------------------------------
 * Arrange minimized application icons along the bottom
 * ----------------------------------------------------------------------- */
typedef struct { int x; int y; } ARRGSTATE;

BOOL CALLBACK __export ArrangeEnumProc(HWND hwnd, LPARAM lParam)
{
    char cls[64];
    ARRGSTATE FAR *ps = (ARRGSTATE FAR *)lParam;
    if (!IsWindowVisible(hwnd) || !IsIconic(hwnd)) {
        return TRUE;
    }
    GetClassName(hwnd, cls, sizeof(cls));
    if (_fmemcmp(cls, "Calmira", 7) == 0) {
        return TRUE;
    }
    SetWindowPos(hwnd, NULL, ps->x, ps->y, 0, 0,
                 SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    ps->x += GetSystemMetrics(SM_CXICONSPACING);
    if (ps->x > GetSystemMetrics(SM_CXSCREEN) - GetSystemMetrics(SM_CXICONSPACING)) {
        ps->x  = 0;
        ps->y -= GetSystemMetrics(SM_CYICONSPACING);
    }
    return TRUE;
}

/* -----------------------------------------------------------------------
 * Close all folder browser windows (Computer windows)
 * ----------------------------------------------------------------------- */
BOOL CALLBACK __export CloseEnumProc(HWND hwnd, LPARAM lParam)
{
    char cls[64];
    GetClassName(hwnd, cls, sizeof(cls));
    if (lstrcmp(cls, COMPUTER_CLASS) == 0) {
        PostMessage(hwnd, WM_CLOSE, 0, 0L);
    }
    return TRUE;
}

/* -----------------------------------------------------------------------
 * Minimize all shortcuts (ClearDesktop / ToggleAll)
 * ----------------------------------------------------------------------- */
static void MinimizeShortcuts(DESKSTATE FAR *ps, BOOL bMinimize)
{
    int i;
    for (i = 0; i < ps->nShorts; i++) {
        if (IsWindow(ps->aShorts[i])) {
            if (bMinimize) {
                ShowWindow(ps->aShorts[i], SW_MINIMIZE);
            }
            else {
                ShowWindow(ps->aShorts[i], SW_RESTORE);
            }
        }
    }
}

/* -----------------------------------------------------------------------
 * Desktop window procedure
 * ----------------------------------------------------------------------- */
LRESULT CALLBACK __export DeskWndProc(HWND hwnd, UINT msg,
                                       WPARAM wParam, LPARAM lParam)
{
    DESKSTATE FAR *ps = GET_STATE(hwnd);

    switch (msg) {
        case WM_CREATE: {
            DESKSTATE FAR *pNew =
                (DESKSTATE FAR *)GlobalAllocPtr(GHND, sizeof(DESKSTATE));
            if (!pNew) {
                return -1;
            }
            pNew->hInst = ((LPCREATESTRUCT)lParam)->hInstance;
            pNew->nShorts = 0;
            pNew->bAllMinimized = FALSE;
            SetWindowLong(hwnd, 0, (LONG)pNew);
            return 0;
        }

        case WM_DESTROY: {
            if (ps) {
                GlobalFreePtr(ps);
                SetWindowLong(hwnd, 0, 0L);
            }
            PostQuitMessage(0);   /* Desktop window owns app lifetime */
            return 0;
        }

        case WM_DESKMENU: {
            if (g_ShowDeskMenu) {
                HMENU hMenu = LoadMenu(ps->hInst,
                                       MAKEINTRESOURCE(IDR_DESKMENU));
                if (hMenu) {
                    HMENU hPop = GetSubMenu(hMenu, 0);
                    POINT pt;
                    pt.x = LOWORD(lParam);
                    pt.y = HIWORD(lParam);
                    TrackPopupMenu(hPop,
                        TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                        pt.x, pt.y, 0, hwnd, NULL);
                    DestroyMenu(hMenu);
                    /* Win16: TrackPopupMenu posts WM_COMMAND automatically */
                }
            }
            return 0;
        }

        case WM_DESKACTIVATE: {
            /* Right-click while a modifier is held - same as WM_DESKMENU
               for our purposes */
            SendMessage(hwnd, WM_DESKMENU, wParam, lParam);
            return 0;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDM_CASCADE: {
                    Desktop_CascadeBrowsers(hwnd);
                    return 0;
                }
                case IDM_ARRANGE: {
                    Desktop_ArrangeTaskIcons(hwnd);
                    return 0;
                }
                case IDM_LINEUP: {
                    Desktop_LineUpIcons(hwnd);
                    return 0;
                }
                case IDM_CLOSEALL: {
                    Desktop_CloseBrowsers(hwnd);
                    return 0;
                }
                case IDM_CLEAR: {
                    Desktop_ClearDesktop(hwnd);
                    return 0;
                }
                case IDM_DESKPROP: {
                    DeskProp_ShowDialog(ps->hInst, hwnd);
                    return 0;
                }
                case IDM_ABOUT: {
                    About_ShowDialog(ps->hInst, hwnd);
                    return 0;
                }
            }
            return 0;
        }

        case WM_SETTINGSCHANGED: {
            if ((TSettingChanges)wParam & scDesktop) {
                /* Re-apply desktop hook settings */
                if (HOOKS_OK()) {
                    g_Hooks.SetRCloseEnabled(g_RightClose);
                    g_Hooks.SetRButtonUpClose(g_RButtonUpClose);
                }
            }
            return 0;
        }

        case WM_QUERYENDSESSION: {
            return 1;   /* allow the session to end */
        }

        case WM_ENDSESSION: {
            if (wParam) {
                /* Windows is shutting down; save state and exit cleanly. */
                Desktop_Save(hwnd);
                Settings_Done();
                PostQuitMessage(0);
            }
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/* -----------------------------------------------------------------------
 * Public API
 * ----------------------------------------------------------------------- */

BOOL Desktop_RegisterClass(HINSTANCE hInst)
{
    WNDCLASS wc;
    Short_RegisterClass(hInst);  /* also register shortcut class */
    wc.style         = 0;
    wc.lpfnWndProc   = DeskWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = DESK_EXTRA;
    wc.hInstance     = hInst;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = DESK_CLASS;
    return RegisterClass(&wc) != 0;
}

HWND Desktop_Create(HINSTANCE hInst, HWND hwndParent)
{
    HWND hwnd = CreateWindow(DESK_CLASS, "",
                             WS_OVERLAPPED,
                             -9999, -9999, 1, 1,
                             hwndParent, NULL, hInst, NULL);
    if (hwnd && HOOKS_OK()) {
        g_Hooks.SetDesktopHook(hwnd);
        g_Hooks.SetCallBackWnd(hwnd);
        g_Hooks.SetRCloseEnabled(g_RightClose);
        g_Hooks.SetRButtonUpClose(g_RButtonUpClose);
    }
    return hwnd;
}

void Desktop_Load(HWND hwndDesk)
{
    DESKSTATE FAR *ps = GET_STATE(hwndDesk);
    int i, n;
    char key[20];
    SHORTINFO si;

    if (!ps) {
        return;
    }
    n = Profile_GetInt(&g_Ini, "Desktop", "NumShorts", 0);
    ps->nShorts = 0;

    for (i = 0; i < n && ps->nShorts < MAX_SHORTS; i++) {
        wsprintf(key, "Shortcut%d", i);
        si.Kind = Profile_GetInt(&g_Ini, key, "Kind", 0);
        if (!si.Kind) {
            continue;
        }
        Profile_GetStr(&g_Ini, key, "Target",       "", si.Target, sizeof(si.Target));
        Profile_GetStr(&g_Ini, key, "Caption",      "", si.Caption, sizeof(si.Caption));
        Profile_GetStr(&g_Ini, key, "IconFile",     "", si.IconFile, sizeof(si.IconFile));
        si.IconIndex = Profile_GetInt(&g_Ini, key, "IconIndex", 0);
        Profile_GetStr(&g_Ini, key, "Params",       "", si.Params, sizeof(si.Params));
        Profile_GetStr(&g_Ini, key, "WorkingFolder","", si.WorkDir, sizeof(si.WorkDir));
        si.ShowMode = Profile_GetInt(&g_Ini, key, "ShowMode", 0);
        si.Left     = Profile_GetInt(&g_Ini, key, "Left",  20 + i * 80);
        si.Top      = Profile_GetInt(&g_Ini, key, "Top",   20);

        {
            HWND hwndShort = Short_Create(ps->hInst, hwndDesk, &si);
            if (hwndShort) {
                ps->aShorts[ps->nShorts++] = hwndShort;
                ShowWindow(hwndShort, SW_SHOW);
            }
        }
    }
}

void Desktop_Save(HWND hwndDesk)
{
    DESKSTATE FAR *ps = GET_STATE(hwndDesk);
    int i;
    char key[20];
    SHORTINFO si;

    if (!ps) {
        return;
    }
    Profile_SetInt(&g_Ini, "Desktop", "NumShorts", ps->nShorts);
    for (i = 0; i < ps->nShorts; i++) {
        if (!IsWindow(ps->aShorts[i])) {
            continue;
        }
        wsprintf(key, "Shortcut%d", i);
        Short_GetInfo(ps->aShorts[i], &si);
        {
            RECT rc;
            GetWindowRect(ps->aShorts[i], &rc);
            si.Left = rc.left;
            si.Top  = rc.top;
        }
        Profile_SetInt(&g_Ini, key, "Kind",           si.Kind);
        Profile_SetStr(&g_Ini, key, "Target",         si.Target);
        Profile_SetStr(&g_Ini, key, "Caption",        si.Caption);
        Profile_SetStr(&g_Ini, key, "IconFile",       si.IconFile);
        Profile_SetInt(&g_Ini, key, "IconIndex",      si.IconIndex);
        Profile_SetStr(&g_Ini, key, "Params",         si.Params);
        Profile_SetStr(&g_Ini, key, "WorkingFolder",  si.WorkDir);
        Profile_SetInt(&g_Ini, key, "ShowMode",       si.ShowMode);
        Profile_SetInt(&g_Ini, key, "Left",           si.Left);
        Profile_SetInt(&g_Ini, key, "Top",            si.Top);
    }
}

void Desktop_CascadeBrowsers(HWND hwndDesk)
{
    typedef struct { RECT rc; int ox; int oy; } CASC;
    CASC casc;
    int  taskH = g_DisableTaskbar ? 0 : TASKBAR_HEIGHT;
    casc.rc.left   = 0;
    casc.rc.top    = g_TopTaskbar ? taskH : 0;
    casc.rc.right  = GetSystemMetrics(SM_CXSCREEN);
    casc.rc.bottom = GetSystemMetrics(SM_CYSCREEN) - (g_TopTaskbar ? 0 : taskH);
    casc.ox = 0;
    casc.oy = 0;
    EnumWindows((WNDENUMPROC)CascadeEnumProc, (LPARAM)(void FAR *)&casc);
}

void Desktop_ArrangeTaskIcons(HWND hwndDesk)
{
    ARRGSTATE as;
    int taskH = g_DisableTaskbar ? 0 : TASKBAR_HEIGHT;
    as.x = 0;
    as.y = GetSystemMetrics(SM_CYSCREEN) - (g_TopTaskbar ? 0 : taskH)
           - GetSystemMetrics(SM_CYICONSPACING);
    EnumWindows((WNDENUMPROC)ArrangeEnumProc, (LPARAM)(void FAR *)&as);
}

void Desktop_LineUpIcons(HWND hwndDesk)
{
    DESKSTATE FAR *ps = GET_STATE(hwndDesk);
    int i, col = 0, row = 0;
    int cx = g_DeskGrid.x;
    int cy = g_DeskGrid.y;
    int cols = GetSystemMetrics(SM_CXSCREEN) / cx;
    if (!ps) {
        return;
    }
    if (cols < 1) {
        cols = 1;
    }
    for (i = 0; i < ps->nShorts; i++) {
        if (!IsWindow(ps->aShorts[i])) {
            continue;
        }
        SetWindowPos(ps->aShorts[i], NULL,
                     col * cx, row * cy, 0, 0,
                     SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
        col++;
        if (col >= cols) { col = 0; row++; }
    }
}

void Desktop_CloseBrowsers(HWND hwndDesk)
{
    EnumWindows((WNDENUMPROC)CloseEnumProc, 0L);
}

void Desktop_ClearDesktop(HWND hwndDesk)
{
    DESKSTATE FAR *ps = GET_STATE(hwndDesk);
    if (ps) {
        MinimizeShortcuts(ps, TRUE);
    }
}

void Desktop_ToggleAll(HWND hwndDesk)
{
    DESKSTATE FAR *ps = GET_STATE(hwndDesk);
    if (!ps) {
        return;
    }
    ps->bAllMinimized = !ps->bAllMinimized;
    MinimizeShortcuts(ps, ps->bAllMinimized);
}

void Desktop_RefreshPath(HWND hwndDesk, LPCSTR szPath)
{
    /* Broadcast refresh to all Computer windows showing this path */
    HWND hw = GetTopWindow(NULL);
    while (hw) {
        char cls[64];
        GetClassName(hw, cls, sizeof(cls));
        if (lstrcmp(cls, COMPUTER_CLASS) == 0) {
            char cur[144];
            Computer_GetPath(hw, cur, sizeof(cur));
            AnsiLower(cur);
            {
                char lpath[144];
                lstrcpy(lpath, szPath);
                AnsiLower(lpath);
                if (lstrcmp(cur, lpath) == 0) {
                    Computer_Refresh(hw);
                }
            }
        }
        hw = GetNextWindow(hw, GW_HWNDNEXT);
    }
}

void Desktop_OnSettingsChanged(HWND hwndDesk, TSettingChanges changes)
{
    if (HOOKS_OK() && (changes & scDesktop)) {
        g_Hooks.SetRCloseEnabled(g_RightClose);
        g_Hooks.SetRButtonUpClose(g_RButtonUpClose);
    }
}

HWND Desktop_AddShortcut(HWND hwndDesk, const SHORTINFO FAR *psi)
{
    DESKSTATE FAR *ps = GET_STATE(hwndDesk);
    HWND hwndShort;
    if (!ps || ps->nShorts >= MAX_SHORTS) {
        return 0;
    }
    hwndShort = Short_Create(ps->hInst, hwndDesk, psi);
    if (hwndShort) {
        ps->aShorts[ps->nShorts++] = hwndShort;
        ShowWindow(hwndShort, SW_SHOW);
    }
    return hwndShort;
}

