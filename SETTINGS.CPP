/***************************************************************************
 *
 *  SETTINGS.CPP - Calmira application settings manager
 *  Translated from SETTINGS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Reads from and writes to CALMIRA.INI using the PROFILE helpers.
 *  All settings are defined (given storage) here as global variables.
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "SETTINGS.H"
#include "..\CALMSGS.H"
#include "HELPERS.H"

/* ======================================================================
 * Global variable definitions
 * ====================================================================== */

char  g_szAppPath[144]   = "";
char  g_szWinPath[144]   = "";
char  g_szWritePath[144] = "";
char  g_szStartFile[144] = "";

PROFILE g_Ini;

/* System */
char  g_ComputerCaption[80] = "Computer";
BOOL  g_ShowSplash      = TRUE;
BOOL  g_RestoreSys      = FALSE;
BOOL  g_SysWinQuit      = FALSE;
BOOL  g_TrackThumb      = TRUE;
BOOL  g_GlobalHotkeys   = TRUE;
BOOL  g_CompIconStart   = FALSE;
BOOL  g_EnableWinScripts= TRUE;
BOOL  g_EnableDosScripts= TRUE;
char  g_DosScriptFilename[144] = "";
char  g_WinScriptExtension[8]  = ".run";
BOOL  g_Win95Keyboard   = FALSE;
BOOL  g_ShowShutDownBMP = TRUE;
char  g_Browser[144]    = "";
BOOL  g_IsShell         = FALSE;
BOOL  g_FirstRun        = FALSE;
BOOL  g_LoadFromWinDir  = FALSE;
int   g_DoubleClickSpeed= 452;

/* File system */
TSortOrder   g_DefaultSort    = soName;
TDisplayMode g_DefaultDisplay = dmLargeIcons;
char  g_DefaultFilter[84]  = "*.*";
char  g_IconStrings[84]    = " exe ";
int   g_ListKBDecimals     = 0;
int   g_ListMBDecimals     = 2;
char  g_AliasExtension[8]  = ".als";
char  g_AliasExtUpper[8]   = ".ALS";

BOOL  g_FileHints          = TRUE;
int   g_HintDelay          = 800;
BOOL  g_UseDescriptions    = FALSE;
char  g_DescriptionFile[20]= "descript.ion";
BOOL  g_DescCaptions       = FALSE;
BOOL  g_Simul4DOS          = FALSE;
BOOL  g_FileSysChange      = TRUE;
BOOL  g_EnableLFN          = FALSE;

BOOL  g_ConfirmCopyStart   = FALSE;
BOOL  g_ConfirmMoveStart   = FALSE;
BOOL  g_ConfirmDelStart    = TRUE;
BOOL  g_ConfirmCopyFile    = FALSE;
BOOL  g_ConfirmMoveFile    = FALSE;
BOOL  g_ConfirmDelFile     = TRUE;
BOOL  g_ConfirmReplace     = TRUE;
BOOL  g_ConfirmProtect     = TRUE;
BOOL  g_ConfirmCopyFolder  = FALSE;
BOOL  g_ConfirmMoveFolder  = FALSE;
BOOL  g_ConfirmDelFolder   = TRUE;
BOOL  g_ConfirmNewAlias    = FALSE;

BOOL  g_ShowBrowserToolbar  = TRUE;
BOOL  g_ShowBrowserStatusbar= TRUE;
BOOL  g_FlatToolbarButtons  = FALSE;
BOOL  g_ShortWinCaptions    = TRUE;
BOOL  g_ShowHidSys          = FALSE;
BOOL  g_ProgDrop            = FALSE;
BOOL  g_FindDlgIcons        = TRUE;
BOOL  g_AliasArrows         = TRUE;
BOOL  g_DefDragCopy         = TRUE;
BOOL  g_DrawShadedIcons     = FALSE;
BOOL  g_MiniIcons           = TRUE;
BOOL  g_HQMiniIcons         = FALSE;
BOOL  g_UpcaseFirstChar     = TRUE;
BOOL  g_NoRegExtensions     = TRUE;

BOOL  g_HintDesc    = TRUE;
BOOL  g_HintBytes   = TRUE;
BOOL  g_HintType    = TRUE;
BOOL  g_HintDate    = TRUE;
BOOL  g_HintTime    = TRUE;
BOOL  g_HintAttrib  = FALSE;

TFileDetails g_DefaultColumns = fdALL;
int   g_FilePaneCols = 5;

char  g_InspectProg[144]  = "";
char  g_DefaultProg[144]  = "";
char  g_UndeleteProg[144] = "";
char  g_DiskProg[144]     = "";

/* Desktop */
TWindowOpen g_WindowOpen = woCascaded;
BOOL  g_SingleStatus       = FALSE;
BOOL  g_SaveWindows        = FALSE;
BOOL  g_ShortArrows        = TRUE;
BOOL  g_ShowDeskMenu       = TRUE;
BOOL  g_ExploreLastFolder  = FALSE;
BOOL  g_ConfirmDelShort    = TRUE;
BOOL  g_StickyShorts       = FALSE;
BOOL  g_OneClickShorts     = FALSE;
BOOL  g_BrowseSame         = FALSE;
BOOL  g_RightClose         = TRUE;
BOOL  g_RButtonUpClose     = FALSE;
BOOL  g_RunAutoClose       = TRUE;
BOOL  g_DesktopParent      = FALSE;
POINT g_DeskGrid           = { 75, 75 };
POINT g_BrowseGrid         = { 75, 65 };
int   g_LineHeight         = 16;
int   g_MinAppHeight       = 60;
int   g_DescWidth          = -1;
int   g_ColumnPadding      = 8;

/* Default Windows 95-style colors */
COLORREF g_Colors[ccCOUNT] = {
    RGB(192,192,192),  /* ccWinFrame      - clBtnFace    */
    RGB(255,255,255),  /* ccIconBack      - clWindow     */
    RGB(0,0,128),      /* ccIconSel       - clHighlight  */
    RGB(0,0,0),        /* ccShortArrow    - clBlack      */
    RGB(0,0,0),        /* ccAliasArrow    - clBlack      */
    RGB(0,0,128),      /* ccPercent       - clNavy       */
    RGB(192,192,192),  /* ccPercentBack   - clSilver     */
    RGB(192,192,192),  /* ccTaskbar       - clSilver     */
    RGB(192,192,192),  /* ccStartmenu     - clSilver     */
    RGB(0,0,128),      /* ccStartHighlight - clHighlight */
    RGB(192,192,192),  /* ccChecklist     - clSilver     */
};

/* Taskbar */
BOOL  g_DisableTaskbar  = FALSE;
BOOL  g_StayVisible     = FALSE;
BOOL  g_ShrinkMax       = TRUE;
BOOL  g_Clock24         = FALSE;
BOOL  g_PopupRes        = TRUE;
BOOL  g_PopupDate       = TRUE;
BOOL  g_Animate         = TRUE;
BOOL  g_FullAnimate     = TRUE;
BOOL  g_ButtonHints     = TRUE;
BOOL  g_ArrangeMin      = TRUE;
BOOL  g_HideMinApps     = TRUE;
BOOL  g_IconWindowTask  = TRUE;
BOOL  g_ExplorerTask    = TRUE;
BOOL  g_FullFolderPath  = FALSE;
BOOL  g_DocNameFirst    = FALSE;
BOOL  g_DocNameLower    = FALSE;
BOOL  g_ShowRealSysMenu = TRUE;
BOOL  g_TopTaskbar      = FALSE;
int   g_RowCount        = 1;
int   g_AnimatePixels   = 1;
int   g_AnimateSlowDown = 0;

/* Bin */
char      g_BinCaption[80] = "Bin";
TBinAction g_BinAction     = baLeave;
int       g_BinCapacity    = 8;
BOOL      g_BinIcons       = FALSE;
BOOL      g_DeleteToBin    = FALSE;

/* Start menu */
BOOL  g_StartMenu3D    = TRUE;
BOOL  g_ShellStartup   = TRUE;
BOOL  g_ShellDDE       = FALSE;
BOOL  g_BoldSelect     = TRUE;
BOOL  g_StartMouseUp   = TRUE;
BOOL  g_LargeRootMenu  = TRUE;
BOOL  g_ColouredBar    = TRUE;
int   g_LargeItemHeight = 34;

/* Display */
int     g_NormalHintPause = 800;
LOGFONT g_GlobalFont;

/* ======================================================================
 * Local helpers
 * ====================================================================== */

/* Expand %WINDIR%, %TEMP%, etc. in paths.
   Uses GetWindowsDirectory / GetTempPath for the common cases. */
void ExpandEnvPath(LPCSTR szSrc, LPSTR szDst, int dstLen)
{
    char winDir[144], tmpDir[144];
    LPCSTR p = szSrc;
    LPSTR  q = szDst;
    int    rem = dstLen - 1;
    char  *tmpEnv;

    GetWindowsDirectory(winDir, sizeof(winDir));
    tmpEnv = getenv("TEMP");
    if (!tmpEnv) {
        tmpEnv = getenv("TMP");
    }
    if (tmpEnv) {
        lstrcpy(tmpDir, tmpEnv);
    }
    else {
        lstrcpy(tmpDir, winDir);
    }

    while (*p && rem > 0) {
        if (*p == '%') {
            LPCSTR end = p + 1;
            while (*end && *end != '%') end++;
            if (*end == '%') {
                char var[32];
                int  varLen = (int)(end - p - 1);
                LPCSTR sub  = NULL;
                lstrcpyn(var, p + 1, varLen + 1);
                AnsiLower(var);
                if      (lstrcmp(var, "windir") == 0) {
                    sub = winDir;
                }
                else if (lstrcmp(var, "temp")   == 0) {
                    sub = tmpDir;
                }
                else if (lstrcmp(var, "tmp")    == 0) {
                    sub = tmpDir;
                }
                if (sub) {
                    int subLen = lstrlen(sub);
                    if (subLen > rem) {
                        subLen = rem;
                    }
                    lstrcpyn(q, sub, subLen + 1);
                    q   += subLen;
                    rem -= subLen;
                    p    = end + 1;
                    continue;
                }
            }
        }
        *q++ = *p++;
        rem--;
    }
    *q = '\0';
}

/* ======================================================================
 * Initialization
 * ====================================================================== */

void Settings_Init(void)
{
    char shellName[144], calName[144];

    /* Build path strings */
    GetModuleFileName(g_hInst, g_szAppPath, sizeof(g_szAppPath));
    {   /* strip filename, keep trailing backslash */
        char *p = g_szAppPath + lstrlen(g_szAppPath);
        while (p > g_szAppPath && *p != '\\' && *p != ':') p--;
        if (*p == '\\' || *p == ':') {
            *(p+1) = '\0';
        }
    }
    GetWindowsDirectory(g_szWinPath, sizeof(g_szWinPath));
    {
        int wlen = lstrlen(g_szWinPath);
        if (wlen > 0 && g_szWinPath[wlen-1] != '\\') {
            lstrcat(g_szWinPath, "\\");
        }
    }

    /* Detect if we are the shell */
    GetPrivateProfileString("boot", "shell", "progman.exe",
                            shellName, sizeof(shellName), "system.ini");
    GetModuleFileName(g_hInst, calName, sizeof(calName));
    AnsiLower(shellName); AnsiLower(calName);
    {   /* compare just the filename parts (case-insensitive) */
        char *s = shellName + lstrlen(shellName);
        char *c = calName   + lstrlen(calName);
        while (s > shellName && *s != '\\' && *s != ':') s--;
        while (c > calName   && *c != '\\' && *c != ':') c--;
        if (*s == '\\' || *s == ':') s++;
        if (*c == '\\' || *c == ':') c++;
        g_IsShell = (lstrcmpi(s, c) == 0);
    }

    /* Read LoadFromWinDir from win.ini */
    g_LoadFromWinDir = (BOOL)GetProfileInt("Calmira", "LoadFromWinDir", 0);
    g_DoubleClickSpeed = (int)GetProfileInt("Windows", "DoubleClickSpeed", 452);

    /* Determine write path for CALMIRA.INI */
    lstrcpy(g_szWritePath, g_LoadFromWinDir ? g_szWinPath : g_szAppPath);
    lstrcpy(g_szStartFile, g_szWritePath);
    lstrcat(g_szStartFile, "start.ini");

    /* Open the INI profile */
    {
        char iniPath[144];
        lstrcpy(iniPath, g_szWritePath);
        lstrcat(iniPath, "calmira.ini");
        Profile_Open(&g_Ini, iniPath);
    }

    /* Default font: stock System font parameters */
    memset(&g_GlobalFont, 0, sizeof(g_GlobalFont));
    g_GlobalFont.lfHeight = -12;
    g_GlobalFont.lfWeight = FW_NORMAL;
    lstrcpy(g_GlobalFont.lfFaceName, "MS Sans Serif");
}

void Settings_Done(void)
{
    Profile_Close(&g_Ini);
}

/* ======================================================================
 * Load / Save individual sections
 * ====================================================================== */

void LoadSystemProp(void)
{
    char buf[144];
    Profile_GetStr(&g_Ini, "Computer", "Caption",
                   "Computer", g_ComputerCaption, sizeof(g_ComputerCaption));
    g_ShowSplash      = Profile_GetBool(&g_Ini, "Preferences", "ShowSplash", TRUE);
    g_RestoreSys      = Profile_GetBool(&g_Ini, "Preferences", "RestoreSys", FALSE);
    g_SysWinQuit      = Profile_GetBool(&g_Ini, "Preferences", "SysWinQuit", FALSE);
    g_TrackThumb      = Profile_GetBool(&g_Ini, "Preferences", "TrackThumb", TRUE);
    g_GlobalHotkeys   = Profile_GetBool(&g_Ini, "Preferences", "GlobalHotKeys", TRUE);
    g_CompIconStart   = Profile_GetBool(&g_Ini, "Preferences", "CompIconStart", FALSE);
    g_ShowShutDownBMP = Profile_GetBool(&g_Ini, "Preferences", "ShowShutDownBMP", TRUE);
    g_Win95Keyboard   = Profile_GetBool(&g_Ini, "Preferences", "Win95Keyboard", FALSE);
    g_EnableDosScripts= Profile_GetBool(&g_Ini, "Scripts", "EnableDosScripts", TRUE);
    g_EnableWinScripts= Profile_GetBool(&g_Ini, "Scripts", "EnableWinScripts", TRUE);
    Profile_GetStr(&g_Ini, "Internet", "Browser", "", g_Browser, sizeof(g_Browser));
    Profile_GetStr(&g_Ini, "Scripts", "WinScriptExtension",
                   ".run", g_WinScriptExtension, sizeof(g_WinScriptExtension));
    Profile_GetStr(&g_Ini, "Scripts", "DosScriptFilename",
                   "%temp%\\calmira.run", buf, sizeof(buf));
    ExpandEnvPath(buf, g_DosScriptFilename, sizeof(g_DosScriptFilename));
}

void SaveSystemProp(void)
{
    Profile_SetStr(&g_Ini,  "Computer",    "Caption",         g_ComputerCaption);
    Profile_SetBool(&g_Ini, "Preferences", "ShowSplash",      g_ShowSplash);
    Profile_SetBool(&g_Ini, "Preferences", "RestoreSys",      g_RestoreSys);
    Profile_SetBool(&g_Ini, "Preferences", "SysWinQuit",      g_SysWinQuit);
    Profile_SetBool(&g_Ini, "Preferences", "TrackThumb",      g_TrackThumb);
    Profile_SetBool(&g_Ini, "Preferences", "GlobalHotKeys",   g_GlobalHotkeys);
    Profile_SetBool(&g_Ini, "Preferences", "CompIconStart",   g_CompIconStart);
    Profile_SetBool(&g_Ini, "Preferences", "ShowShutDownBMP", g_ShowShutDownBMP);
    Profile_SetBool(&g_Ini, "Preferences", "Win95Keyboard",   g_Win95Keyboard);
    Profile_SetBool(&g_Ini, "Scripts",     "EnableDosScripts",g_EnableDosScripts);
    Profile_SetBool(&g_Ini, "Scripts",     "EnableWinScripts",g_EnableWinScripts);
    Profile_SetStr(&g_Ini,  "Internet",    "Browser",         g_Browser);
}

void LoadDeskProp(void)
{
    int c;
    static const COLORREF defColors[ccCOUNT] = {
        RGB(192,192,192), RGB(255,255,255), RGB(0,0,128),    RGB(0,0,0),
        RGB(0,0,0),       RGB(0,0,128),     RGB(192,192,192), RGB(192,192,192),
        RGB(192,192,192), RGB(0,0,128),     RGB(192,192,192)
    };
    static const char *colorNames[ccCOUNT] = {
        "Window frames", "Icon background", "Icon selection", "Shortcut arrows",
        "Alias arrows", "Percent bar", "Percent back", "Taskbar", "Startmenu",
        "Start highlight", "Checklist"
    };

    g_SingleStatus      = Profile_GetBool(&g_Ini, "Preferences", "SingleStatus",      FALSE);
    g_SaveWindows       = Profile_GetBool(&g_Ini, "Preferences", "SaveWindows",       FALSE);
    g_ShortArrows       = Profile_GetBool(&g_Ini, "Preferences", "ShortArrows",       TRUE);
    g_ShowDeskMenu      = Profile_GetBool(&g_Ini, "Preferences", "ShowDeskMenu",      TRUE);
    g_ExploreLastFolder = Profile_GetBool(&g_Ini, "Preferences", "ExploreLastFolder", FALSE);
    g_StickyShorts      = Profile_GetBool(&g_Ini, "Preferences", "StickyShorts",      FALSE);
    g_OneClickShorts    = Profile_GetBool(&g_Ini, "Preferences", "OneClickShorts",    FALSE);
    g_BrowseSame        = Profile_GetBool(&g_Ini, "Preferences", "BrowseSame",        FALSE);
    g_RightClose        = Profile_GetBool(&g_Ini, "Preferences", "RightClose",        TRUE);
    g_RButtonUpClose    = Profile_GetBool(&g_Ini, "Preferences", "RButtonUpClose",    FALSE);
    g_ConfirmDelShort   = Profile_GetBool(&g_Ini, "Confirmation","DelShort",          TRUE);
    g_RunAutoClose      = Profile_GetBool(&g_Ini, "Preferences", "RunAutoClose",      TRUE);
    g_DesktopParent     = Profile_GetBool(&g_Ini, "Preferences", "DesktopParent",     FALSE);
    g_WindowOpen  = (TWindowOpen)Profile_GetInt(&g_Ini, "Preferences", "WindowOpen", 0);
    g_DeskGrid.x  = Profile_GetInt(&g_Ini, "Desktop",  "DeskGridX",  75);
    g_DeskGrid.y  = Profile_GetInt(&g_Ini, "Desktop",  "DeskGridY",  75);
    g_BrowseGrid.x= Profile_GetInt(&g_Ini, "Display",  "BrowseGridX",75);
    g_BrowseGrid.y= Profile_GetInt(&g_Ini, "Display",  "BrowseGridY",65);
    g_LineHeight  = Profile_GetInt(&g_Ini, "Display",  "LineHeight",  16);
    for (c = 0; c < ccCOUNT; c++)
        g_Colors[c] = Profile_GetColor(&g_Ini, "Colors", colorNames[c], defColors[c]);
    Profile_GetFont(&g_Ini, "Display", &g_GlobalFont);
}

void SaveDeskProp(void)
{
    int c;
    static const char *colorNames[ccCOUNT] = {
        "Window frames", "Icon background", "Icon selection", "Shortcut arrows",
        "Alias arrows", "Percent bar", "Percent back", "Taskbar", "Startmenu",
        "Start highlight", "Checklist"
    };
    Profile_SetBool(&g_Ini, "Preferences", "SingleStatus",      g_SingleStatus);
    Profile_SetBool(&g_Ini, "Preferences", "SaveWindows",       g_SaveWindows);
    Profile_SetBool(&g_Ini, "Preferences", "ShortArrows",       g_ShortArrows);
    Profile_SetBool(&g_Ini, "Preferences", "ShowDeskMenu",      g_ShowDeskMenu);
    Profile_SetBool(&g_Ini, "Preferences", "ExploreLastFolder", g_ExploreLastFolder);
    Profile_SetBool(&g_Ini, "Preferences", "StickyShorts",      g_StickyShorts);
    Profile_SetBool(&g_Ini, "Preferences", "OneClickShorts",    g_OneClickShorts);
    Profile_SetBool(&g_Ini, "Preferences", "BrowseSame",        g_BrowseSame);
    Profile_SetBool(&g_Ini, "Preferences", "RightClose",        g_RightClose);
    Profile_SetBool(&g_Ini, "Preferences", "RButtonUpClose",    g_RButtonUpClose);
    Profile_SetBool(&g_Ini, "Confirmation","DelShort",           g_ConfirmDelShort);
    Profile_SetBool(&g_Ini, "Preferences", "RunAutoClose",      g_RunAutoClose);
    Profile_SetBool(&g_Ini, "Preferences", "DesktopParent",     g_DesktopParent);
    Profile_SetInt(&g_Ini,  "Preferences", "WindowOpen",        (int)g_WindowOpen);
    Profile_SetInt(&g_Ini,  "Desktop",     "DeskGridX",         g_DeskGrid.x);
    Profile_SetInt(&g_Ini,  "Desktop",     "DeskGridY",         g_DeskGrid.y);
    Profile_SetInt(&g_Ini,  "Display",     "BrowseGridX",       g_BrowseGrid.x);
    Profile_SetInt(&g_Ini,  "Display",     "BrowseGridY",       g_BrowseGrid.y);
    Profile_SetInt(&g_Ini,  "Display",     "LineHeight",        g_LineHeight);
    for (c = 0; c < ccCOUNT; c++)
        Profile_SetColor(&g_Ini, "Colors", colorNames[c], g_Colors[c]);
}

void LoadFileSysProp(void)
{
    char buf[84];
    g_DefaultDisplay   = (TDisplayMode)Profile_GetInt(&g_Ini, "File system", "DefaultDisplay",   0);
    g_DefaultSort      = (TSortOrder)  Profile_GetInt(&g_Ini, "File system", "DefaultSort",      0);
    Profile_GetStr(&g_Ini, "File system", "IconStrings", "exe", buf, sizeof(buf));
    wsprintf(g_IconStrings, " %s ", buf);
    AnsiLower(g_IconStrings);
    Profile_GetStr(&g_Ini, "File system ", "DefaultFilter", "*.*",
                   g_DefaultFilter, sizeof(g_DefaultFilter));
    g_FileHints        = Profile_GetBool(&g_Ini, "File system ", "FileHints",        TRUE);
    g_HintDelay        = Profile_GetInt(&g_Ini,  "File system ", "HintDelay",        800);
    g_ListKBDecimals   = Profile_GetInt(&g_Ini,  "File system",  "ListKBDecimals",   0);
    g_ListMBDecimals   = Profile_GetInt(&g_Ini,  "File system",  "ListMBDecimals",   2);
    g_UseDescriptions  = Profile_GetBool(&g_Ini, "File system",  "UseDescriptions",  FALSE);
    Profile_GetStr(&g_Ini, "File system",  "DescriptionFile",
                   "descript.ion", g_DescriptionFile, sizeof(g_DescriptionFile));
    g_DescCaptions     = Profile_GetBool(&g_Ini, "File system",  "DescCaptions",     FALSE);
    g_Simul4DOS        = Profile_GetBool(&g_Ini, "File system",  "Simul4DOS",        FALSE);
    g_FileSysChange    = Profile_GetBool(&g_Ini, "File system",  "FileSysChange",    TRUE);
    g_EnableLFN        = Profile_GetBool(&g_Ini, "File system",  "EnableLFN",        FALSE);
    g_ConfirmCopyStart = Profile_GetBool(&g_Ini, "Confirmation", "CopyStart",        FALSE);
    g_ConfirmMoveStart = Profile_GetBool(&g_Ini, "Confirmation", "MoveStart",        FALSE);
    g_ConfirmDelStart  = Profile_GetBool(&g_Ini, "Confirmation", "DelStart",         TRUE);
    g_ConfirmCopyFile  = Profile_GetBool(&g_Ini, "Confirmation", "CopyFile",         FALSE);
    g_ConfirmMoveFile  = Profile_GetBool(&g_Ini, "Confirmation", "MoveFile",         FALSE);
    g_ConfirmDelFile   = Profile_GetBool(&g_Ini, "Confirmation", "DelFile",          TRUE);
    g_ConfirmReplace   = Profile_GetBool(&g_Ini, "Confirmation", "Replace",          TRUE);
    g_ConfirmProtect   = Profile_GetBool(&g_Ini, "Confirmation", "Protect",          TRUE);
    g_ConfirmCopyFolder= Profile_GetBool(&g_Ini, "Confirmation", "CopyFolder",       FALSE);
    g_ConfirmMoveFolder= Profile_GetBool(&g_Ini, "Confirmation", "MoveFolder",       FALSE);
    g_ConfirmDelFolder = Profile_GetBool(&g_Ini, "Confirmation", "DelFolder",        TRUE);
    g_ConfirmNewAlias  = Profile_GetBool(&g_Ini, "Confirmation", "NewAlias",         FALSE);
    g_ShowBrowserToolbar  = Profile_GetBool(&g_Ini, "Preferences", "ShowBrowserToolbar",  TRUE);
    g_ShowBrowserStatusbar= Profile_GetBool(&g_Ini, "Preferences", "ShowBrowserStatusbar",TRUE);
    g_FlatToolbarButtons  = Profile_GetBool(&g_Ini, "Preferences", "FlatToolbarButtons",  FALSE);
    g_ShortWinCaptions = Profile_GetBool(&g_Ini, "Preferences", "ShortWinCaptions",  TRUE);
    g_ShowHidSys       = Profile_GetBool(&g_Ini, "Preferences", "ShowHidSys",        FALSE);
    g_ProgDrop         = Profile_GetBool(&g_Ini, "Preferences", "ProgDrop",          FALSE);
    g_FindDlgIcons     = Profile_GetBool(&g_Ini, "Preferences", "FindDlgIcons",      TRUE);
    g_AliasArrows      = Profile_GetBool(&g_Ini, "Preferences", "AliasArrows",       TRUE);
    g_DefDragCopy      = Profile_GetBool(&g_Ini, "Preferences", "DefDragCopy",       TRUE);
    g_DrawShadedIcons  = Profile_GetBool(&g_Ini, "Preferences", "DrawShadedIcons",   FALSE);
    g_MiniIcons        = Profile_GetBool(&g_Ini, "Preferences", "MiniIcons",         TRUE);
    g_HQMiniIcons      = Profile_GetBool(&g_Ini, "Preferences", "HQMiniIcons",       FALSE);
    g_UpcaseFirstChar  = Profile_GetBool(&g_Ini, "Preferences", "UpcaseFirstChar",   TRUE);
    g_NoRegExtensions  = Profile_GetBool(&g_Ini, "Preferences", "NoRegExtensions",   TRUE);
    g_HintDesc         = Profile_GetBool(&g_Ini, "Preferences", "HintDesc",          TRUE);
    g_HintBytes        = Profile_GetBool(&g_Ini, "Preferences", "HintBytes",         TRUE);
    g_HintType         = Profile_GetBool(&g_Ini, "Preferences", "HintType",          TRUE);
    g_HintDate         = Profile_GetBool(&g_Ini, "Preferences", "HintDate",          TRUE);
    g_HintTime         = Profile_GetBool(&g_Ini, "Preferences", "HintTime",          TRUE);
    g_HintAttrib       = Profile_GetBool(&g_Ini, "Preferences", "HintAttrib",        FALSE);
    g_DefaultColumns   = (TFileDetails)Profile_GetInt(&g_Ini, "Preferences", "DefaultColumns", fdALL);
    g_FilePaneCols     = Profile_GetInt(&g_Ini,  "File system", "FilePaneCols",      5);
    g_DescWidth        = Profile_GetInt(&g_Ini,  "File system", "DescriptionWidth",  -1);
    g_ColumnPadding    = Profile_GetInt(&g_Ini,  "File system", "ColumnPadding",     8);
    Profile_GetStr(&g_Ini, "Utilities", "Inspect",  "", g_InspectProg,  sizeof(g_InspectProg));
    Profile_GetStr(&g_Ini, "Utilities", "Default",  "", g_DefaultProg,  sizeof(g_DefaultProg));
    Profile_GetStr(&g_Ini, "Utilities", "Undelete", "", g_UndeleteProg, sizeof(g_UndeleteProg));
    Profile_GetStr(&g_Ini, "Utilities", "Disk",     "", g_DiskProg,     sizeof(g_DiskProg));
    Profile_GetStr(&g_Ini, "File system", "AliasExtension",
                   ".als", g_AliasExtension, sizeof(g_AliasExtension));
    AnsiLower(g_AliasExtension);
    lstrcpy(g_AliasExtUpper, g_AliasExtension);
    AnsiUpper(g_AliasExtUpper);
}

void SaveFileSysProp(void)
{
    Profile_SetInt(&g_Ini,  "File system",  "DefaultDisplay",   (int)g_DefaultDisplay);
    Profile_SetInt(&g_Ini,  "File system",  "DefaultSort",      (int)g_DefaultSort);
    Profile_SetStr(&g_Ini,  "File system",  "IconStrings",      g_IconStrings);
    Profile_SetStr(&g_Ini,  "File system ", "DefaultFilter",    g_DefaultFilter);
    Profile_SetBool(&g_Ini, "File system ", "FileHints",        g_FileHints);
    Profile_SetInt(&g_Ini,  "File system ", "HintDelay",        g_HintDelay);
    Profile_SetInt(&g_Ini,  "File system",  "ListKBDecimals",   g_ListKBDecimals);
    Profile_SetInt(&g_Ini,  "File system",  "ListMBDecimals",   g_ListMBDecimals);
    Profile_SetBool(&g_Ini, "File system",  "UseDescriptions",  g_UseDescriptions);
    Profile_SetBool(&g_Ini, "File system",  "DescCaptions",     g_DescCaptions);
    Profile_SetBool(&g_Ini, "File system",  "Simul4DOS",        g_Simul4DOS);
    Profile_SetBool(&g_Ini, "File system",  "FileSysChange",    g_FileSysChange);
    Profile_SetBool(&g_Ini, "File system",  "EnableLFN",        g_EnableLFN);
    Profile_SetBool(&g_Ini, "Confirmation", "CopyStart",        g_ConfirmCopyStart);
    Profile_SetBool(&g_Ini, "Confirmation", "MoveStart",        g_ConfirmMoveStart);
    Profile_SetBool(&g_Ini, "Confirmation", "DelStart",         g_ConfirmDelStart);
    Profile_SetBool(&g_Ini, "Confirmation", "CopyFile",         g_ConfirmCopyFile);
    Profile_SetBool(&g_Ini, "Confirmation", "MoveFile",         g_ConfirmMoveFile);
    Profile_SetBool(&g_Ini, "Confirmation", "DelFile",          g_ConfirmDelFile);
    Profile_SetBool(&g_Ini, "Confirmation", "Replace",          g_ConfirmReplace);
    Profile_SetBool(&g_Ini, "Confirmation", "Protect",          g_ConfirmProtect);
    Profile_SetBool(&g_Ini, "Confirmation", "CopyFolder",       g_ConfirmCopyFolder);
    Profile_SetBool(&g_Ini, "Confirmation", "MoveFolder",       g_ConfirmMoveFolder);
    Profile_SetBool(&g_Ini, "Confirmation", "DelFolder",        g_ConfirmDelFolder);
    Profile_SetBool(&g_Ini, "Confirmation", "NewAlias",         g_ConfirmNewAlias);
    Profile_SetBool(&g_Ini, "Preferences",  "ShowBrowserToolbar",   g_ShowBrowserToolbar);
    Profile_SetBool(&g_Ini, "Preferences",  "ShowBrowserStatusbar", g_ShowBrowserStatusbar);
    Profile_SetBool(&g_Ini, "Preferences",  "FlatToolbarButtons",   g_FlatToolbarButtons);
    Profile_SetBool(&g_Ini, "Preferences",  "ShortWinCaptions", g_ShortWinCaptions);
    Profile_SetBool(&g_Ini, "Preferences",  "ShowHidSys",       g_ShowHidSys);
    Profile_SetBool(&g_Ini, "Preferences",  "MiniIcons",        g_MiniIcons);
    Profile_SetBool(&g_Ini, "Preferences",  "HQMiniIcons",      g_HQMiniIcons);
    Profile_SetBool(&g_Ini, "Preferences",  "DrawShadedIcons",  g_DrawShadedIcons);
    Profile_SetBool(&g_Ini, "Preferences",  "HintDesc",         g_HintDesc);
    Profile_SetBool(&g_Ini, "Preferences",  "HintBytes",        g_HintBytes);
    Profile_SetBool(&g_Ini, "Preferences",  "HintType",         g_HintType);
    Profile_SetBool(&g_Ini, "Preferences",  "HintDate",         g_HintDate);
    Profile_SetBool(&g_Ini, "Preferences",  "HintTime",         g_HintTime);
    Profile_SetBool(&g_Ini, "Preferences",  "HintAttrib",       g_HintAttrib);
    Profile_SetInt(&g_Ini,  "Preferences",  "DefaultColumns",   (int)g_DefaultColumns);
    Profile_SetStr(&g_Ini,  "Utilities",    "Inspect",          g_InspectProg);
    Profile_SetStr(&g_Ini,  "Utilities",    "Default",          g_DefaultProg);
    Profile_SetStr(&g_Ini,  "Utilities",    "Undelete",         g_UndeleteProg);
    Profile_SetStr(&g_Ini,  "Utilities",    "Disk",             g_DiskProg);
}

void LoadBinProp(void)
{
    Profile_GetStr(&g_Ini, "Bin", "Caption", "Bin", g_BinCaption, sizeof(g_BinCaption));
    g_BinAction   = (TBinAction)Profile_GetInt(&g_Ini,  "Bin", "Action",    0);
    g_BinCapacity = Profile_GetInt(&g_Ini,  "Bin", "Capacity",  8);
    g_BinIcons    = Profile_GetBool(&g_Ini, "Bin", "Icons",     FALSE);
    g_DeleteToBin = Profile_GetBool(&g_Ini, "Bin", "DeleteToBin", FALSE);
}

void SaveBinProp(void)
{
    Profile_SetStr(&g_Ini,  "Bin", "Caption",   g_BinCaption);
    Profile_SetInt(&g_Ini,  "Bin", "Action",    (int)g_BinAction);
    Profile_SetInt(&g_Ini,  "Bin", "Capacity",  g_BinCapacity);
    Profile_SetBool(&g_Ini, "Bin", "Icons",     g_BinIcons);
    Profile_SetBool(&g_Ini, "Bin", "DeleteToBin", g_DeleteToBin);
}

void LoadStartProp(void)
{
    g_StartMenu3D    = Profile_GetBool(&g_Ini, "Start menu", "StartMenu3D",    TRUE);
    g_BoldSelect     = Profile_GetBool(&g_Ini, "Start menu", "BoldSelect",     TRUE);
    g_ShellStartup   = Profile_GetBool(&g_Ini, "Start menu", "ShellStartup",   TRUE);
    g_ShellDDE       = Profile_GetBool(&g_Ini, "Start menu", "ShellDDE",       FALSE);
    g_StartMouseUp   = Profile_GetBool(&g_Ini, "Start menu", "StartMouseUp",   TRUE);
    g_LargeRootMenu  = Profile_GetBool(&g_Ini, "Start menu", "LargeRootMenu",  TRUE);
    g_ColouredBar    = Profile_GetBool(&g_Ini, "Start menu", "ColouredBar",    TRUE);
    g_LargeItemHeight= Profile_GetInt(&g_Ini,  "Start menu", "LargeItemHeight",34);
    g_Win95Keyboard  = Profile_GetBool(&g_Ini, "Preferences","Win95Keyboard",  FALSE);
}

void SaveStartProp(void)
{
    Profile_SetBool(&g_Ini, "Start menu", "StartMenu3D",    g_StartMenu3D);
    Profile_SetBool(&g_Ini, "Start menu", "BoldSelect",     g_BoldSelect);
    Profile_SetBool(&g_Ini, "Start menu", "ShellStartup",   g_ShellStartup);
    Profile_SetBool(&g_Ini, "Start menu", "ShellDDE",       g_ShellDDE);
    Profile_SetBool(&g_Ini, "Start menu", "StartMouseUp",   g_StartMouseUp);
    Profile_SetBool(&g_Ini, "Start menu", "LargeRootMenu",  g_LargeRootMenu);
    Profile_SetBool(&g_Ini, "Start menu", "ColouredBar",    g_ColouredBar);
    Profile_SetInt(&g_Ini,  "Start menu", "LargeItemHeight",g_LargeItemHeight);
    Profile_SetBool(&g_Ini, "Preferences","Win95Keyboard",  g_Win95Keyboard);
}

void LoadTaskProp(void)
{
    g_DisableTaskbar  = Profile_GetBool(&g_Ini, "Taskbar", "Disable",          FALSE);
    g_StayVisible     = Profile_GetBool(&g_Ini, "Taskbar", "StayVisible",      FALSE);
    g_ShrinkMax       = Profile_GetBool(&g_Ini, "Taskbar", "ShrinkMax",        TRUE);
    g_Clock24         = Profile_GetBool(&g_Ini, "Taskbar", "Clock24",          FALSE);
    g_PopupRes        = Profile_GetBool(&g_Ini, "Taskbar", "PopupRes",         TRUE);
    g_PopupDate       = Profile_GetBool(&g_Ini, "Taskbar", "PopupDate",        TRUE);
    g_Animate         = Profile_GetBool(&g_Ini, "Taskbar", "Animate",          TRUE);
    g_FullAnimate     = Profile_GetBool(&g_Ini, "Taskbar", "FullAnimate",      TRUE);
    g_ButtonHints     = Profile_GetBool(&g_Ini, "Taskbar", "ButtonHints",      TRUE);
    g_ArrangeMin      = Profile_GetBool(&g_Ini, "Taskbar", "ArrangeMin",       TRUE);
    g_HideMinApps     = Profile_GetBool(&g_Ini, "Taskbar", "HideMinApps",      TRUE);
    g_IconWindowTask  = Profile_GetBool(&g_Ini, "Taskbar", "IconWindowTask",   TRUE);
    g_ExplorerTask    = Profile_GetBool(&g_Ini, "Taskbar", "ExplorerTask",     TRUE);
    g_FullFolderPath  = Profile_GetBool(&g_Ini, "Taskbar", "FullFolderPath",   FALSE);
    g_DocNameFirst    = Profile_GetBool(&g_Ini, "Taskbar", "DocNameFirst",     FALSE);
    g_DocNameLower    = Profile_GetBool(&g_Ini, "Taskbar", "DocNameLower",     FALSE);
    g_MinAppHeight    = Profile_GetInt(&g_Ini,  "Taskbar", "MinAppHeight",     60);
    g_TopTaskbar      = Profile_GetBool(&g_Ini, "Taskbar", "TopTaskbar",       FALSE);
    g_RowCount        = Profile_GetInt(&g_Ini,  "Taskbar", "RowCount",         1);
    if (g_RowCount < 1) {
        g_RowCount = 1;
    }
    g_ShowRealSysMenu = Profile_GetBool(&g_Ini, "Taskbar", "ShowRealSysMenu",  TRUE);
    g_AnimatePixels   = Profile_GetInt(&g_Ini,  "Taskbar", "AnimatePixels",    1);
    g_AnimateSlowDown = Profile_GetInt(&g_Ini,  "Taskbar", "AnimateSlowDown",  0);
}

void SaveTaskProp(void)
{
    Profile_SetBool(&g_Ini, "Taskbar", "Disable",          g_DisableTaskbar);
    Profile_SetBool(&g_Ini, "Taskbar", "StayVisible",      g_StayVisible);
    Profile_SetBool(&g_Ini, "Taskbar", "ShrinkMax",        g_ShrinkMax);
    Profile_SetBool(&g_Ini, "Taskbar", "Clock24",          g_Clock24);
    Profile_SetBool(&g_Ini, "Taskbar", "PopupRes",         g_PopupRes);
    Profile_SetBool(&g_Ini, "Taskbar", "PopupDate",        g_PopupDate);
    Profile_SetBool(&g_Ini, "Taskbar", "Animate",          g_Animate);
    Profile_SetBool(&g_Ini, "Taskbar", "FullAnimate",      g_FullAnimate);
    Profile_SetBool(&g_Ini, "Taskbar", "ButtonHints",      g_ButtonHints);
    Profile_SetBool(&g_Ini, "Taskbar", "ArrangeMin",       g_ArrangeMin);
    Profile_SetBool(&g_Ini, "Taskbar", "HideMinApps",      g_HideMinApps);
    Profile_SetBool(&g_Ini, "Taskbar", "IconWindowTask",   g_IconWindowTask);
    Profile_SetBool(&g_Ini, "Taskbar", "ExplorerTask",     g_ExplorerTask);
    Profile_SetBool(&g_Ini, "Taskbar", "FullFolderPath",   g_FullFolderPath);
    Profile_SetBool(&g_Ini, "Taskbar", "DocNameFirst",     g_DocNameFirst);
    Profile_SetBool(&g_Ini, "Taskbar", "DocNameLower",     g_DocNameLower);
    Profile_SetBool(&g_Ini, "Taskbar", "TopTaskbar",       g_TopTaskbar);
    Profile_SetInt(&g_Ini,  "Taskbar", "RowCount",         g_RowCount);
    Profile_SetBool(&g_Ini, "Taskbar", "ShowRealSysMenu",  g_ShowRealSysMenu);
    Profile_SetInt(&g_Ini,  "Taskbar", "AnimatePixels",    g_AnimatePixels);
    Profile_SetInt(&g_Ini,  "Taskbar", "AnimateSlowDown",  g_AnimateSlowDown);
}

/* ======================================================================
 * Master load
 * ====================================================================== */

void LoadSettings(void)
{
    /* Check for first run */
    g_FirstRun = Profile_GetBool(&g_Ini, "Calmira", "Calmira32", TRUE);
    if (g_FirstRun) {
        Profile_SetBool(&g_Ini, "Calmira", "Calmira32", FALSE);
    }

    g_NormalHintPause = Profile_GetInt(&g_Ini, "Calmira", "NormalHintPause", 800);
    g_FilePaneCols    = Profile_GetInt(&g_Ini, "File system", "FilePaneCols", 5);
    g_DescWidth       = Profile_GetInt(&g_Ini, "File system", "DescriptionWidth", -1);
    g_ColumnPadding   = Profile_GetInt(&g_Ini, "File system", "ColumnPadding", 8);

    LoadSystemProp();
    LoadDeskProp();
    LoadFileSysProp();
    LoadBinProp();
    LoadTaskProp();
    LoadStartProp();
}

/* ======================================================================
 * Broadcast settings change to all Calmira top-level windows
 * ====================================================================== */

void AnnounceSettingsChanged(TSettingChanges changes)
{
    HWND hw;
    if (changes & scINIFile) {
        LoadSettings();
    }
    /* Enumerate all top-level windows and post WM_SETTINGSCHANGED */
    hw = GetTopWindow(NULL);
    while (hw) {
        char cls[64];
        GetClassName(hw, cls, sizeof(cls));
        /* Only notify our own windows (registered with CALMIRA_CLASS prefix) */
        if (lstrlen(cls) >= 8 && _fmemcmp(cls, "Calmira", 7) == 0) {
            PostMessage(hw, WM_SETTINGSCHANGED, (WPARAM)changes, 0L);
        }
        hw = GetNextWindow(hw, GW_HWNDNEXT);
    }
}
