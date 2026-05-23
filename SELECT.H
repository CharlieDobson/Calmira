/***************************************************************************
 *  SELECT.H - File selection dialog          (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef SELECT_H
#define SELECT_H
#include <windows.h>
/* Show a file-pattern select dialog.  Fills buf with chosen mask.
   Returns TRUE on OK. */
BOOL Select_Show(HINSTANCE hInst, HWND hwndParent, LPSTR buf, int bufLen);
#endif
