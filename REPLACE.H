/***************************************************************************
 *  REPLACE.H - File replace confirmation dialog (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef REPLACE_H
#define REPLACE_H
#include <windows.h>
/* Returns IDYES, IDNO, IDCANCEL, or IDYESALL */
#define IDYESALL  200
int Replace_Ask(HINSTANCE hInst, HWND hwndParent, LPCSTR szExisting,
                LPCSTR szNew);
#endif
