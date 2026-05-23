/***************************************************************************
 *
 *  COMPSYS.H - Computer / File Manager window declarations
 *  Translated from COMPSYS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  The Computer window is Calmira's file manager.  It contains:
 *   - A directory tree pane (left, from TREE.CPP)
 *   - An icon/details view pane (right, from ICONWIN.CPP)
 *   - A toolbar (RESOURCE.H bitmaps)
 *   - A path combobox
 *   - A status bar
 *
 ***************************************************************************/

#ifndef COMPSYS_H
#define COMPSYS_H

#include <windows.h>
#include "SETTINGS.H"

/* Window class name */
#define COMPUTER_CLASS  "CalmiraComputer"

/* Minimum pane widths */
#define TREE_MIN_WIDTH  60
#define FILE_MIN_WIDTH  80

/* ------------------------------------------------------------------
 * Public interface
 * ------------------------------------------------------------------ */

BOOL Computer_RegisterClass(HINSTANCE hInst);
HWND Computer_Create(HINSTANCE hInst, HWND hwndParent);

/* Navigate to a new folder path.  path may have '*' prefix for
   Explorer-mode (show tree + files).  Called by CM_EXPLORER handler. */
void Computer_NavigateTo(HWND hwndComp, LPCSTR szPath);

/* Refresh the current folder view */
void Computer_Refresh(HWND hwndComp);

/* Get/set the current path */
void Computer_GetPath(HWND hwndComp, LPSTR buf, int bufLen);
void Computer_SetPath(HWND hwndComp, LPCSTR szPath);

/* Show the Computer window (called if RestoreSys is set) */
void Computer_ShowNormal(HWND hwndComp);

/* Save / restore window position from INI */
void Computer_SavePos(HWND hwndComp);
void Computer_LoadPos(HWND hwndComp);

/* Child window IDs are in RESOURCE.H */

/* WM_COMMAND messages from toolbar / menu handled by Computer_WndProc */

#endif /* COMPSYS_H */
