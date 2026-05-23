/***************************************************************************
 *  INCSRCH.H - Incremental search bar        (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef INCSRCH_H
#define INCSRCH_H
#include <windows.h>
HWND IncSrch_Create(HINSTANCE hInst, HWND hwndParent);
void IncSrch_SetText(HWND hwndIS, LPCSTR szText);
#endif
