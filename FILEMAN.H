/***************************************************************************
 *
 *  FILEMAN.H - File management operations
 *  Translated from FILEMAN.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Provides high-level file operations (copy, move, delete) with
 *  progress dialog, error handling, and confirmation dialogs.
 *  Also wraps WinExec / ShellExecute for launching programs.
 *
 ***************************************************************************/

#ifndef FILEMAN_H
#define FILEMAN_H

#include <windows.h>
#include "SETTINGS.H"

/* ------------------------------------------------------------------
 * Operation result codes
 * ------------------------------------------------------------------ */
#define FM_OK       0   /* operation completed successfully */
#define FM_ERROR    1   /* non-fatal error (already reported) */
#define FM_CANCEL   2   /* user cancelled */
#define FM_SKIP     3   /* user chose to skip this item */

/* ------------------------------------------------------------------
 * File operation flags
 * ------------------------------------------------------------------ */
#define FOP_COPY    0x01
#define FOP_MOVE    0x02
#define FOP_DELETE  0x04
#define FOP_BIN     0x08   /* move to bin rather than delete */
#define FOP_CONFIRM 0x10   /* ask for confirmation first */

/* ------------------------------------------------------------------
 * Public interface
 * ------------------------------------------------------------------ */

/* Copy srcPath to destDir (a directory, not a filename) */
int  FileMan_CopyFile(HWND hwndParent, LPCSTR szSrc, LPCSTR szDestDir,
                      BOOL bConfirm);

/* Move srcPath to destDir */
int  FileMan_MoveFile(HWND hwndParent, LPCSTR szSrc, LPCSTR szDestDir,
                      BOOL bConfirm);

/* Delete a file (or move to bin if bToBin) */
int  FileMan_DeleteFile(HWND hwndParent, LPCSTR szPath, BOOL bToBin,
                        BOOL bConfirm);

/* Copy an entire directory tree */
int  FileMan_CopyDir(HWND hwndParent, LPCSTR szSrc, LPCSTR szDestDir,
                     BOOL bConfirm);

/* Move an entire directory tree */
int  FileMan_MoveDir(HWND hwndParent, LPCSTR szSrc, LPCSTR szDestDir,
                     BOOL bConfirm);

/* Delete an entire directory tree */
int  FileMan_DeleteDir(HWND hwndParent, LPCSTR szPath, BOOL bToBin,
                       BOOL bConfirm);

/* Rename a file or directory */
BOOL FileMan_Rename(HWND hwndParent, LPCSTR szOldPath, LPCSTR szNewName);

/* Create a directory */
BOOL FileMan_MkDir(HWND hwndParent, LPCSTR szParent, LPCSTR szName);

/* Execute a file using ShellExecute / WinExec.
   szVerb: "open", "print", etc. (NULL = default)
   Returns TRUE on success. */
BOOL FileMan_Exec(HWND hwndParent, LPCSTR szPath, LPCSTR szParams,
                  LPCSTR szWorkDir, int nShow);

/* Default execute: auto-detect verb from file type */
BOOL FileMan_DefaultExec(HWND hwndParent, LPCSTR szPath,
                          LPCSTR szParams, LPCSTR szWorkDir, int nShow);

/* Check if extension is in the Programs list (exe, com, bat, pif) */
BOOL FileMan_IsProgram(LPCSTR szExt);

/* Report a WinExec error with the appropriate message string */
void FileMan_ReportExecError(HWND hwndParent, UINT uResult, LPCSTR szPath);

/* Background processing - called during long operations to pump messages */
typedef void (CALLBACK *BACKGROUNDPROC)(void);
extern BACKGROUNDPROC g_BackgroundProc;
void FileMan_BackgroundProcess(void);

#endif /* FILEMAN_H */
