/***************************************************************************
 *
 *  FOURDOS.CPP - 4DOS description file (descript.ion) support
 *  Translated from FOURDOS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Format of descript.ion:
 *    filename.ext Description text here\r\n
 *  Filenames with spaces are quoted.  One entry per line.
 *
 ***************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "FOURDOS.H"
#include "SETTINGS.H"

static void BuildDescPath(LPCSTR szDir, LPSTR szPath, int pathLen)
{
    lstrcpyn(szPath, szDir, pathLen);
    if (szPath[lstrlen(szPath)-1] != '\\') {
        lstrcat(szPath, "\\");
    }
    lstrcat(szPath, g_DescriptionFile);
}

/* Parse one line of descript.ion: extract filename and description */
static BOOL ParseLine(LPCSTR line, LPSTR szName, int nameLen,
                       LPSTR szDesc, int descLen)
{
    LPCSTR p = line;
    int    i = 0;
    /* Quoted filename? */
    if (*p == '"') {
        p++;
        while (*p && *p != '"' && i < nameLen - 1)
            szName[i++] = *p++;
        szName[i] = '\0';
        if (*p == '"') p++;
    } else {
        while (*p && *p != ' ' && *p != '\t' && i < nameLen - 1)
            szName[i++] = *p++;
        szName[i] = '\0';
    }
    /* Skip whitespace */
    while (*p == ' ' || *p == '\t') p++;
    /* Rest is description (strip trailing \r\n) */
    {
        int dlen = lstrlen(p);
        while (dlen > 0 && (p[dlen-1] == '\r' || p[dlen-1] == '\n'))
            dlen--;
        lstrcpyn(szDesc, p, descLen);
        szDesc[dlen < descLen ? dlen : descLen-1] = '\0';
    }
    return (*szName != '\0');
}

BOOL FourDOS_GetDesc(LPCSTR szDir, LPCSTR szName, LPSTR buf, int bufLen)
{
    char  path[144], line[512];
    char  fname[14], desc[256];
    FILE *f;

    BuildDescPath(szDir, path, sizeof(path));
    f = fopen(path, "r");
    if (!f) {
        return FALSE;
    }

    while (fgets(line, sizeof(line), f)) {
        if (ParseLine(line, fname, sizeof(fname), desc, sizeof(desc))) {
            if (lstrcmpi(fname, szName) == 0) {
                lstrcpyn(buf, desc, bufLen);
                fclose(f);
                return TRUE;
            }
        }
    }
    fclose(f);
    return FALSE;
}

BOOL FourDOS_SetDesc(LPCSTR szDir, LPCSTR szName, LPCSTR szDesc)
{
    char  path[144], tmpPath[144], line[512];
    char  fname[14], desc[256];
    FILE *fIn, *fOut;
    BOOL  written = FALSE;

    BuildDescPath(szDir, path, sizeof(path));
    lstrcpy(tmpPath, path);
    tmpPath[lstrlen(tmpPath)-1] = '~';   /* e.g. descript.io~ */

    fIn  = fopen(path,    "r");
    fOut = fopen(tmpPath, "w");
    if (!fOut) { if (fIn) fclose(fIn); return FALSE; }

    if (fIn) {
        while (fgets(line, sizeof(line), fIn)) {
            if (ParseLine(line, fname, sizeof(fname), desc, sizeof(desc))) {
                if (lstrcmpi(fname, szName) == 0) {
                    fprintf(fOut, "%s %s\r\n", szName, szDesc);
                    written = TRUE;
                    continue;
                }
            }
            fputs(line, fOut);
        }
        fclose(fIn);
    }
    if (!written && *szDesc) {
        fprintf(fOut, "%s %s\r\n", szName, szDesc);
    }
    fclose(fOut);

    remove(path);
    rename(tmpPath, path);
    return TRUE;
}

BOOL FourDOS_RemoveDesc(LPCSTR szDir, LPCSTR szName)
{
    return FourDOS_SetDesc(szDir, szName, "");
}

int FourDOS_LoadAll(LPCSTR szDir, LPCSTR FAR * FAR *ppNames,
                    LPCSTR FAR * FAR *ppDescs)
{
    /* Minimal stub - return 0 */
    *ppNames = NULL;
    *ppDescs = NULL;
    return 0;
}
