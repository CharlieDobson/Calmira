/***************************************************************************
 *  OPENFILE.CPP - "Open file with..." dialog (MSVC 1.52c / Win16)
 ***************************************************************************/
#include <windows.h>
#include <commdlg.h>
#include <memory.h>
#include <string.h>
#include "OPENFILE.H"
#include "RESOURCE.H"

BOOL OpenFile_Show(HINSTANCE hInst, HWND hwndParent,
                   LPCSTR szDocPath, LPSTR szProg, int progLen)
{
    OPENFILENAME ofn;
    szProg[0] = '\0';
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = hwndParent;
    ofn.lpstrFilter = "Programs\0*.exe;*.com;*.bat;*.pif\0All files\0*.*\0";
    ofn.lpstrFile   = szProg;
    ofn.nMaxFile    = progLen;
    ofn.Flags       = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrTitle  = "Open With";
    return GetOpenFileName(&ofn);
}
