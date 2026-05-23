/***************************************************************************
 *
 *  ICONWIN.H - Icon window (file view pane) declarations
 *  Translated from ICONWIN.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  An icon window displays the contents of a directory in one of
 *  three views: Large Icons, Small Icons, or Details.
 *  It handles selection, drag-drop, keyboard navigation, and
 *  context menus for the displayed items.
 *
 ***************************************************************************/

#ifndef ICONWIN_H
#define ICONWIN_H

#include <windows.h>
#include "ICONIC.H"
#include "SETTINGS.H"

/* Window class name */
#define ICONWIN_CLASS   "CalmiraIconWin"

/* Maximum files in one directory.
   Limited to 512 so that ICONWINDATA (512 * 96 bytes + overhead) fits
   within a single 64 KB Win16 global memory block (GHND limit). */
#define MAX_FILES  512

/* ------------------------------------------------------------------
 * Icon window state (stored per-window in extra bytes or a map)
 * ------------------------------------------------------------------ */
typedef struct tagICONWINDATA {
    char         szPath[144];           /* current directory path        */
    ICONITEM     aItems[MAX_FILES];     /* directory contents            */
    int          nItems;                /* number of valid entries       */
    TDisplayMode mode;                  /* current view mode             */
    TSortOrder   sortOrder;             /* current sort                  */
    char         szFilter[84];          /* current file mask ("*.*")     */
    int          nScrollX;
    int          nScrollY;
    int          nSelected;             /* last clicked item index       */
    COLWIDTHS    colWidths;             /* detail column widths          */
} ICONWINDATA;

/* ------------------------------------------------------------------
 * Messages sent from icon window to its parent (Computer window)
 * ------------------------------------------------------------------ */
#define IWM_SELCHANGE    (WM_USER + 300)  /* selection changed  */
#define IWM_DBLCLICK     (WM_USER + 301)  /* item double-clicked */
#define IWM_PATHCHANGE   (WM_USER + 302)  /* navigate to wParam=atom */
#define IWM_STATUSUPDATE (WM_USER + 303)  /* status text changed */

/* ------------------------------------------------------------------
 * Public interface
 * ------------------------------------------------------------------ */

BOOL IconWin_RegisterClass(HINSTANCE hInst);

/* Create an icon window as a child of hwndParent */
HWND IconWin_Create(HINSTANCE hInst, HWND hwndParent, LPRECT prc);

/* Navigate to a folder */
void IconWin_SetPath(HWND hwndIW, LPCSTR szPath);

/* Get current path */
void IconWin_GetPath(HWND hwndIW, LPSTR buf, int bufLen);

/* Set display mode (large, small, details) */
void IconWin_SetMode(HWND hwndIW, TDisplayMode mode);

/* Set sort order */
void IconWin_SetSort(HWND hwndIW, TSortOrder order);

/* Set file filter mask */
void IconWin_SetFilter(HWND hwndIW, LPCSTR szMask);

/* Refresh directory listing */
void IconWin_Refresh(HWND hwndIW);

/* Selection operations */
void IconWin_SelectAll(HWND hwndIW);
void IconWin_InvertSel(HWND hwndIW);
int  IconWin_GetSelCount(HWND hwndIW);

/* Build a list of selected file paths (NUL-delimited, double-NUL term) */
int  IconWin_GetSelPaths(HWND hwndIW, LPSTR buf, int bufLen);

/* Get/set toggle for showing hidden/system files */
void IconWin_SetShowHidden(HWND hwndIW, BOOL bShow);

/* Recalculate default column widths from current font */
void IconWin_CalcColWidths(void);

#endif /* ICONWIN_H */
