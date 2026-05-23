/***************************************************************************
 *  FOURDOS.H - 4DOS description file support
 *  Translated from FOURDOS.PAS
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#ifndef FOURDOS_H
#define FOURDOS_H
#include <windows.h>

/* Read description for szName from descript.ion in szDir.
   Returns TRUE and fills buf[bufLen] on success. */
BOOL FourDOS_GetDesc(LPCSTR szDir, LPCSTR szName, LPSTR buf, int bufLen);

/* Write (or update) description for szName in szDir's descript.ion. */
BOOL FourDOS_SetDesc(LPCSTR szDir, LPCSTR szName, LPCSTR szDesc);

/* Remove description for szName from szDir's descript.ion. */
BOOL FourDOS_RemoveDesc(LPCSTR szDir, LPCSTR szName);

/* Load all descriptions from szDir into an array.  Returns count. */
int  FourDOS_LoadAll(LPCSTR szDir, LPCSTR FAR * FAR *ppNames,
                     LPCSTR FAR * FAR *ppDescs);
#endif
