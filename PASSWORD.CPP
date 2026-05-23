/***************************************************************************
 *  PASSWORD.CPP - Password / multi-user support stub
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "PASSWORD.H"
#include "SETTINGS.H"

BOOL Password_CheckLogon(HINSTANCE hInst, HWND hwndParent) { return TRUE; }
BOOL Password_Change(HINSTANCE hInst, HWND hwndParent)     { return TRUE; }
BOOL Password_Verify(LPCSTR szUser, LPCSTR szPwd)           { return TRUE; }
