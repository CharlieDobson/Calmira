/***************************************************************************
 *  LOGON.H - Calmira logon check
 *  Translated from LOGON.PAS
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#ifndef LOGON_H
#define LOGON_H
#include <windows.h>
/* Show logon dialog if MultiUser is enabled in CALMIRA.INI.
   Returns FALSE if logon was rejected (caller should exit). */
BOOL CheckForLogon(HINSTANCE hInst, HWND hwndParent);
#endif
