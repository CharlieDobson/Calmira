/***************************************************************************
 *  BINPROP.CPP - Recycle Bin Properties dialog
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "BINPROP.H"
#include "RESOURCE.H"
#include "SETTINGS.H"

BOOL CALLBACK __export BinPropProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_INITDIALOG: {
            SetDlgItemText(hwnd, IDC_BIN_CAPTION,  g_BinCaption);
            SetDlgItemInt(hwnd,  IDC_BIN_CAPACITY, g_BinCapacity, FALSE);
            CheckDlgButton(hwnd, IDC_BIN_ICONS,    g_BinIcons);
            CheckDlgButton(hwnd, IDC_BIN_DELTOBTN, g_DeleteToBin);
            return TRUE;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == IDOK) {
                GetDlgItemText(hwnd, IDC_BIN_CAPTION, g_BinCaption, sizeof(g_BinCaption));
                g_BinCapacity = GetDlgItemInt(hwnd, IDC_BIN_CAPACITY, NULL, FALSE);
                g_BinIcons    = IsDlgButtonChecked(hwnd, IDC_BIN_ICONS);
                g_DeleteToBin = IsDlgButtonChecked(hwnd, IDC_BIN_DELTOBTN);
                SaveBinProp();
                AnnounceSettingsChanged(scBin);
                EndDialog(hwnd, IDOK); return TRUE;
            }
            if (LOWORD(wParam) == IDCANCEL) { EndDialog(hwnd, IDCANCEL); return TRUE; }
            break;
        }
    }
    return FALSE;
}

BOOL BinProp_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_BINPROP), hwndParent, (DLGPROC)BinPropProc) == IDOK;
}
