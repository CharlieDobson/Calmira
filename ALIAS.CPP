/***************************************************************************
 *
 *  ALIAS.CPP - Alias (.ALS) file support
 *  Translated from ALIAS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  An alias is a small INI-format file with extension .ALS that stores
 *  a shortcut to another file, folder, or URL.
 *
 ***************************************************************************/

#include <windows.h>
#include "ALIAS.H"
#include "SETTINGS.H"
#include "FILEMAN.H"

#define ALIAS_SECT "Alias"

BOOL Alias_Read(LPCSTR szAliasPath, ALIASINFO FAR *pai)
{
    GetPrivateProfileString(ALIAS_SECT, "Target",   "", pai->Target,   sizeof(pai->Target),   szAliasPath);
    GetPrivateProfileString(ALIAS_SECT, "Params",   "", pai->Params,   sizeof(pai->Params),   szAliasPath);
    GetPrivateProfileString(ALIAS_SECT, "WorkDir",  "", pai->WorkDir,  sizeof(pai->WorkDir),  szAliasPath);
    GetPrivateProfileString(ALIAS_SECT, "IconFile", "", pai->IconFile, sizeof(pai->IconFile), szAliasPath);
    pai->IconIndex = GetPrivateProfileInt(ALIAS_SECT, "IconIndex", 0,           szAliasPath);
    pai->ShowMode  = GetPrivateProfileInt(ALIAS_SECT, "ShowMode",  SW_SHOWNORMAL, szAliasPath);
    return (*pai->Target != '\0');
}

BOOL Alias_Write(LPCSTR szAliasPath, const ALIASINFO FAR *pai)
{
    char buf[8];
    WritePrivateProfileString(ALIAS_SECT, "Target",   pai->Target,   szAliasPath);
    WritePrivateProfileString(ALIAS_SECT, "Params",   pai->Params,   szAliasPath);
    WritePrivateProfileString(ALIAS_SECT, "WorkDir",  pai->WorkDir,  szAliasPath);
    WritePrivateProfileString(ALIAS_SECT, "IconFile", pai->IconFile, szAliasPath);
    wsprintf(buf, "%d", pai->IconIndex);
    WritePrivateProfileString(ALIAS_SECT, "IconIndex", buf, szAliasPath);
    wsprintf(buf, "%d", pai->ShowMode);
    WritePrivateProfileString(ALIAS_SECT, "ShowMode", buf, szAliasPath);
    return TRUE;
}

BOOL Alias_Execute(HWND hwndParent, LPCSTR szAliasPath)
{
    ALIASINFO ai;
    if (!Alias_Read(szAliasPath, &ai)) {
        return FALSE;
    }
    return FileMan_DefaultExec(hwndParent, ai.Target,
                                ai.Params[0] ? ai.Params : NULL,
                                ai.WorkDir[0] ? ai.WorkDir : NULL,
                                ai.ShowMode);
}
