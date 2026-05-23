/***************************************************************************
 *
 *  START.H - Start menu declarations
 *  Translated from START.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  The Start menu is a hierarchical owner-drawn popup menu driven
 *  entirely by START.INI.
 *
 *  START.INI format (per section):
 *    [Start]
 *    Caption=command;workdir;showmode;iconfile;iconindex
 *
 *  Special values:
 *    Caption*   = folder (has a submenu)
 *    -n         = separator
 *    $Name      = built-in Calmira command (Run, ShutDown, Find, etc.)
 *    *nCaption* = appended to Control Panel level
 *
 ***************************************************************************/

#ifndef START_H
#define START_H

#include <windows.h>

/* Maximum levels of nesting */
#define START_MAX_DEPTH  8

/* Maximum items per menu level */
#define START_MAX_ITEMS  64

/* Maximum length of a start menu entry line */
#define START_LINE_MAX   256

/* ------------------------------------------------------------------
 * Built-in Calmira commands (value of the command field when
 * prefixed with '$' in START.INI)
 * ------------------------------------------------------------------ */
#define SC_RUN          "Run"
#define SC_SHUTDOWN     "ShutDown"
#define SC_FIND         "Find"
#define SC_HELP         "Help"
#define SC_EXPLORE      "Explore"
#define SC_SYSPROP      "SystemProp"
#define SC_DESKPROP     "DesktopProp"
#define SC_FILESYSPROP  "FileSystemProp"
#define SC_BINPROP      "BinProp"
#define SC_STARTPROP    "StartMenuProp"
#define SC_TASKPROP     "TaskbarProp"

/* ------------------------------------------------------------------
 * Start menu item descriptor (loaded from START.INI)
 * ------------------------------------------------------------------ */
typedef struct tagSTARTITEM {
    char Caption[64];     /* menu caption (& = accelerator)           */
    char Command[144];    /* program path or $builtin name            */
    char Params[144];     /* optional command-line parameters         */
    char WorkDir[144];    /* working directory                        */
    int  ShowMode;        /* SW_* constant for ShellExecute           */
    char IconFile[144];   /* icon source file ('' = extract from exe) */
    int  IconIndex;       /* index within icon file                   */
    BOOL bFolder;         /* TRUE if this item has a sub-menu         */
    BOOL bSep;            /* TRUE if this is a separator              */
    char SectName[128];   /* INI section for sub-menu (if bFolder)    */
} STARTITEM;

/* ------------------------------------------------------------------
 * Public interface
 * ------------------------------------------------------------------ */

/* Load the start menu from START.INI.  Call once at startup. */
void StartMenu_Load(HWND hwndOwner);

/* Show the start menu popup at the Start button location */
void StartMenu_Show(HWND hwndOwner, int x, int y);

/* Execute items in the Startup folder */
void StartMenu_RunStartup(HWND hwndOwner);

/* Reconfigure after settings change */
void StartMenu_Configure(HWND hwndOwner);

/* Execute a built-in Calmira command (called by start menu handler) */
void StartMenu_ExecBuiltin(HWND hwndOwner, LPCSTR szCmd);

/* Execute a program entry */
void StartMenu_ExecItem(HWND hwndOwner, const STARTITEM FAR *pItem);

#endif /* START_H */
