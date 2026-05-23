/***************************************************************************
 *  SHORTS.H - Desktop shortcut icon window declarations
 *  Translated from SHORTS.PAS
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#ifndef SHORTS_H
#define SHORTS_H

#include <windows.h>
#include "DESK.H"

#define SHORT_CLASS  "CalmiraShort"

BOOL  Short_RegisterClass(HINSTANCE hInst);
HWND  Short_Create(HINSTANCE hInst, HWND hwndDesk,
                   const SHORTINFO FAR *psi);
void  Short_GetInfo(HWND hwndShort, SHORTINFO FAR *psi);
void  Short_SetInfo(HWND hwndShort, const SHORTINFO FAR *psi);
void  Short_Activate(HWND hwndShort);   /* open/run the target */

#endif
