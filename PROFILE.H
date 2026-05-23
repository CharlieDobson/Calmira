/***************************************************************************
 *
 *  PROFILE.H - INI file utility functions
 *  Translated from VCL\PROFILE.PAS (Delphi TProfile class)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Wraps GetPrivateProfileString / WritePrivateProfileString with
 *  helpers for booleans, integers, colors, and font descriptors.
 *
 ***************************************************************************/

#ifndef PROFILE_H
#define PROFILE_H

#include <windows.h>

/* Maximum string value length for INI reads */
#define PROFILE_MAXVAL  256

/* ------------------------------------------------------------------
 * Profile handle - holds the path to an open INI file
 * ------------------------------------------------------------------ */
typedef struct tagPROFILE {
    char szPath[144];    /* full path to the .INI file */
} PROFILE;

/* Open / close */
void    Profile_Open(PROFILE FAR *p, LPCSTR szPath);
void    Profile_Close(PROFILE FAR *p);

/* Reads */
void    Profile_GetStr(const PROFILE FAR *p, LPCSTR sect, LPCSTR key,
                       LPCSTR def, LPSTR buf, int bufLen);
int     Profile_GetInt(const PROFILE FAR *p, LPCSTR sect, LPCSTR key,
                       int def);
BOOL    Profile_GetBool(const PROFILE FAR *p, LPCSTR sect, LPCSTR key,
                        BOOL def);
COLORREF Profile_GetColor(const PROFILE FAR *p, LPCSTR sect, LPCSTR key,
                          COLORREF def);
void    Profile_GetFont(const PROFILE FAR *p, LPCSTR sect, LOGFONT FAR *plf);

/* Writes */
void    Profile_SetStr(PROFILE FAR *p, LPCSTR sect, LPCSTR key,
                       LPCSTR val);
void    Profile_SetInt(PROFILE FAR *p, LPCSTR sect, LPCSTR key, int val);
void    Profile_SetBool(PROFILE FAR *p, LPCSTR sect, LPCSTR key, BOOL val);
void    Profile_SetColor(PROFILE FAR *p, LPCSTR sect, LPCSTR key,
                         COLORREF clr);
void    Profile_SetFont(PROFILE FAR *p, LPCSTR sect, const LOGFONT FAR *plf);

/* Section utilities */
void    Profile_GetSection(const PROFILE FAR *p, LPCSTR sect,
                           LPSTR buf, int bufLen);
void    Profile_DeleteKey(PROFILE FAR *p, LPCSTR sect, LPCSTR key);
void    Profile_DeleteSection(PROFILE FAR *p, LPCSTR sect);

#endif /* PROFILE_H */
