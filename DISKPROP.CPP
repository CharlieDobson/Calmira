/***************************************************************************
 *  DISKPROP.CPP - Disk Properties dialog
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include <dos.h>
#include "HELPERS.H"
#include "DISKPROP.H"
#include "RESOURCE.H"
#include "ICONIC.H"

static char s_szDrive[4];

BOOL CALLBACK __export DiskPropProc(HWND hwnd, UINT msg, WPARAM wP, LPARAM lP)
{
    switch (msg) {
        case WM_INITDIALOG: {
            struct diskfree_t df;
            char   label[12], buf[32];
            DWORD  total, free2, used;
            unsigned int driveNum = s_szDrive[0] - 'A' + 1;

            /* Volume label via DOS _A_VOLID (Win16 - no GetVolumeInformation) */
            {
                struct _find_t fd;
                char spec[8];
                wsprintf(spec, "%c:\\*.*", s_szDrive[0]);
                if (_dos_findfirst(spec, _A_VOLID, &fd) == 0 &&
                    (fd.attrib & _A_VOLID))
                    lstrcpyn(label, fd.name, sizeof(label));
                else {
                    label[0] = '\0';
                }
            }
            SetDlgItemText(hwnd, IDC_DP_LABEL, label);

            /* Disk space */
            if (_dos_getdiskfree(driveNum, &df) == 0) {
                total = (DWORD)df.total_clusters  * df.sectors_per_cluster * df.bytes_per_sector;
                free2 = (DWORD)df.avail_clusters  * df.sectors_per_cluster * df.bytes_per_sector;
                used  = total - free2;
                Iconic_FormatSize(total, buf, sizeof(buf)); SetDlgItemText(hwnd, IDC_DP_TOTAL, buf);
                Iconic_FormatSize(free2, buf, sizeof(buf)); SetDlgItemText(hwnd, IDC_DP_FREE, buf);
                Iconic_FormatSize(used,  buf, sizeof(buf)); SetDlgItemText(hwnd, IDC_DP_USED, buf);
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

BOOL DiskProp_ShowDialog(HINSTANCE hInst, HWND hwndParent, LPCSTR szDriveRoot)
{
    lstrcpyn(s_szDrive, szDriveRoot, sizeof(s_szDrive));
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DISKPROP),
                     hwndParent, (DLGPROC)DiskPropProc) == IDOK;
}
