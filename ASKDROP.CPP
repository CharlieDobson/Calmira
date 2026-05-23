/***************************************************************************
 *  ASKDROP.CPP - Drop confirmation dialog stub (MSVC 1.52c / Win16)
 ***************************************************************************/
#include <windows.h>
#include "ASKDROP.H"

int AskDrop_Show(HINSTANCE hInst, HWND hwndParent, int nFiles,
                 LPCSTR szSrc, LPCSTR szDest)
{
    char msg[256];
    wsprintf(msg, "Copy or Move %d item(s) from %s to %s?", nFiles, szSrc, szDest);
    switch (MessageBox(hwndParent, msg, "Calmira",
                       MB_ICONQUESTION | MB_YESNOCANCEL)) {
        case IDYES:    return IDCOPY;
        case IDNO:     return IDMOVE;
        default:       return IDCANCEL;
    }
}
