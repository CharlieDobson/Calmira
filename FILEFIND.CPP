/***************************************************************************
 *  FILEFIND.CPP - Find Files dialog
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include <dos.h>
#include <string.h>
#include "HELPERS.H"
#include "FILEFIND.H"
#include "RESOURCE.H"
#include "SETTINGS.H"
#include "LOCALE.H"
#include "FILEMAN.H"
#include "DIRECTRY.H"

static HINSTANCE s_hInst     = 0;
static BOOL      s_bSearching = FALSE;

static void DoSearch(HWND hwnd, LPCSTR szSpec, LPCSTR szStart, BOOL bRecurse)
{
    struct _find_t fd;
    char   path[144];
    lstrcpy(path, szStart);
    if (path[lstrlen(path)-1] != '\\') {
        lstrcat(path, "\\");
    }
    {
        char fullSpec[144];
        lstrcpy(fullSpec, path);
        lstrcat(fullSpec, szSpec);
        if (_dos_findfirst(fullSpec, _A_NORMAL | _A_RDONLY | _A_ARCH, &fd) == 0) {
            do {
                char full[144];
                lstrcpy(full, path);
                lstrcat(full, fd.name);
                SendDlgItemMessage(hwnd, IDC_FIND_LIST, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)full);
            } while (_dos_findnext(&fd) == 0);
        }
    }
    if (bRecurse) {
        char sub[144];
        if (_dos_findfirst(path, _A_SUBDIR, &fd) == 0) {
            do {
                if (fd.name[0] == '.') {
                    continue;
                }
                if (!(fd.attrib & _A_SUBDIR)) {
                    continue;
                }
                lstrcpy(sub, path); lstrcat(sub, fd.name);
                DoSearch(hwnd, szSpec, sub, TRUE);
            } while (_dos_findnext(&fd) == 0);
        }
    }
}

BOOL CALLBACK __export FindDlgProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP)
{
    switch (msg) {
        case WM_INITDIALOG: {
            SetDlgItemText(hwnd, IDC_FIND_NAME, "*.*");
            SetDlgItemText(hwnd, IDC_FIND_PATH, "C:\\");
            CheckDlgButton(hwnd, IDC_FIND_RECURSE, BST_CHECKED);
            return TRUE;
        }
        case WM_COMMAND: {
            switch (LOWORD(wP)) {
                case IDC_FIND_START: {
                    char spec[84], path[144];
                    BOOL bRecurse = IsDlgButtonChecked(hwnd, IDC_FIND_RECURSE);
                    GetDlgItemText(hwnd, IDC_FIND_NAME, spec, sizeof(spec));
                    GetDlgItemText(hwnd, IDC_FIND_PATH, path, sizeof(path));
                    SendDlgItemMessage(hwnd, IDC_FIND_LIST, LB_RESETCONTENT, 0, 0L);
                    DoSearch(hwnd, spec, path, bRecurse);
                    return TRUE;
                }
                case IDC_FIND_OPEN: {
                    int idx = (int)SendDlgItemMessage(hwnd, IDC_FIND_LIST,
                                                      LB_GETCURSEL, 0, 0L);
                    if (idx >= 0) {
                        char full[144];
                        SendDlgItemMessage(hwnd, IDC_FIND_LIST,
                                           LB_GETTEXT, idx, (LPARAM)(LPSTR)full);
                        FileMan_DefaultExec(hwnd, full, NULL, NULL, SW_SHOWNORMAL);
                    }
                    return TRUE;
                }
                case IDCANCEL: {
                    ShowWindow(hwnd, SW_HIDE);
                    return TRUE;
                }
            }
            break;
        }
    }
    return FALSE;
}

HWND FileFind_Create(HINSTANCE hInst, HWND hwndParent)
{
    s_hInst = hInst;
    return CreateDialog(hInst, MAKEINTRESOURCE(IDD_FILEFIND),
                        hwndParent, (DLGPROC)FindDlgProc);
}

void FileFind_Show(HWND hwndFind)
{
    if (hwndFind) {
        ShowWindow(hwndFind, SW_SHOW);
        SetForegroundWindow(hwndFind);
    }
}
