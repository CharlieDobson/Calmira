/***************************************************************************
 *  GOTOFLD.H - Go To Folder dialog            (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef GOTOFLD_H
#define GOTOFLD_H
#include <windows.h>
/* Shows Go To Folder dialog.  Returns TRUE and fills buf on OK. */
BOOL GotoFld_Show(HINSTANCE hInst, HWND hwndParent, LPSTR buf, int bufLen);
#endif
