/***************************************************************************
 *  REFERENC.CPP - Shortcut reference editor stub
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include <memory.h>
#include <string.h>
#include "REFERENC.H"
#include "RESOURCE.H"

static SHORTINFO s_si;

BOOL CALLBACK __export RefProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP)
{
    switch (msg) {
        case WM_INITDIALOG: {
            SetDlgItemText(hwnd, 1001, s_si.Target);
            SetDlgItemText(hwnd, 1002, s_si.Caption);
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wP) == IDOK) {
                GetDlgItemText(hwnd, 1001, s_si.Target, sizeof(s_si.Target));
                GetDlgItemText(hwnd, 1002, s_si.Caption, sizeof(s_si.Caption));
                EndDialog(hwnd, IDOK); return TRUE;
            }
            if (LOWORD(wP) == IDCANCEL) { EndDialog(hwnd, IDCANCEL); return TRUE; }
            break;
        }
    }
    return FALSE;
}

BOOL Referenc_EditShortcut(HINSTANCE hInst, HWND hwndParent, SHORTINFO FAR *psi)
{
    _fmemcpy(&s_si, psi, sizeof(SHORTINFO));
    if (DialogBox(hInst, MAKEINTRESOURCE(IDD_SHORTS), hwndParent, (DLGPROC)RefProc) == IDOK) {
        _fmemcpy(psi, &s_si, sizeof(SHORTINFO));
        return TRUE;
    }
    return FALSE;
}
