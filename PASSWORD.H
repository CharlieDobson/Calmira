/***************************************************************************
 *  PASSWORD.H - Password / multi-user logon dialogs
 *  Translated from PASSWORD.PAS + LOGON.PAS
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#ifndef PASSWORD_H
#define PASSWORD_H
#include <windows.h>

/* Check if multi-user mode is enabled and show logon dialog.
   Returns FALSE if the user cancelled or was rejected. */
BOOL Password_CheckLogon(HINSTANCE hInst, HWND hwndParent);

/* Change password dialog.  Returns TRUE on success. */
BOOL Password_Change(HINSTANCE hInst, HWND hwndParent);

/* Verify a password against the stored hash for szUser. */
BOOL Password_Verify(LPCSTR szUser, LPCSTR szPassword);
#endif
