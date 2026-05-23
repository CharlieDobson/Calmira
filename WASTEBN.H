/***************************************************************************
 *
 *  WASTEBN.H - Recycle Bin window declarations
 *  Translated from WASTEBIN.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  The Bin window shows a list of files that have been moved to a
 *  temporary holding folder (g_szWritePath\BIN\).  Items can be
 *  restored to their original location or permanently deleted.
 *
 *  Bin item storage format (CALMIRA.INI [Bin Locations]):
 *    original_path=bin_temp_name
 *
 ***************************************************************************/

#ifndef WASTEBN_H
#define WASTEBN_H

#include <windows.h>
#include "SETTINGS.H"

/* Window class name */
#define BIN_CLASS   "CalmiraBin"

/* Sub-directory within the write path that holds binned files */
#define BIN_SUBDIR  "BIN\\"

/* ------------------------------------------------------------------
 * Bin item descriptor
 * ------------------------------------------------------------------ */
typedef struct tagBINITEM {
    char szOrigPath[144];   /* original full path before binning     */
    char szBinName[13];     /* 8.3 filename in the BIN\ subdirectory */
    BOOL bIsFolder;         /* TRUE if this was a directory          */
} BINITEM;

/* ------------------------------------------------------------------
 * Public interface
 * ------------------------------------------------------------------ */

BOOL Bin_RegisterClass(HINSTANCE hInst);
HWND Bin_Create(HINSTANCE hInst, HWND hwndParent);

/* Move a file/folder to the bin.  Returns TRUE on success. */
BOOL Bin_AddFile(HWND hwndBin, LPCSTR szSrcPath);

/* Restore a binned item to its original location */
BOOL Bin_RestoreItem(HWND hwndBin, int nIndex);

/* Permanently delete a binned item */
BOOL Bin_DeleteItem(HWND hwndBin, int nIndex);

/* Empty the entire bin */
void Bin_Empty(HWND hwndBin);

/* Refresh the display */
void Bin_Refresh(HWND hwndBin);

/* Return the number of items currently in the bin */
int  Bin_GetCount(HWND hwndBin);

/* Get the full path to the BIN\ subdirectory */
void Bin_GetDir(LPSTR buf, int bufLen);

/* Save / restore window position */
void Bin_SavePos(HWND hwndBin);
void Bin_LoadPos(HWND hwndBin);

/* Child window IDs */
#define IDW_BIN_LIST   1010

#endif /* WASTEBN_H */
