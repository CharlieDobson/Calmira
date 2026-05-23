/***************************************************************************
 *
 *  FILEMAN.CPP - File management operations
 *  Translated from FILEMAN.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include <shellapi.h>
#include <dos.h>
#include <io.h>
#include <string.h>
#include <stdio.h>
#include "FILEMAN.H"
#include "SETTINGS.H"
#include "LOCALE.H"
#include "DIRECTRY.H"
#include "HELPERS.H"

BACKGROUNDPROC g_BackgroundProc = NULL;

void FileMan_BackgroundProcess(void)
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

/* -----------------------------------------------------------------------
 * Report a localised error message
 * ----------------------------------------------------------------------- */
static void ReportError(HWND hwnd, UINT sid, LPCSTR szArg)
{
    char fmt[256], msg[320];
    LoadString(g_hInst, sid, fmt, sizeof(fmt));
    wsprintf(msg, fmt, szArg ? szArg : "");
    MessageBox(hwnd, msg, "Calmira", MB_ICONEXCLAMATION | MB_OK);
}

/* -----------------------------------------------------------------------
 * Low-level file copy (no UI) - also used by WASTEBN.CPP
 * ----------------------------------------------------------------------- */
BOOL CopyFileRaw(LPCSTR szSrc, LPCSTR szDest)
{
    FILE *fSrc, *fDst;
    char  buf[4096];
    int   n;
    BOOL  ok = TRUE;

    fSrc = fopen(szSrc, "rb");
    if (!fSrc) {
        return FALSE;
    }
    fDst = fopen(szDest, "wb");
    if (!fDst) { fclose(fSrc); return FALSE; }

    while ((n = fread(buf, 1, sizeof(buf), fSrc)) > 0) {
        if ((int)fwrite(buf, 1, n, fDst) != n) { ok = FALSE; break; }
        if (g_BackgroundProc) {
            g_BackgroundProc();
        }
    }
    fclose(fSrc);
    fclose(fDst);
    if (!ok) {
        remove(szDest);
    }
    return ok;
}

/* -----------------------------------------------------------------------
 * Public: copy a single file
 * ----------------------------------------------------------------------- */
int FileMan_CopyFile(HWND hwndParent, LPCSTR szSrc, LPCSTR szDestDir,
                     BOOL bConfirm)
{
    char   szDest[144];
    LPCSTR szName = Directory_GetName(szSrc);

    Directory_JoinPath(szDestDir, szName, szDest, sizeof(szDest));

    /* Confirm overwrite if dest exists */
    if (bConfirm && _access(szDest, 0) == 0) {
        char msg[320], fmt[256];
        LoadString(g_hInst, SQueryReplaceFile, fmt, sizeof(fmt));
        wsprintf(msg, fmt, szName);
        if (MessageBox(hwndParent, msg, "Calmira",
                       MB_ICONQUESTION | MB_YESNO) != IDYES)
            return FM_SKIP;
    }

    if (!CopyFileRaw(szSrc, szDest)) {
        ReportError(hwndParent, SCannotReplaceFile, szName);
        return FM_ERROR;
    }
    return FM_OK;
}

/* -----------------------------------------------------------------------
 * Public: move a single file
 * ----------------------------------------------------------------------- */
int FileMan_MoveFile(HWND hwndParent, LPCSTR szSrc, LPCSTR szDestDir,
                     BOOL bConfirm)
{
    char szDest[144];
    LPCSTR szName = Directory_GetName(szSrc);

    Directory_JoinPath(szDestDir, szName, szDest, sizeof(szDest));

    if (bConfirm && _access(szDest, 0) == 0) {
        char msg[320], fmt[256];
        LoadString(g_hInst, SQueryReplaceFile, fmt, sizeof(fmt));
        wsprintf(msg, fmt, szName);
        if (MessageBox(hwndParent, msg, "Calmira",
                       MB_ICONQUESTION | MB_YESNO) != IDYES)
            return FM_SKIP;
    }

    /* Try rename first (same drive) */
    if (rename(szSrc, szDest) == 0) {
        return FM_OK;
    }

    /* Cross-drive: copy then delete */
    if (!CopyFileRaw(szSrc, szDest)) {
        ReportError(hwndParent, SCannotMoveFile, szName);
        return FM_ERROR;
    }
    if (remove(szSrc) != 0) {
        /* Copied OK but could not delete src - leave both */
        ReportError(hwndParent, SCannotDeleteFile, szSrc);
    }
    return FM_OK;
}

/* -----------------------------------------------------------------------
 * Public: delete a single file (or move to bin)
 * ----------------------------------------------------------------------- */
int FileMan_DeleteFile(HWND hwndParent, LPCSTR szPath, BOOL bToBin,
                       BOOL bConfirm)
{
    LPCSTR szName = Directory_GetName(szPath);

    if (bConfirm && g_ConfirmDelFile) {
        char msg[320], fmt[256];
        LoadString(g_hInst, SQueryDeleteItems, fmt, sizeof(fmt));
        wsprintf(msg, fmt, 1, "", szName);
        if (MessageBox(hwndParent, msg, "Calmira",
                       MB_ICONQUESTION | MB_YESNO) != IDYES)
            return FM_CANCEL;
    }

    if (bToBin && g_DeleteToBin) {
        /* Defer to the bin module */
        extern HWND g_hwBin;
        extern BOOL Bin_AddFile(HWND, LPCSTR);
        if (g_hwBin && Bin_AddFile(g_hwBin, szPath)) {
            return FM_OK;
        }
    }

    /* Check for read-only / system */
    {
        struct _find_t fd;
        if (_dos_findfirst(szPath, _A_NORMAL | _A_RDONLY | _A_HIDDEN | _A_SYSTEM, &fd) == 0) {
            if ((fd.attrib & (_A_RDONLY | _A_SYSTEM)) && g_ConfirmProtect) {
                char msg[320], fmt[256];
                LoadString(g_hInst, SQueryAffectProtected, fmt, sizeof(fmt));
                wsprintf(msg, fmt, szName, "delete");
                if (MessageBox(hwndParent, msg, "Calmira",
                               MB_ICONEXCLAMATION | MB_YESNO) != IDYES)
                    return FM_CANCEL;
                _dos_setfileattr(szPath, _A_NORMAL);
            }
        }
    }

    if (remove(szPath) != 0) {
        ReportError(hwndParent, SCannotDeleteFile, szName);
        return FM_ERROR;
    }
    return FM_OK;
}

/* -----------------------------------------------------------------------
 * Recursive directory copy / move / delete helpers
 * ----------------------------------------------------------------------- */
int FileMan_CopyDir(HWND hwndParent, LPCSTR szSrc, LPCSTR szDestDir,
                    BOOL bConfirm)
{
    struct _find_t fd;
    char  spec[144], destSub[144];
    LPCSTR szName = Directory_GetName(szSrc);
    int   rc = FM_OK;

    Directory_JoinPath(szDestDir, szName, destSub, sizeof(destSub));
    if (!Directory_MakeTree(destSub)) {
        ReportError(hwndParent, SCannotCreateFolder, szName);
        return FM_ERROR;
    }

    lstrcpy(spec, szSrc);
    lstrcat(spec, "\\*.*");
    if (_dos_findfirst(spec, _A_NORMAL | _A_RDONLY | _A_ARCH | _A_SUBDIR, &fd) == 0) {
        do {
            char childSrc[144];
            if (fd.name[0] == '.') {
                continue;
            }
            Directory_JoinPath(szSrc, fd.name, childSrc, sizeof(childSrc));
            if (fd.attrib & _A_SUBDIR) {
                rc = FileMan_CopyDir(hwndParent, childSrc, destSub, bConfirm);
            }
            else {
                rc = FileMan_CopyFile(hwndParent, childSrc, destSub, bConfirm);
            }
            if (rc == FM_CANCEL) {
                break;
            }
            if (g_BackgroundProc) {
                g_BackgroundProc();
            }
        } while (_dos_findnext(&fd) == 0);
    }
    return rc;
}

int FileMan_MoveDir(HWND hwndParent, LPCSTR szSrc, LPCSTR szDestDir,
                    BOOL bConfirm)
{
    /* Try rename first */
    char destSub[144];
    LPCSTR szName = Directory_GetName(szSrc);
    Directory_JoinPath(szDestDir, szName, destSub, sizeof(destSub));
    if (rename(szSrc, destSub) == 0) {
        return FM_OK;
    }
    /* Cross-drive: copy then delete source tree */
    {
        int rc = FileMan_CopyDir(hwndParent, szSrc, szDestDir, bConfirm);
        if (rc == FM_OK) {
            FileMan_DeleteDir(hwndParent, szSrc, FALSE, FALSE);
        }
        return rc;
    }
}

int FileMan_DeleteDir(HWND hwndParent, LPCSTR szPath, BOOL bToBin,
                      BOOL bConfirm)
{
    struct _find_t fd;
    char   spec[144];
    lstrcpy(spec, szPath); lstrcat(spec, "\\*.*");
    if (_dos_findfirst(spec, _A_NORMAL | _A_RDONLY | _A_ARCH | _A_SUBDIR |
                             _A_HIDDEN | _A_SYSTEM, &fd) == 0) {
        do {
            char child[144];
            if (fd.name[0] == '.') {
                continue;
            }
            Directory_JoinPath(szPath, fd.name, child, sizeof(child));
            if (fd.attrib & _A_SUBDIR) {
                FileMan_DeleteDir(hwndParent, child, bToBin, FALSE);
            }
            else {
                if (fd.attrib & (_A_RDONLY | _A_SYSTEM)) {
                    _dos_setfileattr(child, _A_NORMAL);
                }
                remove(child);
            }
            if (g_BackgroundProc) {
                g_BackgroundProc();
            }
        } while (_dos_findnext(&fd) == 0);
    }
    if (_rmdir(szPath) != 0) {
        ReportError(hwndParent, SCannotDeleteFile, szPath);
        return FM_ERROR;
    }
    return FM_OK;
}

BOOL FileMan_Rename(HWND hwndParent, LPCSTR szOldPath, LPCSTR szNewName)
{
    char szNew[144];
    Directory_GetParent(szOldPath, szNew, sizeof(szNew));
    lstrcat(szNew, szNewName);
    if (rename(szOldPath, szNew) != 0) {
        ReportError(hwndParent, SCannotRename, szOldPath);
        return FALSE;
    }
    return TRUE;
}

BOOL FileMan_MkDir(HWND hwndParent, LPCSTR szParent, LPCSTR szName)
{
    char szPath[144];
    Directory_JoinPath(szParent, szName, szPath, sizeof(szPath));
    if (_mkdir(szPath) != 0) {
        ReportError(hwndParent, SCannotCreateFolder, szName);
        return FALSE;
    }
    return TRUE;
}

/* -----------------------------------------------------------------------
 * Program execution
 * ----------------------------------------------------------------------- */

void FileMan_ReportExecError(HWND hwndParent, UINT uResult, LPCSTR szPath)
{
    static const UINT s_errIds[] = {
        0,                  /* 0 = success */
        SOutOfMemory,       /* 1 */
        SFileNotFound,      /* 2 */
        SFileNotFound,      /* 3 - path not found */
        SOutOfMemory,       /* 4 */
        SAccessDenied,      /* 5 */
        SInvalidEXE,        /* 6 */
        SInvalidEXE,        /* 7 */
        SInvalidEXE,        /* 8 */
        SInvalidEXE,        /* 9 */
        SInvalidEXE,        /* 10 */
        SInvalidEXE,        /* 11 */
        SInvalidEXE,        /* 12 */
        SInvalidEXE,        /* 13 */
        SNonWindowsApp,     /* 14 */
        SRequiresWin32,     /* 15 = Win32 app */
        SSecondInstance,    /* 16 */
        SInvalidEXE,        /* 17 */
        SInvalidEXE,        /* 18 */
        SCompressedEXE,     /* 19 */
        SAppOutOfMemory,    /* 20 */
    };
    UINT sid = (uResult < 21) ? s_errIds[uResult] : SCannotRunOrView;
    if (sid) {
        ReportError(hwndParent, sid, szPath);
    }
}

BOOL FileMan_Exec(HWND hwndParent, LPCSTR szPath, LPCSTR szParams,
                  LPCSTR szWorkDir, int nShow)
{
    UINT res = WinExec(szPath, nShow);
    if (res < 32) {
        FileMan_ReportExecError(hwndParent, res, szPath);
        return FALSE;
    }
    return TRUE;
}

BOOL FileMan_IsProgram(LPCSTR szExt)
{
    char lower[8];
    char search[12];
    lstrcpyn(lower, szExt, sizeof(lower));
    AnsiLower(lower);
    wsprintf(search, " %s ", lower);
    return lstrstr(g_IconStrings, search) != NULL  /* in IconStrings */
        || lstrstr(" com exe bat pif ", search) != NULL;
}

BOOL FileMan_DefaultExec(HWND hwndParent, LPCSTR szPath,
                          LPCSTR szParams, LPCSTR szWorkDir, int nShow)
{
    /* Try ShellExecute first (handles associations) */
    UINT res = (UINT)ShellExecute(hwndParent, "open",
                                   szPath, szParams,
                                   szWorkDir ? szWorkDir : "",
                                   nShow);
    if (res > 32) {
        return TRUE;
    }
    /* Fall back to WinExec */
    return FileMan_Exec(hwndParent, szPath, szParams, szWorkDir, nShow);
}
