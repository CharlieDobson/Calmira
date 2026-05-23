/***************************************************************************
 *  DESKPROP.CPP - Desktop Properties dialog
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "DESKPROP.H"
#include "RESOURCE.H"
#include "SETTINGS.H"

BOOL CALLBACK __export DeskPropProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            CheckDlgButton(hwnd, IDC_DESK_SINGLE, g_SingleStatus);
            CheckDlgButton(hwnd, IDC_DESK_SAVE,   g_SaveWindows);
            CheckDlgButton(hwnd, IDC_DESK_ARROWS, g_ShortArrows);
            CheckDlgButton(hwnd, IDC_DESK_MENU,   g_ShowDeskMenu);
            CheckDlgButton(hwnd, IDC_DESK_RCLOSE, g_RightClose);
            SetDlgItemInt(hwnd, IDC_DESK_GRIDX, g_DeskGrid.x, FALSE);
            SetDlgItemInt(hwnd, IDC_DESK_GRIDY, g_DeskGrid.y, FALSE);
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDOK) {
                g_SingleStatus = IsDlgButtonChecked(hwnd, IDC_DESK_SINGLE);
                g_SaveWindows  = IsDlgButtonChecked(hwnd, IDC_DESK_SAVE);
                g_ShortArrows  = IsDlgButtonChecked(hwnd, IDC_DESK_ARROWS);
                g_ShowDeskMenu = IsDlgButtonChecked(hwnd, IDC_DESK_MENU);
                g_RightClose   = IsDlgButtonChecked(hwnd, IDC_DESK_RCLOSE);
                g_DeskGrid.x   = GetDlgItemInt(hwnd, IDC_DESK_GRIDX, NULL, FALSE);
                g_DeskGrid.y   = GetDlgItemInt(hwnd, IDC_DESK_GRIDY, NULL, FALSE);
                SaveDeskProp();
                AnnounceSettingsChanged(scDesktop);
                EndDialog(hwnd, IDOK); return TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) { EndDialog(hwnd, IDCANCEL); return TRUE; }
            break;
        }
    }
    return FALSE;
}

BOOL DeskProp_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DESKPROP), hwndParent, (DLGPROC)DeskPropProc) == IDOK;
}
