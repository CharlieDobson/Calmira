/***************************************************************************
 *  DYNAMENU.H - Dynamic (runtime-built) popup menu utilities
 *  Translated from DYNAMENU.PAS
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#ifndef DYNAMENU_H
#define DYNAMENU_H
#include <windows.h>

/* Append a string item to an HMENU */
void DynaMenu_Append(HMENU hmenu, UINT uId, LPCSTR szText);
/* Append a separator */
void DynaMenu_Sep(HMENU hmenu);
/* Append a submenu (takes ownership of hSub) */
void DynaMenu_AppendSub(HMENU hmenu, HMENU hSub, LPCSTR szText);
/* Track and destroy a popup, return selected ID (0 = none) */
UINT DynaMenu_Track(HMENU hmenu, HWND hwndOwner, int x, int y);
#endif
