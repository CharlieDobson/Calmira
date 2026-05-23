/***************************************************************************
 *  TASKPROP.CPP - Taskbar Properties dialog
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "TASKPROP.H"
#include "RESOURCE.H"
#include "SETTINGS.H"

BOOL CALLBACK __export TaskPropProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            CheckDlgButton(hwnd, IDC_TASK_DISABLE, g_DisableTaskbar);
            CheckDlgButton(hwnd, IDC_TASK_STAY,    g_StayVisible);
            CheckDlgButton(hwnd, IDC_TASK_SHRINK,  g_ShrinkMax);
            CheckDlgButton(hwnd, IDC_TASK_CLOCK24, g_Clock24);
            CheckDlgButton(hwnd, IDC_TASK_POPRES,  g_PopupRes);
            CheckDlgButton(hwnd, IDC_TASK_POPDATE, g_PopupDate);
            CheckDlgButton(hwnd, IDC_TASK_ANIMATE, g_Animate);
            CheckDlgButton(hwnd, IDC_TASK_TOP,     g_TopTaskbar);
            SetDlgItemInt(hwnd, IDC_TASK_ROWS, g_RowCount, FALSE);
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDOK) {
                g_DisableTaskbar = IsDlgButtonChecked(hwnd, IDC_TASK_DISABLE);
                g_StayVisible    = IsDlgButtonChecked(hwnd, IDC_TASK_STAY);
                g_ShrinkMax      = IsDlgButtonChecked(hwnd, IDC_TASK_SHRINK);
                g_Clock24        = IsDlgButtonChecked(hwnd, IDC_TASK_CLOCK24);
                g_PopupRes       = IsDlgButtonChecked(hwnd, IDC_TASK_POPRES);
                g_PopupDate      = IsDlgButtonChecked(hwnd, IDC_TASK_POPDATE);
                g_Animate        = IsDlgButtonChecked(hwnd, IDC_TASK_ANIMATE);
                g_TopTaskbar     = IsDlgButtonChecked(hwnd, IDC_TASK_TOP);
                g_RowCount       = GetDlgItemInt(hwnd, IDC_TASK_ROWS, NULL, FALSE);
                if (g_RowCount < 1) {
                    g_RowCount = 1;
                }
                SaveTaskProp();
                AnnounceSettingsChanged(scTaskbar);
                EndDialog(hwnd, IDOK); return TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) { EndDialog(hwnd, IDCANCEL); return TRUE; }
            break;
        }
    }
    return FALSE;
}

BOOL TaskProp_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_TASKPROP), hwndParent, (DLGPROC)TaskPropProc) == IDOK;
}
