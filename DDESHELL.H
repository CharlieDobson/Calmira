/***************************************************************************
 *
 *  DDESHELL.H - Program Manager DDE shell interface
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Implements the PROGMAN DDE protocol so that Windows installers and
 *  setup programs can create program groups and items in Calmira's
 *  start menu (START.INI).
 *
 *  Supported commands:
 *    [CreateGroup(name[,path])]
 *    [ShowGroup(name,show_cmd)]
 *    [AddItem(cmd[,name[,icon_file[,icon_idx[,hotkey[,minimize[,work_dir]]]]]])]
 *    [ReplaceItem(name)]
 *    [DeleteItem(name)]
 *    [DeleteGroup(name)]
 *    [ExitProgman([save])]
 *    [Reload(name)]          (silently accepted)
 *
 *  The server window is g_hwMain (CALMIRA_CLASS).  Groups are stored as
 *  hierarchical sections under [Start\Programs\<name>] in START.INI.
 *
 ***************************************************************************/

#ifndef DDESHELL_H
#define DDESHELL_H

#include <windows.h>

/* Called from CalmiraWndProc for each incoming DDE message.
   All functions return TRUE when the message has been fully handled. */

/* WM_DDE_INITIATE: respond if service=="PROGMAN" and topic=="PROGMAN". */
BOOL DdeShell_OnInitiate(HWND hwndServer, HWND hwndClient,
                         ATOM aApp, ATOM aTopic);

/* WM_DDE_EXECUTE: parse and execute a command string from hData. */
BOOL DdeShell_OnExecute(HWND hwndServer, HWND hwndClient, HANDLE hData);

/* WM_DDE_TERMINATE: close conversation and echo back. */
void DdeShell_OnTerminate(HWND hwndServer, HWND hwndClient);

#endif /* DDESHELL_H */
