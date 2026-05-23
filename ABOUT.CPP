/***************************************************************************
 *
 *  ABOUT.CPP - About dialog
 *  Translated from ABOUT.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include "ABOUT.H"
#include "RESOURCE.H"
#include "LOCALE.H"
#include "SETTINGS.H"
#include "FILEMAN.H"

static HINSTANCE s_hInst = 0;

BOOL CALLBACK __export AboutDlgProc(HWND hwnd, UINT msg,
                                    WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            char buf[32];
            DWORD dwFree;

            /* Memory and resource info */
            dwFree = GetFreeSpace(0);           /* Win16 - free global heap */
            wsprintf(buf, "%lu KB free", dwFree / 1024);
            SetDlgItemText(hwnd, IDC_ABOUT_MEM, buf);

            wsprintf(buf, "%d%%", GetFreeSystemResources(GFSR_SYSTEMRESOURCES));
            SetDlgItemText(hwnd, IDC_ABOUT_RES, buf);

            /* Make the URL label look like a link (underline, blue) */
            /* In Win16, we just leave it as the static text already set in RC */
            return TRUE;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK:
                case IDCANCEL: {
                    EndDialog(hwnd, IDOK);
                    return TRUE;
                }

                case IDC_ABOUT_LIC: {
                    /* Open LICENSE.TXT in Notepad */
                    char szLic[144];
                    lstrcpy(szLic, g_szAppPath);
                    lstrcat(szLic, "LICENSE.TXT");
                    if (!FileMan_Exec(hwnd, "notepad.exe", szLic, NULL, SW_SHOW)) {
                        char msg[256];
                        LoadString(s_hInst, SCannotRunNotepad, msg, sizeof(msg));
                        MessageBox(hwnd, msg, "Calmira", MB_ICONEXCLAMATION | MB_OK);
                    }
                    return TRUE;
                }

                case IDC_ABOUT_WEB: {
                    /* Open the Calmira homepage */
                    char szBrowser[144];
                    lstrcpy(szBrowser, g_Browser);
                    if (!*szBrowser) {
                        lstrcpy(szBrowser, "iexplore.exe");
                    }
                    FileMan_Exec(hwnd, szBrowser,
                                 "http://www.calmira.org/", NULL, SW_SHOW);
                    return TRUE;
                }
            }
            break;
        }
    }
    return FALSE;
}

void About_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    s_hInst = hInst;
    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT), hwndParent, (DLGPROC)AboutDlgProc);
}
