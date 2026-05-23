/***************************************************************************
 *  SYSPROP.CPP - System Properties dialog stub
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "SYSPROP.H"
#include "RESOURCE.H"
#include "SETTINGS.H"

BOOL CALLBACK __export SysPropProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            SetDlgItemText(hwnd, IDC_SP_CAPTION, g_ComputerCaption);
            CheckDlgButton(hwnd, IDC_SP_SPLASH,   g_ShowSplash);
            CheckDlgButton(hwnd, IDC_SP_RESTORE,  g_RestoreSys);
            CheckDlgButton(hwnd, IDC_SP_SYSQUIT,  g_SysWinQuit);
            CheckDlgButton(hwnd, IDC_SP_HOTKEYS,  g_GlobalHotkeys);
            SetDlgItemText(hwnd, IDC_SP_BROWSER,  g_Browser);
            return TRUE;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    GetDlgItemText(hwnd, IDC_SP_CAPTION, g_ComputerCaption, sizeof(g_ComputerCaption));
                    g_ShowSplash     = IsDlgButtonChecked(hwnd, IDC_SP_SPLASH);
                    g_RestoreSys     = IsDlgButtonChecked(hwnd, IDC_SP_RESTORE);
                    g_SysWinQuit     = IsDlgButtonChecked(hwnd, IDC_SP_SYSQUIT);
                    g_GlobalHotkeys  = IsDlgButtonChecked(hwnd, IDC_SP_HOTKEYS);
                    GetDlgItemText(hwnd, IDC_SP_BROWSER, g_Browser, sizeof(g_Browser));
                    SaveSystemProp();
                    AnnounceSettingsChanged(scSystem);
                    EndDialog(hwnd, IDOK);
                    return TRUE;
                }
                case IDCANCEL: {
                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;
                }
            }
            break;
        }
    }
    return FALSE;
}

BOOL SysProp_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_SYSPROP),
                     hwndParent, (DLGPROC)SysPropProc) == IDOK;
}
