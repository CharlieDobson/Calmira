/***************************************************************************
 *  FILEPROP.CPP - File Properties dialog
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include <dos.h>
#include "FILEPROP.H"
#include "RESOURCE.H"
#include "SETTINGS.H"
#include "LOCALE.H"
#include "DIRECTRY.H"
#include "FOURDOS.H"
#include "FILEMAN.H"
#include "HELPERS.H"
#include "ICONIC.H"

static char s_szPath[144];

BOOL CALLBACK __export FilePropProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP)
{
    switch (msg) {
        case WM_INITDIALOG: {
            struct _find_t fd;
            char  parent[144], sizeBuf[32], dateBuf[32], attrBuf[8];
            LPCSTR fname = Directory_GetName(s_szPath);
            SetDlgItemText(hwnd, IDC_FP_NAME, fname);
            Directory_GetParent(s_szPath, parent, sizeof(parent));
            SetDlgItemText(hwnd, IDC_FP_PATH, parent);
            if (_dos_findfirst(s_szPath, _A_NORMAL | _A_RDONLY | _A_ARCH |
                                         _A_HIDDEN | _A_SYSTEM, &fd) == 0) {
                ICONITEM pi;
                memset(&pi, 0, sizeof(pi));
                pi.dwSize = fd.size;
                pi.wDate  = fd.wr_date;
                pi.wTime  = fd.wr_time;
                pi.bAttr  = fd.attrib;
                Iconic_FormatSize(fd.size, sizeBuf, sizeof(sizeBuf));
                Iconic_FormatDate(fd.wr_date, fd.wr_time, dateBuf, sizeof(dateBuf));
                Iconic_FormatAttr(fd.attrib, attrBuf, sizeof(attrBuf));
                SetDlgItemText(hwnd, IDC_FP_SIZE, sizeBuf);
                SetDlgItemText(hwnd, IDC_FP_DATE, dateBuf);
                SetDlgItemText(hwnd, IDC_FP_ATTR_R, "");
                CheckDlgButton(hwnd, IDC_FP_ATTR_R, fd.attrib & _A_RDONLY);
                CheckDlgButton(hwnd, IDC_FP_ATTR_H, fd.attrib & _A_HIDDEN);
                CheckDlgButton(hwnd, IDC_FP_ATTR_S, fd.attrib & _A_SYSTEM);
                CheckDlgButton(hwnd, IDC_FP_ATTR_A, fd.attrib & _A_ARCH);
                if (g_UseDescriptions) {
                    char desc[64];
                    if (FourDOS_GetDesc(parent, fname, desc, sizeof(desc))) {
                        SetDlgItemText(hwnd, IDC_FP_DESC, desc);
                    }
                }
            }
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wP) == IDOK) {
                /* Save attribute changes */
                struct _find_t fd;
                if (_dos_findfirst(s_szPath, _A_NORMAL | _A_RDONLY | _A_ARCH |
                                             _A_HIDDEN | _A_SYSTEM, &fd) == 0) {
                    BYTE attr = 0;
                    if (IsDlgButtonChecked(hwnd, IDC_FP_ATTR_R)) {
                        attr |= _A_RDONLY;
                    }
                    if (IsDlgButtonChecked(hwnd, IDC_FP_ATTR_H)) {
                        attr |= _A_HIDDEN;
                    }
                    if (IsDlgButtonChecked(hwnd, IDC_FP_ATTR_S)) {
                        attr |= _A_SYSTEM;
                    }
                    if (IsDlgButtonChecked(hwnd, IDC_FP_ATTR_A)) {
                        attr |= _A_ARCH;
                    }
                    _dos_setfileattr(s_szPath, attr);
                }
                if (g_UseDescriptions) {
                    char desc[64], parent[144];
                    LPCSTR fname = Directory_GetName(s_szPath);
                    GetDlgItemText(hwnd, IDC_FP_DESC, desc, sizeof(desc));
                    Directory_GetParent(s_szPath, parent, sizeof(parent));
                    FourDOS_SetDesc(parent, fname, desc);
                }
                EndDialog(hwnd, IDOK); return TRUE;
            }
            if (LOWORD(wP) == IDCANCEL) { EndDialog(hwnd, IDCANCEL); return TRUE; }
            if (LOWORD(wP) == IDC_FP_OPEN) {
                FileMan_DefaultExec(hwnd, s_szPath, NULL, NULL, SW_SHOWNORMAL);
                return TRUE;
            }
            break;
        }
    }
    return FALSE;
}

BOOL FileProp_ShowDialog(HINSTANCE hInst, HWND hwndParent, LPCSTR szPath)
{
    lstrcpyn(s_szPath, szPath, sizeof(s_szPath));
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_FILEPROP),
                     hwndParent, (DLGPROC)FilePropProc) == IDOK;
}
