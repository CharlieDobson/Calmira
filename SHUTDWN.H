/***************************************************************************
 *  SHUTDWN.H - Shutdown / Quit dialog
 *  Translated from SHUTDOWN.PAS
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#ifndef SHUTDWN_H
#define SHUTDWN_H

#include <windows.h>

/* Return values from Shutdown_ShowDialog */
#define SD_CANCEL   0
#define SD_DOS      1   /* restart in MS-DOS mode     */
#define SD_WINDOWS  2   /* restart Windows            */
#define SD_REBOOT   3   /* reboot the computer        */
#define SD_QUIT     4   /* exit Windows               */

int  Shutdown_ShowDialog(HINSTANCE hInst, HWND hwndParent);
void Shutdown_Execute(int nAction, BOOL bShowBmp, HINSTANCE hInst);

#endif
