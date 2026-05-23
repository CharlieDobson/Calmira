/***************************************************************************
 *
 *  TREE.CPP - Directory tree pane
 *  Translated from TREE.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include <dos.h>
#include <string.h>
#include "TREE.H"
#include "SETTINGS.H"
#include "RESOURCE.H"
#include "DIRECTRY.H"
#include "HELPERS.H"

#define INDENT_W    16   /* pixels per nesting level */
#define ROW_H       18   /* height of one tree row   */

typedef struct tagTREEDATA {
    TREENODE  aNodes[TREE_MAX_NODES];
    int       nNodes;
    int       nScroll;    /* first visible node index */
    HBITMAP   hbmPlus;
    HBITMAP   hbmMinus;
    HBITMAP   hbmFolder;
    HINSTANCE hInst;
} TREEDATA;

#define TREE_EXTRA  sizeof(LONG)
#define GET_TD(hwnd)  ((TREEDATA FAR *)GetWindowLong(hwnd, 0))

/* -----------------------------------------------------------------------
 * Enumerate child directories of szPath into aNodes starting at *pIdx
 * ----------------------------------------------------------------------- */
static void EnumChildren(TREEDATA FAR *ptd, LPCSTR szPath,
                          int nLevel, int FAR *pIdx)
{
    struct _find_t fd;
    char   spec[144];
    lstrcpy(spec, szPath); lstrcat(spec, "*.*");
    if (_dos_findfirst(spec, _A_SUBDIR, &fd) != 0) {
        return;
    }
    do {
        TREENODE FAR *pn;
        if (fd.name[0] == '.') {
            continue;
        }
        if (!(fd.attrib & _A_SUBDIR)) {
            continue;
        }
        if (!g_ShowHidSys && (fd.attrib & (_A_HIDDEN | _A_SYSTEM))) {
            continue;
        }
        if (*pIdx >= TREE_MAX_NODES) {
            break;
        }

        pn = &ptd->aNodes[(*pIdx)++];
        lstrcpyn(pn->szName, fd.name, sizeof(pn->szName));
        lstrcpy(pn->szFullPath, szPath);
        lstrcat(pn->szFullPath, fd.name);
        lstrcat(pn->szFullPath, "\\");
        pn->nLevel      = nLevel;
        pn->bExpanded   = FALSE;
        pn->bSelected   = FALSE;
        /* Check if it has children (try to find one sub-dir) */
        {
            char cspec[144];
            struct _find_t cfd;
            lstrcpy(cspec, pn->szFullPath); lstrcat(cspec, "*.*");
            pn->bHasChildren = FALSE;
            if (_dos_findfirst(cspec, _A_SUBDIR, &cfd) == 0) {
                do {
                    if (cfd.name[0] != '.' && (cfd.attrib & _A_SUBDIR)) {
                        pn->bHasChildren = TRUE;
                        break;
                    }
                } while (_dos_findnext(&cfd) == 0);
            }
        }
    } while (_dos_findnext(&fd) == 0);
}

/* -----------------------------------------------------------------------
 * Populate the top-level drive list
 * ----------------------------------------------------------------------- */
static void BuildDriveList(TREEDATA FAR *ptd)
{
    DWORD drives = GetLogicalDrives();
    char  letter;
    int   idx = 0;

    for (letter = 'A'; letter <= 'Z' && idx < TREE_MAX_NODES; letter++) {
        TREENODE FAR *pn;
        if (!(drives & (1UL << (letter - 'A')))) {
            continue;
        }
        pn = &ptd->aNodes[idx++];
        pn->szName[0] = letter; pn->szName[1] = ':'; pn->szName[2] = '\0';
        pn->szFullPath[0] = letter; pn->szFullPath[1] = ':';
        pn->szFullPath[2] = '\\'; pn->szFullPath[3] = '\0';
        pn->nLevel      = 0;
        pn->bExpanded   = FALSE;
        pn->bSelected   = FALSE;
        pn->bHasChildren = TRUE;
    }
    ptd->nNodes = idx;
}

/* -----------------------------------------------------------------------
 * Paint one node row
 * ----------------------------------------------------------------------- */
static void PaintNode(HDC hdc, TREEDATA FAR *ptd, int row, int idx,
                      int clientW)
{
    TREENODE FAR *pn = &ptd->aNodes[idx];
    int x = pn->nLevel * INDENT_W + 4;
    int y = row * ROW_H;
    RECT rc;

    /* Plus/minus box */
    if (pn->bHasChildren) {
        HBITMAP hbm = pn->bExpanded ? ptd->hbmMinus : ptd->hbmPlus;
        if (hbm) {
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, hbm);
            BitBlt(hdc, x, y + 1, 9, 9, hdcMem, 0, 0, SRCCOPY);
            SelectObject(hdcMem, hOld);
            DeleteDC(hdcMem);
        }
    }
    x += 12;

    /* Folder icon */
    if (ptd->hbmFolder) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, ptd->hbmFolder);
        BitBlt(hdc, x, y, 16, 16, hdcMem, 0, 0, SRCCOPY);
        SelectObject(hdcMem, hOld);
        DeleteDC(hdcMem);
    }
    x += 18;

    /* Name */
    rc.left = x; rc.top = y + 1;
    rc.right = clientW - 2; rc.bottom = y + ROW_H - 1;
    if (pn->bSelected) {
        FillRect(hdc, &rc, GetSysColorBrush(COLOR_HIGHLIGHT));
        SetTextColor(hdc, GetSysColor(COLOR_HIGHLIGHTTEXT));
    } else {
        FillRect(hdc, &rc, GetSysColorBrush(COLOR_WINDOW));
        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
    }
    SetBkMode(hdc, TRANSPARENT);
    DrawText(hdc, pn->szName, -1, &rc,
             DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
}

/* -----------------------------------------------------------------------
 * Tree window procedure
 * ----------------------------------------------------------------------- */
LRESULT CALLBACK __export TreeWndProc(HWND hwnd, UINT msg,
                                       WPARAM wParam, LPARAM lParam)
{
    TREEDATA FAR *ptd = GET_TD(hwnd);

    switch (msg) {
        case WM_CREATE: {
            TREEDATA FAR *p =
                (TREEDATA FAR *)GlobalAllocPtr(GHND, sizeof(TREEDATA));
            if (!p) {
                return -1;
            }
            p->hInst   = ((LPCREATESTRUCT)lParam)->hInstance;
            p->nScroll = 0;
            p->hbmPlus   = LoadBitmap(p->hInst, MAKEINTRESOURCE(IDB_PLUS));
            p->hbmMinus  = LoadBitmap(p->hInst, MAKEINTRESOURCE(IDB_MINUS));
            p->hbmFolder = LoadBitmap(p->hInst, MAKEINTRESOURCE(IDB_FOLDER_CLO));
            SetWindowLong(hwnd, 0, (LONG)p);
            BuildDriveList(p);
            return 0;
        }

        case WM_DESTROY: {
            if (ptd) {
                if (ptd->hbmPlus) {
                    DeleteObject(ptd->hbmPlus);
                }
                if (ptd->hbmMinus) {
                    DeleteObject(ptd->hbmMinus);
                }
                if (ptd->hbmFolder) {
                    DeleteObject(ptd->hbmFolder);
                }
                GlobalFreePtr(ptd);
                SetWindowLong(hwnd, 0, 0L);
            }
            return 0;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC  hdc = BeginPaint(hwnd, &ps);
            RECT client;
            int  i, row = 0;
            GetClientRect(hwnd, &client);
            FillRect(hdc, &client, GetSysColorBrush(COLOR_WINDOW));
            if (ptd) {
                for (i = ptd->nScroll; i < ptd->nNodes; i++, row++) {
                    if (row * ROW_H > client.bottom) {
                        break;
                    }
                    PaintNode(hdc, ptd, row, i, client.right);
                }
            }
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_LBUTTONDOWN: {
            int y = HIWORD(lParam);
            int row = y / ROW_H;
            int idx = (ptd ? ptd->nScroll : 0) + row;
            if (!ptd || idx >= ptd->nNodes) {
                return 0;
            }
            {
                TREENODE FAR *pn = &ptd->aNodes[idx];
                int btnX = pn->nLevel * INDENT_W + 4;
                int clickX = LOWORD(lParam);
                /* Plus/minus click? */
                if (pn->bHasChildren && clickX >= btnX && clickX <= btnX + 9) {
                    pn->bExpanded = !pn->bExpanded;
                    if (pn->bExpanded) {
                        /* Insert children after this node */
                        int insIdx = idx + 1;
                        TREEDATA FAR *tmp = ptd;
                        int oldN = ptd->nNodes;
                        /* Shift existing nodes to make room */
                        EnumChildren(ptd, pn->szFullPath,
                                     pn->nLevel + 1, &insIdx);
                        /* Already inserted in-place - nNodes updated above */
                    }
                    /* Collapse: remove all children */
                    else {
                        int i2 = idx + 1;
                        while (i2 < ptd->nNodes &&
                               ptd->aNodes[i2].nLevel > pn->nLevel) i2++;
                        _fmemmove(&ptd->aNodes[idx+1], &ptd->aNodes[i2],
                                  (ptd->nNodes - i2) * sizeof(TREENODE));
                        ptd->nNodes -= (i2 - idx - 1);
                    }
                } else {
                    /* Select this node */
                    int j;
                    for (j = 0; j < ptd->nNodes; j++)
                        ptd->aNodes[j].bSelected = FALSE;
                    pn->bSelected = TRUE;
                    /* Notify parent */
                    PostMessage(GetParent(hwnd), TWM_NAVIGATE,
                                0, (LPARAM)(LPCSTR)pn->szFullPath);
                }
                InvalidateRect(hwnd, NULL, FALSE);
            }
            return 0;
        }

        case WM_VSCROLL: {
            if (!ptd) {
                return 0;
            }
            switch (LOWORD(wParam)) {
                case SB_LINEUP:   ptd->nScroll = max(0, ptd->nScroll - 1); break;
                case SB_LINEDOWN: ptd->nScroll = min(ptd->nNodes - 1, ptd->nScroll + 1); break;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL Tree_RegisterClass(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style         = CS_DBLCLKS;
    wc.lpfnWndProc   = TreeWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = TREE_EXTRA;
    wc.hInstance     = hInst;
    wc.hIcon         = 0;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = TREE_CLASS;
    return RegisterClass(&wc) != 0;
}

HWND Tree_Create(HINSTANCE hInst, HWND hwndParent, LPRECT prc)
{
    return CreateWindow(TREE_CLASS, "",
                        WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER,
                        prc->left, prc->top,
                        prc->right - prc->left,
                        prc->bottom - prc->top,
                        hwndParent, (HMENU)IDW_TREE, hInst, NULL);
}

void Tree_SelectPath(HWND hwndTree, LPCSTR szPath)
{
    TREEDATA FAR *ptd = GET_TD(hwndTree);
    int i;
    if (!ptd) {
        return;
    }
    for (i = 0; i < ptd->nNodes; i++) {
        ptd->aNodes[i].bSelected =
            lstrcmpi(ptd->aNodes[i].szFullPath, szPath) == 0;
    }
    InvalidateRect(hwndTree, NULL, FALSE);
}

void Tree_GetPath(HWND hwndTree, LPSTR buf, int bufLen)
{
    TREEDATA FAR *ptd = GET_TD(hwndTree);
    int i;
    buf[0] = '\0';
    if (!ptd) {
        return;
    }
    for (i = 0; i < ptd->nNodes; i++) {
        if (ptd->aNodes[i].bSelected) {
            lstrcpyn(buf, ptd->aNodes[i].szFullPath, bufLen);
            return;
        }
    }
}

void Tree_RefreshDrives(HWND hwndTree)
{
    TREEDATA FAR *ptd = GET_TD(hwndTree);
    if (!ptd) {
        return;
    }
    BuildDriveList(ptd);
    InvalidateRect(hwndTree, NULL, TRUE);
}

void Tree_RefreshNode(HWND hwndTree, LPCSTR szPath)
{
    InvalidateRect(hwndTree, NULL, FALSE);
}
