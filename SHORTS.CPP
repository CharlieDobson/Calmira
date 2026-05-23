/***************************************************************************
 *
 *  SHORTS.CPP - Desktop shortcut icon windows
 *  Translated from SHORTS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Each desktop shortcut is a small top-level window (48x64 pixels)
 *  showing a 32x32 icon and a caption.  It can be dragged to any
 *  position on the desktop.  Double-click (or single if OneClick)
 *  activates the target.
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include "SHORTS.H"
#include "DESK.H"
#include "DIRECTRY.H"
#include "HELPERS.H"
#include "ICONIC.H"
#include "SETTINGS.H"
#include "RESOURCE.H"
#include "LOCALE.H"
#include "FILEMAN.H"
#include "ALIAS.H"

#define SHORT_W   64    /* icon window width  */
#define SHORT_H   72    /* icon window height */

/* Per-shortcut state stored in extra bytes */
#define SHORT_EXTRA  sizeof(LONG)

typedef struct tagSHORTSTATE {
    SHORTINFO si;
    HICON     hIcon;
    BOOL      bDragging;
    int       nDragOffX, nDragOffY;
} SHORTSTATE;

#define GET_SS(hwnd)  ((SHORTSTATE FAR *)GetWindowLong(hwnd, 0))

LRESULT CALLBACK __export ShortWndProc(HWND hwnd, UINT msg,
                                        WPARAM wParam, LPARAM lParam)
{
    SHORTSTATE FAR *pss = GET_SS(hwnd);

    switch (msg) {
        case WM_CREATE: {
            SHORTSTATE FAR *p =
                (SHORTSTATE FAR *)GlobalAllocPtr(GHND, sizeof(SHORTSTATE));
            if (!p) {
                return -1;
            }
            SetWindowLong(hwnd, 0, (LONG)p);
            return 0;
        }

        case WM_DESTROY: {
            if (pss) {
                if (pss->hIcon) {
                    Iconic_ReleaseIcon(pss->hIcon);
                }
                GlobalFreePtr(pss);
                SetWindowLong(hwnd, 0, 0L);
            }
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rc;
            GetClientRect(hwnd, &rc);

            /* Background */
            FillRect(hdc, &rc, (HBRUSH)GetStockObject(NULL_BRUSH));

            /* Icon */
            if (pss && pss->hIcon) {
                DrawIcon(hdc, (SHORT_W - 32) / 2, 2, pss->hIcon);
            }

            /* Caption */
            if (pss) {
                RECT rcText;
                rcText.left   = 0;
                rcText.top    = 36;
                rcText.right  = SHORT_W;
                rcText.bottom = SHORT_H;
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                DrawText(hdc, pss->si.Caption, -1, &rcText,
                         DT_CENTER | DT_WORDBREAK | DT_NOPREFIX);
            }
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDBLCLK: {
            Short_Activate(hwnd);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            if (g_OneClickShorts) {
                Short_Activate(hwnd);
            } else if (!g_StickyShorts) {
                /* Begin drag */
                SetCapture(hwnd);
                if (pss) {
                    pss->bDragging  = TRUE;
                    pss->nDragOffX  = LOWORD(lParam);
                    pss->nDragOffY  = HIWORD(lParam);
                }
            }
            return 0;
        }

        case WM_MOUSEMOVE: {
            if (pss && pss->bDragging && (wParam & MK_LBUTTON)) {
                POINT pt;
                RECT  rc2;
                pt.x = LOWORD(lParam);
                pt.y = HIWORD(lParam);
                ClientToScreen(hwnd, &pt);
                GetWindowRect(hwnd, &rc2);
                SetWindowPos(hwnd, NULL,
                             pt.x - pss->nDragOffX,
                             pt.y - pss->nDragOffY,
                             0, 0,
                             SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            }
            return 0;
        }

        case WM_LBUTTONUP: {
            if (pss && pss->bDragging) {
                ReleaseCapture();
                pss->bDragging = FALSE;
            }
            return 0;
        }

        case WM_RBUTTONDOWN: {
            HMENU hMenu = LoadMenu(
                (HINSTANCE)GetWindowWord(hwnd, GWW_HINSTANCE),
                MAKEINTRESOURCE(IDR_ICONMENU));
            if (hMenu) {
                HMENU hPop = GetSubMenu(hMenu, 0);
                POINT pt;
                pt.x = LOWORD(lParam); pt.y = HIWORD(lParam);
                ClientToScreen(hwnd, &pt);
                TrackPopupMenu(hPop,
                    TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                    pt.x, pt.y, 0, hwnd, NULL);
                DestroyMenu(hMenu);
            }
            return 0;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDM_OBJ_OPEN: {
                    Short_Activate(hwnd);
                    return 0;
                }
                case IDM_OBJ_DELETE: {
                    if (pss && g_ConfirmDelShort) {
                        char msg[256], fmt[128];
                        LoadString((HINSTANCE)GetWindowWord(hwnd, GWW_HINSTANCE),
                                   SQueryDeleteShortcut, fmt, sizeof(fmt));
                        wsprintf(msg, fmt, pss->si.Caption);
                        if (MessageBox(hwnd, msg, "Calmira",
                                       MB_ICONQUESTION | MB_YESNO) != IDYES)
                            return 0;
                    }
                    DestroyWindow(hwnd);
                    return 0;
                }
                case IDM_OBJ_PROP: {
                    /* Show shortcut properties - placeholder */
                    return 0;
                }
            }
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL Short_RegisterClass(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style         = CS_DBLCLKS | CS_SAVEBITS;
    wc.lpfnWndProc   = ShortWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = SHORT_EXTRA;
    wc.hInstance     = hInst;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = SHORT_CLASS;
    return RegisterClass(&wc) != 0;
}

HWND Short_Create(HINSTANCE hInst, HWND hwndDesk,
                  const SHORTINFO FAR *psi)
{
    HWND hwnd = CreateWindow(SHORT_CLASS, psi->Caption,
                             WS_POPUP,
                             psi->Left, psi->Top, SHORT_W, SHORT_H,
                             hwndDesk, NULL, hInst, NULL);
    if (hwnd) {
        SHORTSTATE FAR *pss = GET_SS(hwnd);
        if (pss) {
            _fmemcpy(&pss->si, psi, sizeof(SHORTINFO));
            /* Extract icon */
            if (*psi->IconFile) {
                pss->hIcon = Iconic_GetIcon(psi->IconFile, FALSE);
            }
            else if (*psi->Target) {
                pss->hIcon = Iconic_GetIcon(psi->Target, FALSE);
            }
        }
    }
    return hwnd;
}

void Short_GetInfo(HWND hwndShort, SHORTINFO FAR *psi)
{
    SHORTSTATE FAR *pss = GET_SS(hwndShort);
    if (pss) {
        _fmemcpy(psi, &pss->si, sizeof(SHORTINFO));
    }
}

void Short_SetInfo(HWND hwndShort, const SHORTINFO FAR *psi)
{
    SHORTSTATE FAR *pss = GET_SS(hwndShort);
    if (!pss) {
        return;
    }
    if (pss->hIcon) {
        Iconic_ReleaseIcon(pss->hIcon);
    }
    _fmemcpy(&pss->si, psi, sizeof(SHORTINFO));
    if (*psi->IconFile) {
        pss->hIcon = Iconic_GetIcon(psi->IconFile, FALSE);
    }
    else if (*psi->Target) {
        pss->hIcon = Iconic_GetIcon(psi->Target, FALSE);
    }
    else {
        pss->hIcon = NULL;
    }
    SetWindowText(hwndShort, psi->Caption);
    InvalidateRect(hwndShort, NULL, TRUE);
}

void Short_Activate(HWND hwndShort)
{
    SHORTSTATE FAR *pss = GET_SS(hwndShort);
    if (!pss || !*pss->si.Target) {
        return;
    }

    /* Alias? */
    {
        LPCSTR ext = Directory_GetExt(pss->si.Target);
        if (lstrcmpi(ext, g_AliasExtension + 1) == 0) {
            Alias_Execute(hwndShort, pss->si.Target);
            return;
        }
    }

    FileMan_DefaultExec(hwndShort, pss->si.Target,
                        pss->si.Params[0] ? pss->si.Params : NULL,
                        pss->si.WorkDir[0] ? pss->si.WorkDir : NULL,
                        pss->si.ShowMode ? pss->si.ShowMode : SW_SHOWNORMAL);
}
