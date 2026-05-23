/***************************************************************************
 *  OPENFILE.H - "Open file with..." dialog   (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef OPENFILE_H
#define OPENFILE_H
#include <windows.h>
/* Show "Open with" dialog.  Fills szProg with chosen executable.
   Returns TRUE on OK. */
BOOL OpenFile_Show(HINSTANCE hInst, HWND hwndParent,
                   LPCSTR szDocPath, LPSTR szProg, int progLen);
#endif
