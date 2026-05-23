/***************************************************************************
 *  FILTER.CPP - File filter dialog stub      (MSVC 1.52c / Win16)
 ***************************************************************************/
#include <windows.h>
#include "FILTER.H"
#include "RESOURCE.H"

static char  s_buf[84];
static int   s_bufLen;

BOOL CALLBACK __export FilterProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP)
{
    switch (msg) {
        case WM_INITDIALOG: {
            SetDlgItemText(hwnd, IDC_FILT_MASK, s_buf);
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wP) == IDOK) {
                GetDlgItemText(hwnd, IDC_FILT_MASK, s_buf, s_bufLen);
                EndDialog(hwnd, IDOK); return TRUE;
            }
            if (LOWORD(wP) == IDCANCEL) { EndDialog(hwnd, IDCANCEL); return TRUE; }
            break;
        }
    }
    return FALSE;
}

BOOL Filter_Show(HINSTANCE hInst, HWND hwndParent, LPSTR buf, int bufLen)
{
    lstrcpyn(s_buf, buf, sizeof(s_buf));
    s_bufLen = bufLen;
    if (DialogBox(hInst, MAKEINTRESOURCE(IDD_FILTER), hwndParent, (DLGPROC)FilterProc) == IDOK) {
        lstrcpyn(buf, s_buf, bufLen);
        return TRUE;
    }
    return FALSE;
}
