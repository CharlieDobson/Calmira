/***************************************************************************
 *
 *  PROFILE.CPP - INI file utility functions
 *  Translated from VCL\PROFILE.PAS (Delphi TProfile class)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include "PROFILE.H"

void Profile_Open(PROFILE FAR *p, LPCSTR szPath)
{
    lstrcpyn(p->szPath, szPath, sizeof(p->szPath));
}

void Profile_Close(PROFILE FAR *p)
{
    /* Nothing to close for private profile functions; flush Windows cache */
    WritePrivateProfileString(NULL, NULL, NULL, p->szPath);
}

/* ------------------------------------------------------------------ */

void Profile_GetStr(const PROFILE FAR *p, LPCSTR sect, LPCSTR key,
                    LPCSTR def, LPSTR buf, int bufLen)
{
    GetPrivateProfileString(sect, key, def, buf, bufLen, p->szPath);
}

int Profile_GetInt(const PROFILE FAR *p, LPCSTR sect, LPCSTR key, int def)
{
    return GetPrivateProfileInt(sect, key, def, p->szPath);
}

BOOL Profile_GetBool(const PROFILE FAR *p, LPCSTR sect, LPCSTR key, BOOL def)
{
    char buf[8];
    GetPrivateProfileString(sect, key, def ? "1" : "0",
                            buf, sizeof(buf), p->szPath);
    /* Accept "1", "True", "Yes", "On" as TRUE (case-insensitive) */
    return (buf[0] == '1' || buf[0] == 't' || buf[0] == 'T' ||
            buf[0] == 'y' || buf[0] == 'Y');
}

/* Colors stored as hex BGR: "RRGGBB" (Windows COLORREF order) */
COLORREF Profile_GetColor(const PROFILE FAR *p, LPCSTR sect, LPCSTR key,
                           COLORREF def)
{
    char buf[12];
    COLORREF clr;
    char szDef[12];
    wsprintf(szDef, "%06lX", (DWORD)def);
    GetPrivateProfileString(sect, key, szDef, buf, sizeof(buf), p->szPath);
    if (sscanf(buf, "%lX", &clr) == 1) {
        return clr;
    }
    return def;
}

/* Font stored as "height,weight,italic,face" */
void Profile_GetFont(const PROFILE FAR *p, LPCSTR sect, LOGFONT FAR *plf)
{
    char buf[PROFILE_MAXVAL];
    int  ht = 0, wt = 400, it = 0;
    char face[LF_FACESIZE];

    GetPrivateProfileString(sect, "Font", "", buf, sizeof(buf), p->szPath);
    if (!*buf) {
        return;
    }

    face[0] = '\0';
    sscanf(buf, "%d,%d,%d,%31[^\n]", &ht, &wt, &it, face);

    plf->lfHeight         = (int)ht;
    plf->lfWidth          = 0;
    plf->lfEscapement     = 0;
    plf->lfOrientation    = 0;
    plf->lfWeight         = (int)wt;
    plf->lfItalic         = (BYTE)it;
    plf->lfUnderline      = 0;
    plf->lfStrikeOut      = 0;
    plf->lfCharSet        = ANSI_CHARSET;
    plf->lfOutPrecision   = OUT_DEFAULT_PRECIS;
    plf->lfClipPrecision  = CLIP_DEFAULT_PRECIS;
    plf->lfQuality        = DEFAULT_QUALITY;
    plf->lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
    if (*face) {
        lstrcpyn(plf->lfFaceName, face, LF_FACESIZE);
    }
}

/* ------------------------------------------------------------------ */

void Profile_SetStr(PROFILE FAR *p, LPCSTR sect, LPCSTR key, LPCSTR val)
{
    WritePrivateProfileString(sect, key, val, p->szPath);
}

void Profile_SetInt(PROFILE FAR *p, LPCSTR sect, LPCSTR key, int val)
{
    char buf[16];
    wsprintf(buf, "%d", val);
    WritePrivateProfileString(sect, key, buf, p->szPath);
}

void Profile_SetBool(PROFILE FAR *p, LPCSTR sect, LPCSTR key, BOOL val)
{
    WritePrivateProfileString(sect, key, val ? "1" : "0", p->szPath);
}

void Profile_SetColor(PROFILE FAR *p, LPCSTR sect, LPCSTR key, COLORREF clr)
{
    char buf[12];
    wsprintf(buf, "%06lX", (DWORD)clr);
    WritePrivateProfileString(sect, key, buf, p->szPath);
}

void Profile_SetFont(PROFILE FAR *p, LPCSTR sect, const LOGFONT FAR *plf)
{
    char buf[PROFILE_MAXVAL];
    wsprintf(buf, "%ld,%ld,%d,%s",
             plf->lfHeight, plf->lfWeight,
             (int)plf->lfItalic, plf->lfFaceName);
    WritePrivateProfileString(sect, "Font", buf, p->szPath);
}

/* ------------------------------------------------------------------ */

void Profile_GetSection(const PROFILE FAR *p, LPCSTR sect,
                         LPSTR buf, int bufLen)
{
    GetPrivateProfileString(sect, NULL, "", buf, bufLen, p->szPath);
}

void Profile_DeleteKey(PROFILE FAR *p, LPCSTR sect, LPCSTR key)
{
    WritePrivateProfileString(sect, key, NULL, p->szPath);
}

void Profile_DeleteSection(PROFILE FAR *p, LPCSTR sect)
{
    WritePrivateProfileString(sect, NULL, NULL, p->szPath);
}
