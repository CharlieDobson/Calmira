/***************************************************************************
 *
 *  SHUTDWN.CPP - Shutdown / Quit dialog
 *  Translated from SHUTDOWN.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Shows a dialog with four radio buttons matching the Delphi original:
 *    Restart in MS-DOS mode
 *    Restart Windows
 *    Restart the computer
 *    Shut down Windows
 *
 ***************************************************************************/

#include <windows.h>
#include "SHUTDWN.H"
#include "RESOURCE.H"
#include "SETTINGS.H"
#include "LOCALE.H"

static int s_nResult = SD_CANCEL;

BOOL CALLBACK __export ShutdownDlgProc(HWND hwnd, UINT msg,
                                      WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            /* Default: Shut down Windows */
            CheckRadioButton(hwnd, IDC_SHUT_DOS, IDC_SHUT_QUIT, IDC_SHUT_QUIT);
            return TRUE;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    if (IsDlgButtonChecked(hwnd, IDC_SHUT_DOS)) {
                        s_nResult = SD_DOS;
                    }
                    else if (IsDlgButtonChecked(hwnd, IDC_SHUT_WIN)) {
                        s_nResult = SD_WINDOWS;
                    }
                    else if (IsDlgButtonChecked(hwnd, IDC_SHUT_BOOT)) {
                        s_nResult = SD_REBOOT;
                    }
                    else {
                        s_nResult = SD_QUIT;
                    }
                    EndDialog(hwnd, IDOK);
                    return TRUE;
                }
                case IDCANCEL: {
                    s_nResult = SD_CANCEL;
                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;
                }
            }
            break;
        }
    }
    return FALSE;
}

int Shutdown_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    s_nResult = SD_CANCEL;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_SHUTDOWN), hwndParent,
              (DLGPROC)ShutdownDlgProc);
    return s_nResult;
}

void Shutdown_Execute(int nAction, BOOL bShowBmp, HINSTANCE hInst)
{
    if (bShowBmp && nAction != SD_CANCEL) {
        /* Show the shutdown bitmap (SD-CALII.BMP) full-screen */
        HBITMAP hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SHUTDOWN));
        if (hBmp) {
            HDC hdc = GetDC(NULL);
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, hBmp);
            BITMAP  bm;
            GetObject(hBmp, sizeof(bm), &bm);
            SetStretchBltMode(hdc, COLORONCOLOR);
            StretchBlt(hdc, 0, 0,
                       GetSystemMetrics(SM_CXSCREEN),
                       GetSystemMetrics(SM_CYSCREEN),
                       hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
            SelectObject(hdcMem, hOld);
            DeleteDC(hdcMem);
            ReleaseDC(NULL, hdc);
            DeleteObject(hBmp);
        }
    }

    switch (nAction) {
        case SD_DOS: {
            /* Exit to DOS: use EW_RESTARTSYSTEM with RNMODE flag
               In Win 3.x, ExitWindows(EW_RESTARTWINDOWS, 0) restarts Windows,
               and ExitWindows(EW_REBOOTSYSTEM, 0) reboots.
               For DOS prompt: ExitWindows(0, 0) returns to DOS shell. */
            ExitWindows(0, 0);
            break;
        }

        case SD_WINDOWS: {
            ExitWindows(EW_RESTARTWINDOWS, 0);
            break;
        }

        case SD_REBOOT: {
            ExitWindows(EW_REBOOTSYSTEM, 0);
            break;
        }

        case SD_QUIT: {
            /* Gracefully close Windows */
            ExitWindows(0, 0);
            break;
        }
    }
}
