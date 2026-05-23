/***************************************************************************
 *  REFERENC.H - Shortcut / alias reference editor  (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef REFERENC_H
#define REFERENC_H
#include <windows.h>
#include "DESK.H"
BOOL Referenc_EditShortcut(HINSTANCE hInst, HWND hwndParent, SHORTINFO FAR *psi);
#endif
