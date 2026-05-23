/***************************************************************************
 *  TASKMAN.CPP - Calmira Task Manager dialog
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "TASKMAN.H"
#include "RESOURCE.H"

BOOL CALLBACK __export TaskManProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP)
{
    switch (msg) {
        case WM_INITDIALOG: {
            /* Populate listbox with top-level windows */
            HWND hw = GetTopWindow(NULL);
            while (hw) {
                char title[128];
                GetWindowText(hw, title, sizeof(title));
                if (*title && IsWindowVisible(hw) && !GetParent(hw)) {
                    SendDlgItemMessage(hwnd, 1001, LB_ADDSTRING, 0, (LPARAM)(LPCSTR)title);
                }
                hw = GetNextWindow(hw, GW_HWNDNEXT);
            }
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wP) == IDOK || LOWORD(wP) == IDCANCEL)
                { EndDialog(hwnd, LOWORD(wP)); return TRUE; }
            break;
        }
    }
    return FALSE;
}

void TaskMan_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    /* Use Windows built-in task manager as fallback */
    WinExec("taskman.exe", SW_SHOW);
}
