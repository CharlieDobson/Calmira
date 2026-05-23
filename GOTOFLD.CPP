/***************************************************************************
 *  GOTOFLD.CPP - Go To Folder dialog         (MSVC 1.52c / Win16)
 ***************************************************************************/
#include <windows.h>
#include "GOTOFLD.H"
#include "RESOURCE.H"

static char s_buf[144];
static int  s_bufLen;

BOOL CALLBACK __export GotoDlgProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP)
{
    switch (msg) {
        case WM_INITDIALOG: {
            SetDlgItemText(hwnd, 1001, s_buf);
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wP) == IDOK) {
                GetDlgItemText(hwnd, 1001, s_buf, s_bufLen);
                EndDialog(hwnd, IDOK); return TRUE;
            }
            if (LOWORD(wP) == IDCANCEL) { EndDialog(hwnd, IDCANCEL); return TRUE; }
            break;
        }
    }
    return FALSE;
}

BOOL GotoFld_Show(HINSTANCE hInst, HWND hwndParent, LPSTR buf, int bufLen)
{
    lstrcpyn(s_buf, buf, sizeof(s_buf));
    s_bufLen = bufLen;
    if (DialogBox(hInst, MAKEINTRESOURCE(IDD_GOTOFLD), hwndParent, (DLGPROC)GotoDlgProc) == IDOK) {
        lstrcpyn(buf, s_buf, bufLen);
        return TRUE;
    }
    return FALSE;
}
