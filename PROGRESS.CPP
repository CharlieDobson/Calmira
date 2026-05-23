/***************************************************************************
 *
 *  PROGRESS.CPP - Modeless progress dialog
 *  Translated from PROGRESS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include "HELPERS.H"
#include "PROGRESS.H"
#include "RESOURCE.H"

static BOOL s_bAborted = FALSE;

BOOL CALLBACK __export ProgressDlgProc(HWND hwnd, UINT msg,
                                       WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            s_bAborted = FALSE;
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDC_PROG_ABORT) {
                s_bAborted = TRUE;
                ShowWindow(hwnd, SW_HIDE);
            }
            return TRUE;
        }
    }
    return FALSE;
}

HWND Progress_Create(HINSTANCE hInst, HWND hwndParent)
{
    return CreateDialog(hInst, MAKEINTRESOURCE(IDD_PROGRESS),
                        hwndParent, (DLGPROC)ProgressDlgProc);
}

void Progress_Begin(HWND hwndProg, LPCSTR szOp)
{
    if (!hwndProg) {
        return;
    }
    s_bAborted = FALSE;
    SetDlgItemText(hwndProg, IDC_PROG_OP, szOp);
    SetDlgItemText(hwndProg, IDC_PROG_FILE, "");
    ShowWindow(hwndProg, SW_SHOW);
    UpdateWindow(hwndProg);
}

void Progress_Update(HWND hwndProg, LPCSTR szFile, int nPercent)
{
    HDC  hdc;
    RECT rc, fill;
    if (!hwndProg) {
        return;
    }
    SetDlgItemText(hwndProg, IDC_PROG_FILE, szFile);
    /* Draw simple progress bar */
    hdc = GetDlgCtrlDC(GetDlgItem(hwndProg, IDC_PROG_BAR));
    if (hdc) {
        GetClientRect(GetDlgItem(hwndProg, IDC_PROG_BAR), &rc);
        fill = rc;
        fill.right = rc.left + (rc.right - rc.left) * nPercent / 100;
        FillRect(hdc, &fill, GetSysColorBrush(COLOR_HIGHLIGHT));
        fill.left = fill.right; fill.right = rc.right;
        FillRect(hdc, &fill, GetSysColorBrush(COLOR_WINDOW));
        ReleaseDC(GetDlgItem(hwndProg, IDC_PROG_BAR), hdc);
    }
    UpdateWindow(hwndProg);
    /* Pump messages to keep responsive */
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

void Progress_End(HWND hwndProg)
{
    if (hwndProg) {
        ShowWindow(hwndProg, SW_HIDE);
    }
}

BOOL Progress_Aborted(HWND hwndProg)
{
    return s_bAborted;
}
