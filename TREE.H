/***************************************************************************
 *
 *  TREE.H - Directory tree pane declarations
 *  Translated from TREE.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  The Tree pane shows a hierarchical view of drives and directories.
 *  Clicking a node navigates the adjacent icon window to that folder.
 *  Nodes can be expanded/collapsed with +/- buttons.
 *
 ***************************************************************************/

#ifndef TREE_H
#define TREE_H

#include <windows.h>

/* Window class name */
#define TREE_CLASS  "CalmiraTree"

/* Maximum tree nodes (drives + directories visible at once).
   Limited to 256 so TREEDATA (256 * 166 bytes + overhead) fits
   within a single 64 KB Win16 global memory block (GHND limit). */
#define TREE_MAX_NODES  256

/* ------------------------------------------------------------------
 * Tree node descriptor
 * ------------------------------------------------------------------ */
typedef struct tagTREENODE {
    char  szName[13];     /* directory name (no path)             */
    char  szFullPath[144];/* full path including drive and parents */
    int   nLevel;         /* nesting depth (0 = drive)            */
    BOOL  bExpanded;      /* children visible?                    */
    BOOL  bHasChildren;   /* FALSE when no sub-directories        */
    BOOL  bSelected;      /* TRUE when this is the current node   */
} TREENODE;

/* ------------------------------------------------------------------
 * Messages sent from tree to parent (Computer window)
 * ------------------------------------------------------------------ */
#define TWM_NAVIGATE  (WM_USER + 310)  /* user selected a folder */

/* ------------------------------------------------------------------
 * Public interface
 * ------------------------------------------------------------------ */

BOOL Tree_RegisterClass(HINSTANCE hInst);

/* Create the tree pane as a child of hwndParent */
HWND Tree_Create(HINSTANCE hInst, HWND hwndParent, LPRECT prc);

/* Expand / collapse to show a given path */
void Tree_SelectPath(HWND hwndTree, LPCSTR szPath);

/* Get the currently selected path */
void Tree_GetPath(HWND hwndTree, LPSTR buf, int bufLen);

/* Refresh the drive list (after net drive change etc.) */
void Tree_RefreshDrives(HWND hwndTree);

/* Refresh a single directory node (after rename / mkdir) */
void Tree_RefreshNode(HWND hwndTree, LPCSTR szPath);

#endif /* TREE_H */
