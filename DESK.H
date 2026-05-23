/***************************************************************************
 *
 *  DESK.H - Desktop window declarations
 *  Translated from DESK.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  The Desktop is an invisible top-level window that:
 *   - Subclasses the Windows desktop to catch right-clicks (via WNDHOOKS)
 *   - Owns all desktop shortcut icon windows (TShort / SHORTS.CPP)
 *   - Implements cascade / tile / arrange / minimize-all operations
 *   - Saves / restores shortcut positions between sessions
 *
 ***************************************************************************/

#ifndef DESK_H
#define DESK_H

#include <windows.h>
#include "SETTINGS.H"

/* Window class name for the desktop */
#define DESK_CLASS   "CalmiraDesk"

/* Maximum desktop shortcuts */
#define MAX_SHORTS   64

/* ------------------------------------------------------------------
 * Shortcut kind codes (stored in CALMIRA.INI [ShortcutN] Kind=)
 * ------------------------------------------------------------------ */
#define SK_PROGRAM   1   /* executable / PIF */
#define SK_FOLDER    2   /* folder shortcut  */
#define SK_FILE      3   /* document file    */
#define SK_URL       4   /* internet URL     */

/* ------------------------------------------------------------------
 * Shortcut descriptor - mirrors [ShortcutN] INI section
 * ------------------------------------------------------------------ */
typedef struct tagSHORTINFO {
    int  Kind;
    char Target[144];
    char Caption[64];
    char IconFile[144];
    int  IconIndex;
    char Params[128];
    char WorkDir[144];
    int  ShowMode;
    int  Left;
    int  Top;
} SHORTINFO;

/* ------------------------------------------------------------------
 * Desktop public interface
 * ------------------------------------------------------------------ */

/* Register window class - call once at startup */
BOOL Desktop_RegisterClass(HINSTANCE hInst);

/* Create the desktop window.  Returns window handle or NULL. */
HWND Desktop_Create(HINSTANCE hInst, HWND hwndParent);

/* Load shortcuts from CALMIRA.INI */
void Desktop_Load(HWND hwndDesk);

/* Save shortcut positions to CALMIRA.INI */
void Desktop_Save(HWND hwndDesk);

/* Window arrangement operations */
void Desktop_CascadeBrowsers(HWND hwndDesk);
void Desktop_ArrangeTaskIcons(HWND hwndDesk);
void Desktop_LineUpIcons(HWND hwndDesk);
void Desktop_CloseBrowsers(HWND hwndDesk);
void Desktop_ClearDesktop(HWND hwndDesk);   /* minimize all shortcuts */
void Desktop_ToggleAll(HWND hwndDesk);       /* CM_TOGGLEDESKTOP */

/* Refresh all icon windows that show a given path */
void Desktop_RefreshPath(HWND hwndDesk, LPCSTR szPath);

/* Settings change notification */
void Desktop_OnSettingsChanged(HWND hwndDesk, TSettingChanges changes);

/* Add a shortcut to the desktop at (x, y) */
HWND Desktop_AddShortcut(HWND hwndDesk, const SHORTINFO FAR *psi);

/* Run the startup folder items (called during init) */
void StartMenu_RunStartup(HWND hwndDesk);

#endif /* DESK_H */
