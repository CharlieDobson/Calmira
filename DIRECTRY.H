/***************************************************************************
 *
 *  DIRECTRY.H - Directory enumeration declarations
 *  Translated from DIRECTRY.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Wraps _dos_findfirst / _dos_findnext to enumerate directory contents
 *  into an array of ICONITEM structures, applying filter and sort.
 *
 ***************************************************************************/

#ifndef DIRECTRY_H
#define DIRECTRY_H

#include <windows.h>
#include <dos.h>
#include "ICONIC.H"
#include "SETTINGS.H"

/* ------------------------------------------------------------------
 * Enumerate a directory into an ICONITEM array.
 *
 * szPath    - full path of directory to list (e.g. "C:\WINDOWS\")
 * szFilter  - wildcard mask (e.g. "*.*")
 * bShowHid  - if TRUE include hidden/system files
 * aItems    - output array (caller allocates MAX_FILES elements)
 * pnItems   - receives the count of items stored
 * sortOrder - how to sort the result
 *
 * Returns the number of items or -1 on error.
 * ------------------------------------------------------------------ */
int Directory_Enum(LPCSTR szPath, LPCSTR szFilter, BOOL bShowHid,
                   ICONITEM FAR *aItems, int maxItems, int FAR *pnItems,
                   TSortOrder sortOrder);

/* ------------------------------------------------------------------
 * Get total size of all files in a directory (non-recursive).
 * Returns -1 on error.
 * ------------------------------------------------------------------ */
long Directory_Size(LPCSTR szPath);

/* ------------------------------------------------------------------
 * Count files matching szFilter in szPath (non-recursive).
 * ------------------------------------------------------------------ */
int Directory_Count(LPCSTR szPath, LPCSTR szFilter, BOOL bShowHid);

/* ------------------------------------------------------------------
 * Check whether a directory path exists and is accessible.
 * ------------------------------------------------------------------ */
BOOL Directory_Exists(LPCSTR szPath);

/* ------------------------------------------------------------------
 * Create all missing components of a multi-level path.
 * e.g. "C:\FOO\BAR\BAZ" creates FOO, BAR, and BAZ as needed.
 * ------------------------------------------------------------------ */
BOOL Directory_MakeTree(LPCSTR szPath);

/* ------------------------------------------------------------------
 * Build a full path from parent + name.
 * Ensures a trailing backslash on the parent before appending.
 * ------------------------------------------------------------------ */
void Directory_JoinPath(LPCSTR szParent, LPCSTR szName,
                        LPSTR szOut, int outLen);

/* ------------------------------------------------------------------
 * Extract drive letter string ("C:\") from a full path.
 * ------------------------------------------------------------------ */
void Directory_GetDrive(LPCSTR szPath, LPSTR szDrive);

/* ------------------------------------------------------------------
 * Extract parent directory (including trailing backslash).
 * ------------------------------------------------------------------ */
void Directory_GetParent(LPCSTR szPath, LPSTR szParent, int parentLen);

/* ------------------------------------------------------------------
 * Extract just the filename (no path) from a full path.
 * ------------------------------------------------------------------ */
LPCSTR Directory_GetName(LPCSTR szPath);

/* ------------------------------------------------------------------
 * Return pointer past the extension dot, or "" if no extension.
 * ------------------------------------------------------------------ */
LPCSTR Directory_GetExt(LPCSTR szName);

/* ------------------------------------------------------------------
 * Load 4DOS description file (descript.ion) for a directory.
 * Returns BOOL - FALSE if not found / disabled.
 * ------------------------------------------------------------------ */
BOOL Directory_LoadDescs(LPCSTR szPath, ICONITEM FAR *aItems, int nItems);

#endif /* DIRECTRY_H */
