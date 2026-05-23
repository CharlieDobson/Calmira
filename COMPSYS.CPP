/***************************************************************************
 *
 *  COMPSYS.CPP - Computer / File Manager window
 *  Translated from COMPSYS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Layout:
 *    [Toolbar row] [Path combobox]
 *    [Tree pane | Splitter | Icon view pane]
 *    [Status bar]
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include "HELPERS.H"
#include "COMPSYS.H"
#include "TREE.H"
#include "ICONWIN.H"
#include "SETTINGS.H"
#include "RESOURCE.H"
#include "FILEMAN.H"
#include "DIRECTRY.H"
#include "FILEFIND.H"
#include "..\CALMSGS.H"

#define TOOLBAR_H  28
#define COMBO_H    22
#define STATUS_H   18
#define TREE_DEF_W 180   /* default tree width in pixels */
#define SPLITTER_W   4

typedef struct tagCOMPSTATE {
    HINSTANCE hInst;
    HWND      hwndTree;
    HWND      hwndIconWin;
    HWND      hwndCombo;
    HWND      hwndStatus;
    int       nTreeW;
    BOOL      bShowTree;
} COMPSTATE;

#define COMP_EXTRA  sizeof(LONG)
#define GET_CS(hwnd)  ((COMPSTATE FAR *)GetWindowLong(hwnd, 0))

/* -----------------------------------------------------------------------
 * Lay out child windows after a resize
 * ----------------------------------------------------------------------- */
static void LayoutChildren(HWND hwnd, COMPSTATE FAR *pcs)
{
    RECT client;
    int  y, h, treeW, viewX, viewW;
    GetClientRect(hwnd, &client);
    y = 0;

    /* Toolbar (simple bar - buttons drawn directly for now) */
    y += TOOLBAR_H;

    /* Path combobox */
    if (pcs->hwndCombo) {
        SetWindowPos(pcs->hwndCombo, NULL,
                     0, y, client.right, COMBO_H,
                     SWP_NOZORDER | SWP_NOACTIVATE);
        y += COMBO_H;
    }

    h = client.bottom - y - STATUS_H;
    if (h < 0) {
        h = 0;
    }

    treeW = pcs->bShowTree ? pcs->nTreeW : 0;
    viewX = treeW + (pcs->bShowTree ? SPLITTER_W : 0);
    viewW = client.right - viewX;
    if (viewW < 0) {
        viewW = 0;
    }

    if (pcs->hwndTree) {
        RECT rcTree;
        rcTree.left = 0; rcTree.top = y;
        rcTree.right = treeW; rcTree.bottom = y + h;
        SetWindowPos(pcs->hwndTree, NULL,
                     0, y, treeW, h,
                     SWP_NOZORDER | SWP_NOACTIVATE);
        ShowWindow(pcs->hwndTree, pcs->bShowTree ? SW_SHOW : SW_HIDE);
    }

    if (pcs->hwndIconWin) {
        SetWindowPos(pcs->hwndIconWin, NULL,
                     viewX, y, viewW, h,
                     SWP_NOZORDER | SWP_NOACTIVATE);
    }

    if (pcs->hwndStatus) {
        SetWindowPos(pcs->hwndStatus, NULL,
                     0, client.bottom - STATUS_H,
                     client.right, STATUS_H,
                     SWP_NOZORDER | SWP_NOACTIVATE);
    }
}

/* -----------------------------------------------------------------------
 * Computer window procedure
 * ----------------------------------------------------------------------- */
LRESULT CALLBACK __export CompWndProc(HWND hwnd, UINT msg,
                                       WPARAM wParam, LPARAM lParam)
{
    COMPSTATE FAR *pcs = GET_CS(hwnd);

    switch (msg) {
        case WM_CREATE: {
            COMPSTATE FAR *p =
                (COMPSTATE FAR *)GlobalAllocPtr(GHND, sizeof(COMPSTATE));
            RECT rc;
            if (!p) {
                return -1;
            }
            p->hInst     = ((LPCREATESTRUCT)lParam)->hInstance;
            p->nTreeW    = TREE_DEF_W;
            p->bShowTree = TRUE;
            SetWindowLong(hwnd, 0, (LONG)p);

            /* Create path combobox */
            p->hwndCombo = CreateWindow("COMBOBOX", "",
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_AUTOHSCROLL,
                0, TOOLBAR_H, 400, 200,
                hwnd, (HMENU)IDW_PATHCOMBO, p->hInst, NULL);

            /* Create tree pane */
            GetClientRect(hwnd, &rc);
            {
                RECT rcTree = { 0, TOOLBAR_H + COMBO_H, TREE_DEF_W,
                                rc.bottom - STATUS_H };
                p->hwndTree = Tree_Create(p->hInst, hwnd, &rcTree);
            }

            /* Create icon view */
            {
                RECT rcIW = { TREE_DEF_W + SPLITTER_W, TOOLBAR_H + COMBO_H,
                              rc.right, rc.bottom - STATUS_H };
                IconWin_RegisterClass(p->hInst);
                p->hwndIconWin = IconWin_Create(p->hInst, hwnd, &rcIW);
            }

            /* Create status bar */
            p->hwndStatus = CreateWindow("STATIC", "",
                WS_CHILD | WS_VISIBLE | SS_LEFT | SS_SUNKEN,
                0, rc.bottom - STATUS_H, rc.right, STATUS_H,
                hwnd, (HMENU)IDW_STATUSBAR, p->hInst, NULL);

            /* Load position */
            Computer_LoadPos(hwnd);
            return 0;
        }

        case WM_DESTROY: {
            Computer_SavePos(hwnd);
            if (pcs) {
                GlobalFreePtr(pcs);
                SetWindowLong(hwnd, 0, 0L);
            }
            return 0;
        }

        case WM_SIZE: {
            if (pcs) {
                LayoutChildren(hwnd, pcs);
            }
            return 0;
        }

        case WM_SETFOCUS: {
            if (pcs && pcs->hwndIconWin) {
                SetFocus(pcs->hwndIconWin);
            }
            return 0;
        }

        case TWM_NAVIGATE: {
            /* Tree sent us a new path */
            if (pcs && pcs->hwndIconWin) {
                IconWin_SetPath(pcs->hwndIconWin, (LPCSTR)lParam);
            }
            if (pcs && pcs->hwndCombo) {
                SetWindowText(pcs->hwndCombo, (LPCSTR)lParam);
            }
            return 0;
        }

        case IWM_PATHCHANGE: {
            /* Icon view navigated (e.g. by double-clicking a folder) */
            if (pcs) {
                char path[144];
                IconWin_GetPath(pcs->hwndIconWin, path, sizeof(path));
                if (pcs->hwndCombo) {
                    SetWindowText(pcs->hwndCombo, path);
                }
                if (pcs->hwndTree) {
                    Tree_SelectPath(pcs->hwndTree, path);
                }
            }
            return 0;
        }

        case WM_COMMAND: {
            if (!pcs) {
                return 0;
            }
            switch (LOWORD(wParam)) {
                case IDM_REFRESH: {
                    if (pcs->hwndIconWin) IconWin_Refresh(pcs->hwndIconWin);
                    return 0;
                }
                case IDM_VIEW_LARGE: {
                    if (pcs->hwndIconWin) IconWin_SetMode(pcs->hwndIconWin, dmLargeIcons);
                    return 0;
                }
                case IDM_VIEW_SMALL: {
                    if (pcs->hwndIconWin) IconWin_SetMode(pcs->hwndIconWin, dmSmallIcons);
                    return 0;
                }
                case IDM_VIEW_DETAILS: {
                    if (pcs->hwndIconWin) IconWin_SetMode(pcs->hwndIconWin, dmDetails);
                    return 0;
                }
                case IDM_SORT_NAME: {
                    if (pcs->hwndIconWin) IconWin_SetSort(pcs->hwndIconWin, soName);
                    return 0;
                }
                case IDM_SORT_TYPE: {
                    if (pcs->hwndIconWin) IconWin_SetSort(pcs->hwndIconWin, soType);
                    return 0;
                }
                case IDM_SORT_SIZE: {
                    if (pcs->hwndIconWin) IconWin_SetSort(pcs->hwndIconWin, soSize);
                    return 0;
                }
                case IDM_SORT_DATE: {
                    if (pcs->hwndIconWin) IconWin_SetSort(pcs->hwndIconWin, soDate);
                    return 0;
                }
                case IDM_SHOWHIDDEN: {
                    if (pcs->hwndIconWin)
                        IconWin_SetShowHidden(pcs->hwndIconWin, !g_ShowHidSys);
                    return 0;
                }
                case IDM_CLOSE: {
                    if (!g_SysWinQuit)
                        ShowWindow(hwnd, SW_HIDE);
                    else
                        PostQuitMessage(0);
                    return 0;
                }
            }
            return 0;
        }

        case WM_SETTINGSCHANGED: {
            if (pcs) {
                if (pcs->hwndIconWin) {
                    PostMessage(pcs->hwndIconWin, WM_SETTINGSCHANGED, wParam, 0L);
                }
            }
            return 0;
        }

        case WM_CLOSE: {
            if (g_SysWinQuit) {
                PostQuitMessage(0);
            }
            else {
                ShowWindow(hwnd, SW_HIDE);
            }
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL Computer_RegisterClass(HINSTANCE hInst)
{
    WNDCLASS wc;
    Tree_RegisterClass(hInst);
    wc.style         = CS_DBLCLKS | CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc   = CompWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = COMP_EXTRA;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_CALMIRA));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_WINMENU);
    wc.lpszClassName = COMPUTER_CLASS;
    return RegisterClass(&wc) != 0;
}

HWND Computer_Create(HINSTANCE hInst, HWND hwndParent)
{
    return CreateWindow(COMPUTER_CLASS, g_ComputerCaption,
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        CW_USEDEFAULT, CW_USEDEFAULT,
                        hwndParent, NULL, hInst, NULL);
}

void Computer_NavigateTo(HWND hwndComp, LPCSTR szPath)
{
    COMPSTATE FAR *pcs = GET_CS(hwndComp);
    char path[144];
    BOOL bExplorer = FALSE;

    if (!pcs) {
        return;
    }

    /* '*' prefix means explorer mode (show tree) */
    if (szPath[0] == '*') {
        bExplorer = TRUE;
        lstrcpy(path, szPath + 1);
    } else {
        lstrcpy(path, szPath);
    }

    if (!*path) {
        lstrcpy(path, "C:\\");
    }

    ShowWindow(hwndComp, SW_SHOWNORMAL);
    SetForegroundWindow(hwndComp);

    if (pcs->hwndIconWin) {
        IconWin_SetPath(pcs->hwndIconWin, path);
    }
    if (pcs->hwndCombo) {
        SetWindowText(pcs->hwndCombo, path);
    }
    if (pcs->hwndTree) {
        Tree_SelectPath(pcs->hwndTree, path);
    }
}

void Computer_Refresh(HWND hwndComp)
{
    COMPSTATE FAR *pcs = GET_CS(hwndComp);
    if (pcs && pcs->hwndIconWin) {
        IconWin_Refresh(pcs->hwndIconWin);
    }
}

void Computer_GetPath(HWND hwndComp, LPSTR buf, int bufLen)
{
    COMPSTATE FAR *pcs = GET_CS(hwndComp);
    if (pcs && pcs->hwndIconWin) {
        IconWin_GetPath(pcs->hwndIconWin, buf, bufLen);
    }
    else if (buf) {
        buf[0] = '\0';
    }
}

void Computer_SetPath(HWND hwndComp, LPCSTR szPath)
{
    Computer_NavigateTo(hwndComp, szPath);
}

void Computer_ShowNormal(HWND hwndComp)
{
    ShowWindow(hwndComp, SW_SHOWNORMAL);
    SetForegroundWindow(hwndComp);
}

void Computer_SavePos(HWND hwndComp)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);
    GetWindowPlacement(hwndComp, &wp);
    Profile_SetInt(&g_Ini, "Computer", "Left",   wp.rcNormalPosition.left);
    Profile_SetInt(&g_Ini, "Computer", "Top",    wp.rcNormalPosition.top);
    Profile_SetInt(&g_Ini, "Computer", "Width",  wp.rcNormalPosition.right  - wp.rcNormalPosition.left);
    Profile_SetInt(&g_Ini, "Computer", "Height", wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);
}

void Computer_LoadPos(HWND hwndComp)
{
    int l = Profile_GetInt(&g_Ini, "Computer", "Left",    117);
    int t = Profile_GetInt(&g_Ini, "Computer", "Top",      80);
    int w = Profile_GetInt(&g_Ini, "Computer", "Width",   321);
    int h = Profile_GetInt(&g_Ini, "Computer", "Height",  238);
    SetWindowPos(hwndComp, NULL, l, t, w, h,
                 SWP_NOZORDER | SWP_NOACTIVATE);
}
