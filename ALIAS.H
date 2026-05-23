/***************************************************************************
 *  ALIAS.H - Alias (.ALS) file support
 *  Translated from ALIAS.PAS
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  An alias is a lightweight shortcut stored as an INI file with .ALS
 *  extension.  It stores target path, icon, parameters, and working dir.
 ***************************************************************************/
#ifndef ALIAS_H
#define ALIAS_H
#include <windows.h>

typedef struct tagALIASINFO {
    char Target[144];
    char Params[128];
    char WorkDir[144];
    char IconFile[144];
    int  IconIndex;
    int  ShowMode;
} ALIASINFO;

BOOL Alias_Read(LPCSTR szAliasPath, ALIASINFO FAR *pai);
BOOL Alias_Write(LPCSTR szAliasPath, const ALIASINFO FAR *pai);
BOOL Alias_Execute(HWND hwndParent, LPCSTR szAliasPath);
#endif
