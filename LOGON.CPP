/***************************************************************************
 *  LOGON.CPP - Calmira logon check
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "LOGON.H"
#include "SETTINGS.H"
#include "RESOURCE.H"

BOOL CALLBACK __export LogonProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: return TRUE;
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDOK)    { EndDialog(hwnd, IDOK);     return TRUE; }
            if (LOWORD(wParam) == IDCANCEL){ EndDialog(hwnd, IDCANCEL); return TRUE; }
            break;
        }
    }
    return FALSE;
}

BOOL CheckForLogon(HINSTANCE hInst, HWND hwndParent)
{
    /* Multi-user not implemented in this port - always succeed */
    return TRUE;
}
