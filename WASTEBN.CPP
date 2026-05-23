/***************************************************************************
 *
 *  WASTEBN.CPP - Recycle Bin window
 *  Translated from WASTEBIN.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include <dos.h>
#include <string.h>
#include <stdio.h>
#include "WASTEBN.H"
#include "SETTINGS.H"
#include "RESOURCE.H"
#include "HELPERS.H"
#include "DIRECTRY.H"
#include "FILEMAN.H"

#define BIN_EXTRA  sizeof(LONG)
#define BIN_SECT   "Bin Locations"

typedef struct tagBINSTATE {
    HINSTANCE hInst;
    HWND      hwndList;
    BINITEM   aItems[64];
    int       nItems;
} BINSTATE;

#define GET_BS(hwnd)  ((BINSTATE FAR *)GetWindowLong(hwnd, 0))

void Bin_GetDir(LPSTR buf, int bufLen)
{
    lstrcpyn(buf, g_szWritePath, bufLen);
    lstrcat(buf, BIN_SUBDIR);
}

static void LoadBinItems(BINSTATE FAR *pbs)
{
    char  iniPath[144], buf[4096];
    LPCSTR p;
    pbs->nItems = 0;
    lstrcpy(iniPath, g_szWritePath);
    lstrcat(iniPath, "calmira.ini");
    GetPrivateProfileSection(BIN_SECT, buf, sizeof(buf), iniPath);
    p = buf;
    while (p && *p && pbs->nItems < 64) {
        BINITEM FAR *pi = &pbs->aItems[pbs->nItems];
        LPCSTR eq = p;
        while (*eq && *eq != '=') eq++;
        if (*eq == '=') {
            lstrcpyn(pi->szOrigPath, p, (int)(eq - p) + 1);
            lstrcpyn(pi->szBinName, eq + 1, sizeof(pi->szBinName));
            pi->bIsFolder = FALSE;
            pbs->nItems++;
        }
        p += lstrlen(p) + 1;
    }
}

static void PopulateList(HWND hwndList, BINSTATE FAR *pbs)
{
    int i;
    SendMessage(hwndList, LB_RESETCONTENT, 0, 0L);
    for (i = 0; i < pbs->nItems; i++)
        SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)pbs->aItems[i].szOrigPath);
}

LRESULT CALLBACK __export BinWndProc(HWND hwnd, UINT msg,
                                      WPARAM wParam, LPARAM lParam)
{
    BINSTATE FAR *pbs = GET_BS(hwnd);

    switch (msg) {
        case WM_CREATE: {
            BINSTATE FAR *p =
                (BINSTATE FAR *)GlobalAllocPtr(GHND, sizeof(BINSTATE));
            RECT client;
            if (!p) {
                return -1;
            }
            p->hInst = ((LPCREATESTRUCT)lParam)->hInstance;
            SetWindowLong(hwnd, 0, (LONG)p);
            GetClientRect(hwnd, &client);
            p->hwndList = CreateWindow("LISTBOX", "",
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY,
                0, 0, client.right, client.bottom,
                hwnd, (HMENU)IDW_BIN_LIST, p->hInst, NULL);
            LoadBinItems(p);
            PopulateList(p->hwndList, p);
            Bin_LoadPos(hwnd);
            return 0;
        }

        case WM_DESTROY: {
            Bin_SavePos(hwnd);
            if (pbs) { GlobalFreePtr(pbs); SetWindowLong(hwnd, 0, 0L); }
            return 0;
        }

        case WM_SIZE: {
            if (pbs && pbs->hwndList) {
                RECT client;
                GetClientRect(hwnd, &client);
                SetWindowPos(pbs->hwndList, NULL, 0, 0,
                             client.right, client.bottom,
                             SWP_NOZORDER | SWP_NOACTIVATE);
            }
            return 0;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDM_BIN_RESTORE: {
                    int idx = (int)SendMessage(pbs->hwndList,
                                               LB_GETCURSEL, 0, 0L);
                    if (idx >= 0) {
                        Bin_RestoreItem(hwnd, idx);
                    }
                    return 0;
                }
                case IDM_BIN_EMPTY: {
                    Bin_Empty(hwnd);
                    return 0;
                }
                case IDM_OBJ_DELETE: {
                    int idx = (int)SendMessage(pbs->hwndList,
                                               LB_GETCURSEL, 0, 0L);
                    if (idx >= 0) {
                        Bin_DeleteItem(hwnd, idx);
                    }
                    return 0;
                }
            }
            return 0;
        }

        case WM_CLOSE: {
            ShowWindow(hwnd, SW_HIDE);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

BOOL Bin_RegisterClass(HINSTANCE hInst)
{
    WNDCLASS wc;
    wc.style         = 0;
    wc.lpfnWndProc   = BinWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = BIN_EXTRA;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_BIN_EMPTY));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = BIN_CLASS;
    return RegisterClass(&wc) != 0;
}

HWND Bin_Create(HINSTANCE hInst, HWND hwndParent)
{
    int l = Profile_GetInt(&g_Ini, "Bin", "Left",    150);
    int t = Profile_GetInt(&g_Ini, "Bin", "Top",      42);
    int w = Profile_GetInt(&g_Ini, "Bin", "Width",   352);
    int h = Profile_GetInt(&g_Ini, "Bin", "Height",  229);
    return CreateWindow(BIN_CLASS, g_BinCaption,
                        WS_OVERLAPPEDWINDOW,
                        l, t, w, h,
                        hwndParent, NULL, hInst, NULL);
}

BOOL Bin_AddFile(HWND hwndBin, LPCSTR szSrcPath)
{
    BINSTATE FAR *pbs = GET_BS(hwndBin);
    char  binDir[144], destName[144], destPath[144];
    int   n;

    if (!pbs) {
        return FALSE;
    }
    if (pbs->nItems >= 64) {
        return FALSE;
    }

    Bin_GetDir(binDir, sizeof(binDir));
    Directory_MakeTree(binDir);

    /* Generate a unique 8.3 name in the bin directory */
    n = pbs->nItems;
    wsprintf(destName, "BIN%05d.TMP", n);
    Directory_JoinPath(binDir, destName, destPath, sizeof(destPath));

    {
        /* rename/remove take near char*; copy far szSrcPath to local buffer */
        char srcNear[144];
        lstrcpyn(srcNear, szSrcPath, sizeof(srcNear));
        if (rename(srcNear, destPath) != 0) {
            /* Cross-drive: copy then remove */
            if (!CopyFileRaw(szSrcPath, destPath)) {
                return FALSE;
            }
            remove(srcNear);
        }
    }

    lstrcpy(pbs->aItems[n].szOrigPath, szSrcPath);
    lstrcpy(pbs->aItems[n].szBinName,  destName);
    pbs->aItems[n].bIsFolder = FALSE;
    pbs->nItems++;

    /* Persist to INI */
    WritePrivateProfileString(BIN_SECT, szSrcPath, destName, g_Ini.szPath);

    if (pbs->hwndList) {
        SendMessage(pbs->hwndList, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)szSrcPath);
    }
    return TRUE;
}

BOOL Bin_RestoreItem(HWND hwndBin, int nIndex)
{
    BINSTATE FAR *pbs = GET_BS(hwndBin);
    char binDir[144], srcPath[144];
    BINITEM FAR *pi;
    if (!pbs || nIndex < 0 || nIndex >= pbs->nItems) {
        return FALSE;
    }
    pi = &pbs->aItems[nIndex];
    Bin_GetDir(binDir, sizeof(binDir));
    Directory_JoinPath(binDir, pi->szBinName, srcPath, sizeof(srcPath));
    {
        /* pi->szOrigPath is accessed via FAR pointer; copy to near buffer */
        char origNear[144];
        lstrcpyn(origNear, pi->szOrigPath, sizeof(origNear));
        if (rename(srcPath, origNear) != 0) {
            return FALSE;
        }
    }
    WritePrivateProfileString(BIN_SECT, pi->szOrigPath, NULL, g_Ini.szPath);
    _fmemmove(&pbs->aItems[nIndex], &pbs->aItems[nIndex+1],
              (pbs->nItems - nIndex - 1) * sizeof(BINITEM));
    pbs->nItems--;
    if (pbs->hwndList) {
        SendMessage(pbs->hwndList, LB_DELETESTRING, nIndex, 0L);
    }
    return TRUE;
}

BOOL Bin_DeleteItem(HWND hwndBin, int nIndex)
{
    BINSTATE FAR *pbs = GET_BS(hwndBin);
    char binDir[144], path[144];
    BINITEM FAR *pi;
    if (!pbs || nIndex < 0 || nIndex >= pbs->nItems) {
        return FALSE;
    }
    pi = &pbs->aItems[nIndex];
    Bin_GetDir(binDir, sizeof(binDir));
    Directory_JoinPath(binDir, pi->szBinName, path, sizeof(path));
    remove(path);
    WritePrivateProfileString(BIN_SECT, pi->szOrigPath, NULL, g_Ini.szPath);
    _fmemmove(&pbs->aItems[nIndex], &pbs->aItems[nIndex+1],
              (pbs->nItems - nIndex - 1) * sizeof(BINITEM));
    pbs->nItems--;
    if (pbs->hwndList) {
        SendMessage(pbs->hwndList, LB_DELETESTRING, nIndex, 0L);
    }
    return TRUE;
}

void Bin_Empty(HWND hwndBin)
{
    BINSTATE FAR *pbs = GET_BS(hwndBin);
    if (!pbs) {
        return;
    }
    while (pbs->nItems > 0)
        Bin_DeleteItem(hwndBin, 0);
}

void Bin_Refresh(HWND hwndBin)
{
    BINSTATE FAR *pbs = GET_BS(hwndBin);
    if (!pbs) {
        return;
    }
    LoadBinItems(pbs);
    if (pbs->hwndList) {
        PopulateList(pbs->hwndList, pbs);
    }
}

int Bin_GetCount(HWND hwndBin)
{
    BINSTATE FAR *pbs = GET_BS(hwndBin);
    return pbs ? pbs->nItems : 0;
}

void Bin_SavePos(HWND hwndBin)
{
    WINDOWPLACEMENT wp;
    wp.length = sizeof(wp);
    GetWindowPlacement(hwndBin, &wp);
    Profile_SetInt(&g_Ini, "Bin", "Left",   wp.rcNormalPosition.left);
    Profile_SetInt(&g_Ini, "Bin", "Top",    wp.rcNormalPosition.top);
    Profile_SetInt(&g_Ini, "Bin", "Width",  wp.rcNormalPosition.right  - wp.rcNormalPosition.left);
    Profile_SetInt(&g_Ini, "Bin", "Height", wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);
}

void Bin_LoadPos(HWND hwndBin)
{
    /* Position already set in Bin_Create from INI */
}
