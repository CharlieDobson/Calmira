/***************************************************************************
 *  MENUEDIT.CPP - Start Menu editor dialog stub
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#include <windows.h>
#include "MENUEDIT.H"
#include "RESOURCE.H"
#include "SETTINGS.H"

void MenuEdit_ShowDialog(HINSTANCE hInst, HWND hwndParent)
{
    /* Open START.INI in Notepad as a simple editor */
    char cmd[144];
    lstrcpy(cmd, "notepad.exe ");
    lstrcat(cmd, g_szStartFile);
    WinExec(cmd, SW_SHOW);
}
