/***************************************************************************
 *  FSYSPROP.CPP - File System Properties dialog
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "FSYSPROP.H"
#include "RESOURCE.H"
#include "SETTINGS.H"

BOOL CALLBACK __export FSysPropProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            CheckDlgButton(hwnd, IDC_SP_RESTORE, g_ShowHidSys);  /* reuse IDs */
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDOK) {
                g_ShowHidSys = IsDlgButtonChecked(hwnd, IDC_SP_RESTORE);
                SaveFileSysProp();
                AnnounceSettingsChanged(scFileSystem);
                EndDialog(hwnd, IDOK); return TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) { EndDialog(hwnd, IDCANCEL); return TRUE; }
            break;
        }
    }
    return FALSE;
}

BOOL FSysProp_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_FSYSPROP), hwndParent, (DLGPROC)FSysPropProc) == IDOK;
}
