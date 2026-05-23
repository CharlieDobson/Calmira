/***************************************************************************
 *  FILEFIND.H - Find Files dialog             (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef FILEFIND_H
#define FILEFIND_H

#include <windows.h>
/* Create (modeless) or show the existing Find Files dialog */
HWND FileFind_Create(HINSTANCE hInst, HWND hwndParent);
void FileFind_Show(HWND hwndFind);

#endif
