/***************************************************************************
 *
 *  ICONWIN.CPP - Icon window (file view pane)
 *  Translated from ICONWIN.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include "ICONWIN.H"
#include "ICONIC.H"
#include "SETTINGS.H"
#include "RESOURCE.H"
#include "DIRECTRY.H"
#include "FILEMAN.H"
#include "HELPERS.H"
#include "..\CALMSGS.H"

#define IW_EXTRA  sizeof(LONG)
#define GET_IWD(hwnd)  ((ICONWINDATA FAR *)GetWindowLong(hwnd, 0))

/* -----------------------------------------------------------------------
 * Load the current directory into aItems
 * ----------------------------------------------------------------------- */
static void LoadDirectory(ICONWINDATA FAR *piwd)
{
    int n;
    Directory_Enum(piwd->szPath, piwd->szFilter, g_ShowHidSys,
                   piwd->aItems, MAX_FILES, &n, piwd->sortOrder);
    piwd->nItems   = n;
    piwd->nScrollX = 0;
    piwd->nScrollY = 0;
    piwd->nSelected = -1;
    if (g_UseDescriptions) {
        Directory_LoadDescs(piwd->szPath, piwd->aItems, n);
    }
}

/* -----------------------------------------------------------------------
 * Icon window WndProc
 * ----------------------------------------------------------------------- */
LRESULT CALLBACK __export IconWinWndProc(HWND hwnd, UINT msg,
                                          WPARAM wParam, LPARAM lParam)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwnd);

    switch (msg) {
        case WM_CREATE: {
            ICONWINDATA FAR *p =
                (ICONWINDATA FAR *)GlobalAllocPtr(GHND, sizeof(ICONWINDATA));
            if (!p) {
                return -1;
            }
            p->mode      = g_DefaultDisplay;
            p->sortOrder = g_DefaultSort;
            lstrcpy(p->szFilter, g_DefaultFilter);
            lstrcpy(p->szPath, "C:\\");
            Iconic_CalcColWidths(&p->colWidths);
            SetWindowLong(hwnd, 0, (LONG)p);
            LoadDirectory(p);
            return 0;
        }

        case WM_DESTROY: {
            if (piwd) {
                int i;
                for (i = 0; i < piwd->nItems; i++) {
                    if (piwd->aItems[i].hIcon) {
                        Iconic_ReleaseIcon(piwd->aItems[i].hIcon);
                    }
                }
                GlobalFreePtr(piwd);
                SetWindowLong(hwnd, 0, 0L);
            }
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC  hdc = BeginPaint(hwnd, &ps);
            RECT client;
            int  i;
            GetClientRect(hwnd, &client);
            FillRect(hdc, &client, GetSysColorBrush(COLOR_WINDOW));
            if (piwd) {
                HFONT hFont = CreateFontIndirect(&g_GlobalFont);
                HFONT hOld  = (HFONT)SelectObject(hdc, hFont);
                int   cx = 0, cy = 0;

                for (i = 0; i < piwd->nItems; i++) {
                    int px, py;
                    if (piwd->mode == dmLargeIcons) {
                        px = cx * LARGE_ICON_W - piwd->nScrollX;
                        py = cy * LARGE_ICON_H - piwd->nScrollY;
                        cx++;
                        if ((cx + 1) * LARGE_ICON_W > client.right) {
                            cx = 0; cy++;
                        }
                    } else if (piwd->mode == dmSmallIcons) {
                        px = cx * g_BrowseGrid.x - piwd->nScrollX;
                        py = cy * g_BrowseGrid.y - piwd->nScrollY;
                        cx++;
                        if ((cx + 1) * g_BrowseGrid.x > client.right) {
                            cx = 0; cy++;
                        }
                    } else {
                        px = 0;
                        py = i * DETAILS_H - piwd->nScrollY;
                    }
                    if (py + LARGE_ICON_H < 0) {
                        continue;
                    }
                    if (py > client.bottom) {
                        break;
                    }
                    Iconic_DrawItem(hdc, &piwd->aItems[i], px, py,
                                    piwd->mode, TRUE,
                                    (HINSTANCE)GetWindowWord(hwnd, GWW_HINSTANCE));
                }
                SelectObject(hdc, hOld);
                DeleteObject(hFont);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDBLCLK: {
            if (!piwd || piwd->nSelected < 0) {
                return 0;
            }
            {
                ICONITEM FAR *pi = &piwd->aItems[piwd->nSelected];
                char fullPath[144];
                Directory_JoinPath(piwd->szPath, pi->szName,
                                   fullPath, sizeof(fullPath));
                if (pi->bType & ITYPE_DIR) {
                    /* Navigate into folder */
                    IconWin_SetPath(hwnd, fullPath);
                    PostMessage(GetParent(hwnd), IWM_PATHCHANGE, 0, 0L);
                } else {
                    FileMan_DefaultExec(hwnd, fullPath, NULL, NULL, SW_SHOWNORMAL);
                }
            }
            return 0;
        }

        case WM_VSCROLL: {
            if (!piwd) {
                return 0;
            }
            switch (LOWORD(wParam)) {
                case SB_LINEUP:   piwd->nScrollY = max(0, piwd->nScrollY - DETAILS_H); break;
                case SB_LINEDOWN: piwd->nScrollY += DETAILS_H; break;
                case SB_THUMBPOSITION: piwd->nScrollY = HIWORD(wParam); break;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }

        case WM_SETTINGSCHANGED: {
            if (piwd) {
                LoadDirectory(piwd);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL IconWin_RegisterClass(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style         = CS_DBLCLKS;
    wc.lpfnWndProc   = IconWinWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = IW_EXTRA;
    wc.hInstance     = hInst;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = ICONWIN_CLASS;
    return RegisterClass(&wc) != 0;
}

HWND IconWin_Create(HINSTANCE hInst, HWND hwndParent, LPRECT prc)
{
    return CreateWindow(ICONWIN_CLASS, "",
                        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER,
                        prc->left, prc->top,
                        prc->right - prc->left,
                        prc->bottom - prc->top,
                        hwndParent, (HMENU)IDW_ICONVIEW, hInst, NULL);
}

void IconWin_SetPath(HWND hwndIW, LPCSTR szPath)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    if (!piwd) {
        return;
    }
    lstrcpyn(piwd->szPath, szPath, sizeof(piwd->szPath));
    {
        int len = lstrlen(piwd->szPath);
        if (len > 0 && piwd->szPath[len-1] != '\\') {
            piwd->szPath[len]   = '\\';
            piwd->szPath[len+1] = '\0';
        }
    }
    LoadDirectory(piwd);
    InvalidateRect(hwndIW, NULL, TRUE);
    PostMessage(GetParent(hwndIW), IWM_PATHCHANGE, 0, 0L);
}

void IconWin_GetPath(HWND hwndIW, LPSTR buf, int bufLen)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    if (piwd) {
        lstrcpyn(buf, piwd->szPath, bufLen);
    }
    else {
        buf[0] = '.';
    }
}

void IconWin_SetMode(HWND hwndIW, TDisplayMode mode)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    if (piwd) { piwd->mode = mode; InvalidateRect(hwndIW, NULL, TRUE); }
}

void IconWin_SetSort(HWND hwndIW, TSortOrder order)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    if (!piwd) {
        return;
    }
    piwd->sortOrder = order;
    LoadDirectory(piwd);
    InvalidateRect(hwndIW, NULL, TRUE);
}

void IconWin_SetFilter(HWND hwndIW, LPCSTR szMask)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    if (!piwd) {
        return;
    }
    lstrcpyn(piwd->szFilter, szMask, sizeof(piwd->szFilter));
    LoadDirectory(piwd);
    InvalidateRect(hwndIW, NULL, TRUE);
}

void IconWin_Refresh(HWND hwndIW)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    if (piwd) { LoadDirectory(piwd); InvalidateRect(hwndIW, NULL, TRUE); }
}

void IconWin_SelectAll(HWND hwndIW)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    int i;
    if (!piwd) {
        return;
    }
    for (i = 0; i < piwd->nItems; i++) piwd->aItems[i].bSelected = TRUE;
    InvalidateRect(hwndIW, NULL, FALSE);
}

void IconWin_InvertSel(HWND hwndIW)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    int i;
    if (!piwd) {
        return;
    }
    for (i = 0; i < piwd->nItems; i++)
        piwd->aItems[i].bSelected = !piwd->aItems[i].bSelected;
    InvalidateRect(hwndIW, NULL, FALSE);
}

int IconWin_GetSelCount(HWND hwndIW)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    int i, n = 0;
    if (!piwd) {
        return 0;
    }
    for (i = 0; i < piwd->nItems; i++)
        if (piwd->aItems[i].bSelected) {
            n++;
        }
    return n;
}

int IconWin_GetSelPaths(HWND hwndIW, LPSTR buf, int bufLen)
{
    ICONWINDATA FAR *piwd = GET_IWD(hwndIW);
    LPSTR p = buf;
    int   i, n = 0, rem = bufLen;
    if (!piwd || !buf || bufLen < 2) {
        return 0;
    }
    for (i = 0; i < piwd->nItems && rem > 1; i++) {
        if (!piwd->aItems[i].bSelected) {
            continue;
        }
        {
            char full[144];
            int  flen;
            Directory_JoinPath(piwd->szPath, piwd->aItems[i].szName,
                               full, sizeof(full));
            flen = lstrlen(full) + 1;
            if (flen >= rem) {
                break;
            }
            lstrcpy(p, full);
            p   += flen;
            rem -= flen;
            n++;
        }
    }
    *p = '\0';  /* double-NUL terminator */
    return n;
}

void IconWin_SetShowHidden(HWND hwndIW, BOOL bShow)
{
    g_ShowHidSys = bShow;
    IconWin_Refresh(hwndIW);
}

void IconWin_CalcColWidths(void)
{
    /* Global recalculation - called after font change */
}
