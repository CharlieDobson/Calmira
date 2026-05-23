/***************************************************************************
 *  STRTPROP.CPP - Start Menu Properties dialog
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "STRTPROP.H"
#include "RESOURCE.H"
#include "SETTINGS.H"

BOOL CALLBACK __export StrtPropProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            CheckDlgButton(hwnd, IDC_START_3D,    g_StartMenu3D);
            CheckDlgButton(hwnd, IDC_START_BOLD,  g_BoldSelect);
            CheckDlgButton(hwnd, IDC_START_SHELL, g_ShellStartup);
            CheckDlgButton(hwnd, IDC_START_MOUSE, g_StartMouseUp);
            CheckDlgButton(hwnd, IDC_START_LARGE, g_LargeRootMenu);
            CheckDlgButton(hwnd, IDC_START_WIN95, g_Win95Keyboard);
            CheckDlgButton(hwnd, IDC_START_COLOUR,g_ColouredBar);
            SetDlgItemText(hwnd, IDC_START_FILE,  g_szStartFile);
            SetDlgItemInt(hwnd,  IDC_START_HEIGHT,g_LargeItemHeight, FALSE);
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDOK) {
                g_StartMenu3D    = IsDlgButtonChecked(hwnd, IDC_START_3D);
                g_BoldSelect     = IsDlgButtonChecked(hwnd, IDC_START_BOLD);
                g_ShellStartup   = IsDlgButtonChecked(hwnd, IDC_START_SHELL);
                g_StartMouseUp   = IsDlgButtonChecked(hwnd, IDC_START_MOUSE);
                g_LargeRootMenu  = IsDlgButtonChecked(hwnd, IDC_START_LARGE);
                g_Win95Keyboard  = IsDlgButtonChecked(hwnd, IDC_START_WIN95);
                g_ColouredBar    = IsDlgButtonChecked(hwnd, IDC_START_COLOUR);
                g_LargeItemHeight= GetDlgItemInt(hwnd, IDC_START_HEIGHT, NULL, FALSE);
                SaveStartProp();
                AnnounceSettingsChanged(scStartMenu);
                EndDialog(hwnd, IDOK); return TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) { EndDialog(hwnd, IDCANCEL); return TRUE; }
            break;
        }
    }
    return FALSE;
}

BOOL StrtProp_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_STRTPROP), hwndParent, (DLGPROC)StrtPropProc) == IDOK;
}
