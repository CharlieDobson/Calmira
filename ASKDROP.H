/***************************************************************************
 *  ASKDROP.H - "Ask before drop" confirmation dialog (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef ASKDROP_H
#define ASKDROP_H
#include <windows.h>
/* Returns IDCOPY, IDMOVE, or IDCANCEL */
#define IDCOPY   101
#define IDMOVE   102
int AskDrop_Show(HINSTANCE hInst, HWND hwndParent, int nFiles,
                 LPCSTR szSrc, LPCSTR szDest);
#endif
