/***************************************************************************
 *  SELECT.CPP - File selection dialog stub   (MSVC 1.52c / Win16)
 ***************************************************************************/
#include <windows.h>
#include "SELECT.H"

BOOL Select_Show(HINSTANCE hInst, HWND hwndParent, LPSTR buf, int bufLen)
{
    /* Simple input box */
    char tmp[84];
    lstrcpyn(tmp, buf, sizeof(tmp));
    if (MessageBox(hwndParent, "Enter file mask:", "Select Files",
                   MB_OKCANCEL) == IDOK) {
        lstrcpyn(buf, tmp[0] ? tmp : "*.*", bufLen);
        return TRUE;
    }
    return FALSE;
}
