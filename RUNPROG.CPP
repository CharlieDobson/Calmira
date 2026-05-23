/***************************************************************************
 *
 *  RUNPROG.CPP - Run Program dialog
 *  Translated from RUNPROG.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include <commdlg.h>
#include <memory.h>
#include <string.h>
#include "RUNPROG.H"
#include "RESOURCE.H"
#include "SETTINGS.H"
#include "FILEMAN.H"

static HINSTANCE s_hInst = 0;

/* Show values in the "Show" combobox */
static const char * const s_aShowModes[] = {
    "Normal", "Minimized", "Maximized", NULL
};
static const int s_aShowCodes[] = {
    SW_SHOWNORMAL, SW_SHOWMINIMIZED, SW_SHOWMAXIMIZED
};

BOOL CALLBACK __export RunProgDlgProc(HWND hwnd, UINT msg,
                                     WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            int i;
            HWND hwndShow = GetDlgItem(hwnd, IDC_RUN_SHOW);
            for (i = 0; s_aShowModes[i]; i++)
                SendMessage(hwndShow, CB_ADDSTRING, 0,
                            (LPARAM)(LPCSTR)s_aShowModes[i]);
            SendMessage(hwndShow, CB_SETCURSEL, 0, 0L);

            /* Restore last command from INI history */
            {
                char last[144];
                Profile_GetStr(&g_Ini, "History", "RunProgram", "",
                               last, sizeof(last));
                SetDlgItemText(hwnd, IDC_RUN_CMD, last);
                SendDlgItemMessage(hwnd, IDC_RUN_CMD,
                                   EM_SETSEL, 0, -1);
            }
            return TRUE;
        }

        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case IDOK: {
                    char cmd[144];
                    int  nSel, nShow;
                    BOOL bOK;

                    GetDlgItemText(hwnd, IDC_RUN_CMD, cmd, sizeof(cmd));
                    if (!*cmd) {
                        return TRUE;
                    }

                    nSel  = (int)SendDlgItemMessage(hwnd, IDC_RUN_SHOW,
                                                    CB_GETCURSEL, 0, 0L);
                    nShow = (nSel >= 0) ? s_aShowCodes[nSel] : SW_SHOWNORMAL;

                    /* Save to history */
                    Profile_SetStr(&g_Ini, "History", "RunProgram", cmd);

                    bOK = FileMan_DefaultExec(hwnd, cmd, NULL, NULL, nShow);
                    if (!bOK) return TRUE;  /* error already reported */

                    if (g_RunAutoClose) {
                        EndDialog(hwnd, IDOK);
                    }
                    return TRUE;
                }

                case IDCANCEL: {
                    EndDialog(hwnd, IDCANCEL);
                    return TRUE;
                }

                case IDC_RUN_BROWSE: {
                    /* Use GetOpenFileName to browse for an executable */
                    OPENFILENAME ofn;
                    char buf[144];
                    buf[0] = '\0';
                    memset(&ofn, 0, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner   = hwnd;
                    ofn.hInstance   = s_hInst;
                    ofn.lpstrFilter = "Programs\0*.exe;*.com;*.bat;*.pif\0All files\0*.*\0";
                    ofn.lpstrFile   = buf;
                    ofn.nMaxFile    = sizeof(buf);
                    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
                    if (GetOpenFileName(&ofn)) {
                        SetDlgItemText(hwnd, IDC_RUN_CMD, buf);
                    }
                    return TRUE;
                }
            }
            break;
        }
    }
    return FALSE;
}

BOOL RunProg_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    s_hInst = hInst;
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_RUNPROG),
                     hwndParent, (DLGPROC)RunProgDlgProc) == IDOK;
}
