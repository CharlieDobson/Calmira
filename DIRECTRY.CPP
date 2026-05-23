/***************************************************************************
 *
 *  DIRECTRY.CPP - Directory enumeration
 *  Translated from DIRECTRY.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include "HELPERS.H"
#include "DIRECTRY.H"
#include "SETTINGS.H"
#include "FOURDOS.H"

/* -----------------------------------------------------------------------
 * Sort comparison functions
 * ----------------------------------------------------------------------- */
static int g_sortOrder;   /* set before qsort call */

static int FAR PASCAL CmpByName(const void FAR *a, const void FAR *b)
{
    const ICONITEM FAR *ia = (const ICONITEM FAR *)a;
    const ICONITEM FAR *ib = (const ICONITEM FAR *)b;
    /* Directories always sort first */
    if ((ia->bType & ITYPE_DIR) != (ib->bType & ITYPE_DIR)) {
        return (ia->bType & ITYPE_DIR) ? -1 : 1;
    }
    return lstrcmpi(ia->szName, ib->szName);
}

static int FAR PASCAL CmpByType(const void FAR *a, const void FAR *b)
{
    const ICONITEM FAR *ia = (const ICONITEM FAR *)a;
    const ICONITEM FAR *ib = (const ICONITEM FAR *)b;
    int r;
    if ((ia->bType & ITYPE_DIR) != (ib->bType & ITYPE_DIR)) {
        return (ia->bType & ITYPE_DIR) ? -1 : 1;
    }
    r = lstrcmpi(Directory_GetExt(ia->szName),
                 Directory_GetExt(ib->szName));
    return r ? r : lstrcmpi(ia->szName, ib->szName);
}

static int FAR PASCAL CmpBySize(const void FAR *a, const void FAR *b)
{
    const ICONITEM FAR *ia = (const ICONITEM FAR *)a;
    const ICONITEM FAR *ib = (const ICONITEM FAR *)b;
    if ((ia->bType & ITYPE_DIR) != (ib->bType & ITYPE_DIR)) {
        return (ia->bType & ITYPE_DIR) ? -1 : 1;
    }
    if (ia->dwSize < ib->dwSize) {
        return -1;
    }
    if (ia->dwSize > ib->dwSize) {
        return  1;
    }
    return lstrcmpi(ia->szName, ib->szName);
}

static int FAR PASCAL CmpByDate(const void FAR *a, const void FAR *b)
{
    const ICONITEM FAR *ia = (const ICONITEM FAR *)a;
    const ICONITEM FAR *ib = (const ICONITEM FAR *)b;
    DWORD da, db;
    if ((ia->bType & ITYPE_DIR) != (ib->bType & ITYPE_DIR)) {
        return (ia->bType & ITYPE_DIR) ? -1 : 1;
    }
    da = MAKELONG(ia->wTime, ia->wDate);
    db = MAKELONG(ib->wTime, ib->wDate);
    if (da < db) return  1;   /* newer = higher in list */
    if (da > db) {
        return -1;
    }
    return lstrcmpi(ia->szName, ib->szName);
}

/* -----------------------------------------------------------------------
 * Main enumeration
 * ----------------------------------------------------------------------- */
int Directory_Enum(LPCSTR szPath, LPCSTR szFilter, BOOL bShowHid,
                   ICONITEM FAR *aItems, int maxItems, int FAR *pnItems,
                   TSortOrder sortOrder)
{
    struct _find_t fd;
    char   szSpec[144];
    int    n = 0;
    UINT   attr;

    *pnItems = 0;

    /* Build "C:\PATH\*.EXT" search spec */
    lstrcpy(szSpec, szPath);
    if (szSpec[lstrlen(szSpec)-1] != '\\') {
        lstrcat(szSpec, "\\");
    }
    lstrcat(szSpec, szFilter);

    /* Enumerate files matching the filter */
    attr = _A_NORMAL | _A_RDONLY | _A_ARCH;
    if (bShowHid) {
        attr |= _A_HIDDEN | _A_SYSTEM;
    }

    if (_dos_findfirst(szSpec, attr, &fd) == 0) {
        do {
            ICONITEM FAR *pi;
            if (n >= maxItems) {
                break;
            }
            if (fd.attrib & _A_SUBDIR) continue;  /* dirs handled below */
            if (!bShowHid && (fd.attrib & (_A_HIDDEN | _A_SYSTEM))) {
                continue;
            }

            pi = &aItems[n++];
            _fmemset(pi, 0, sizeof(*pi));
            lstrcpyn(pi->szName, fd.name, sizeof(pi->szName));
            pi->dwSize = fd.size;
            pi->wDate  = fd.wr_date;
            pi->wTime  = fd.wr_time;
            pi->bAttr  = fd.attrib;
            pi->bType  = ITYPE_FILE;

            /* Detect alias extension */
            {
                LPCSTR ext = Directory_GetExt(fd.name);
                if (lstrcmpi(ext, g_AliasExtension + 1) == 0) {
                    pi->bType |= ITYPE_ALIAS;
                }
            }
        } while (_dos_findnext(&fd) == 0);
    }

    /* Now enumerate subdirectories using "C:\PATH\*.*" */
    {
        char szDirSpec[144];
        lstrcpy(szDirSpec, szPath);
        if (szDirSpec[lstrlen(szDirSpec)-1] != '\\') {
            lstrcat(szDirSpec, "\\");
        }
        lstrcat(szDirSpec, "*.*");

        if (_dos_findfirst(szDirSpec, _A_SUBDIR, &fd) == 0) {
            do {
                ICONITEM FAR *pi;
                if (!(fd.attrib & _A_SUBDIR)) {
                    continue;
                }
                if (fd.name[0] == '.') continue;  /* skip . and .. */
                if (!bShowHid && (fd.attrib & (_A_HIDDEN | _A_SYSTEM))) {
                    continue;
                }
                if (n >= maxItems) {
                    break;
                }

                pi = &aItems[n++];
                _fmemset(pi, 0, sizeof(*pi));
                lstrcpyn(pi->szName, fd.name, sizeof(pi->szName));
                pi->dwSize = 0;
                pi->wDate  = fd.wr_date;
                pi->wTime  = fd.wr_time;
                pi->bAttr  = fd.attrib;
                pi->bType  = ITYPE_DIR;
            } while (_dos_findnext(&fd) == 0);
        }
    }

    *pnItems = n;

    /* Sort */
    switch (sortOrder) {
        case soName: _far_qsort(aItems, n, sizeof(ICONITEM), CmpByName); break;
        case soType: _far_qsort(aItems, n, sizeof(ICONITEM), CmpByType); break;
        case soSize: _far_qsort(aItems, n, sizeof(ICONITEM), CmpBySize); break;
        case soDate: _far_qsort(aItems, n, sizeof(ICONITEM), CmpByDate); break;
    }
    return n;
}

long Directory_Size(LPCSTR szPath)
{
    struct _find_t fd;
    char   spec[144];
    long   total = 0;
    lstrcpy(spec, szPath);
    if (spec[lstrlen(spec)-1] != '\\') {
        lstrcat(spec, "\\");
    }
    lstrcat(spec, "*.*");
    if (_dos_findfirst(spec, _A_NORMAL | _A_RDONLY | _A_ARCH, &fd) == 0) {
        do {
            if (!(fd.attrib & _A_SUBDIR)) {
                total += fd.size;
            }
        } while (_dos_findnext(&fd) == 0);
    }
    return total;
}

int Directory_Count(LPCSTR szPath, LPCSTR szFilter, BOOL bShowHid)
{
    struct _find_t fd;
    char   spec[144];
    int    n = 0;
    UINT   attr = _A_NORMAL | _A_RDONLY | _A_ARCH;
    if (bShowHid) {
        attr |= _A_HIDDEN | _A_SYSTEM;
    }
    lstrcpy(spec, szPath);
    if (spec[lstrlen(spec)-1] != '\\') {
        lstrcat(spec, "\\");
    }
    lstrcat(spec, szFilter);
    if (_dos_findfirst(spec, attr, &fd) == 0) {
        do { if (!(fd.attrib & _A_SUBDIR)) n++; }
        while (_dos_findnext(&fd) == 0);
    }
    return n;
}

BOOL Directory_Exists(LPCSTR szPath)
{
    struct _find_t fd;
    char   spec[144];
    /* Check if it is a root "C:\" */
    if (szPath[1] == ':' && szPath[2] == '\\' && szPath[3] == '\0') {
        return TRUE;
    }
    lstrcpy(spec, szPath);
    {
        int len = lstrlen(spec);
        if (spec[len-1] == '\\') {
            spec[len-1] = '\0';
        }
    }
    return _dos_findfirst(spec, _A_SUBDIR, &fd) == 0 &&
           (fd.attrib & _A_SUBDIR);
}

BOOL Directory_MakeTree(LPCSTR szPath)
{
    char   buf[144];
    LPSTR  p;
    lstrcpy(buf, szPath);
    p = buf + 3;   /* skip "C:\" */
    while (*p) {
        if (*p == '\\') {
            *p = '\0';
            if (!Directory_Exists(buf)) {
                if (_mkdir(buf) != 0) {
                    return FALSE;
                }
            }
            *p = '\\';
        }
        p++;
    }
    if (!Directory_Exists(buf)) {
        return _mkdir(buf) == 0;
    }
    return TRUE;
}

void Directory_JoinPath(LPCSTR szParent, LPCSTR szName,
                        LPSTR szOut, int outLen)
{
    lstrcpyn(szOut, szParent, outLen);
    {
        int len = lstrlen(szOut);
        if (len > 0 && szOut[len-1] != '\\' && len < outLen - 1) {
            szOut[len]   = '\\';
            szOut[len+1] = '\0';
        }
    }
    {
        int rem = outLen - lstrlen(szOut) - 1;
        if (rem > 0) {
            lstrcatn(szOut, szName, rem);
        }
    }
}

void Directory_GetDrive(LPCSTR szPath, LPSTR szDrive)
{
    szDrive[0] = szPath[0];
    szDrive[1] = ':';
    szDrive[2] = '\\';
    szDrive[3] = '\0';
}

void Directory_GetParent(LPCSTR szPath, LPSTR szParent, int parentLen)
{
    LPSTR  p;
    int    len = lstrlen(szPath);
    lstrcpyn(szParent, szPath, parentLen);
    /* Remove trailing backslash if present */
    if (len > 3 && szParent[len-1] == '\\') {
        szParent[--len] = '\0';
    }
    /* Find last backslash */
    p = szParent + len;
    while (p > szParent + 2 && *p != '\\') p--;
    if (*p == '\\') {
        *(p+1) = '\0';
    }
}

LPCSTR Directory_GetName(LPCSTR szPath)
{
    LPCSTR p = szPath + lstrlen(szPath);
    while (p > szPath && *p != '\\' && *p != ':') p--;
    return (*p == '\\' || *p == ':') ? p + 1 : p;
}

LPCSTR Directory_GetExt(LPCSTR szName)
{
    LPCSTR dot = NULL;
    LPCSTR p   = szName;
    while (*p) {
        if (*p == '.') {
            dot = p;
        }
        p++;
    }
    return dot ? dot + 1 : p;   /* point past dot, or end of string */
}

BOOL Directory_LoadDescs(LPCSTR szPath, ICONITEM FAR *aItems, int nItems)
{
    if (!g_UseDescriptions) {
        return FALSE;
    }
    {
        char dir[144];
        int i;
        lstrcpy(dir, szPath);
        if (dir[lstrlen(dir)-1] != '\\') {
            lstrcat(dir, "\\");
        }
        for (i = 0; i < nItems; i++) {
            char desc[64];
            if (FourDOS_GetDesc(dir, aItems[i].szName, desc, sizeof(desc))) {
                lstrcpyn(aItems[i].szDesc, desc, sizeof(aItems[i].szDesc));
            }
        }
    }
    return TRUE;
}
