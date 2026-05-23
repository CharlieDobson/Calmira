/***************************************************************************
 *
 *  SETTINGS.H - Calmira global application settings declarations
 *  Translated from SETTINGS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  All settings are global variables initialized from CALMIRA.INI.
 *  Include this header in any module that needs to read settings.
 *  Only SETTINGS.CPP writes to these variables.
 *
 ***************************************************************************/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <windows.h>
#include "PROFILE.H"

/* ======================================================================
 * Enumerated types (mirroring Delphi type definitions)
 * ====================================================================== */

typedef enum { soName=0, soType, soSize, soDate } TSortOrder;
typedef enum { woCascaded=0, woRandom, woSaved }  TWindowOpen;
typedef enum { dmLargeIcons=0, dmSmallIcons, dmDetails } TDisplayMode;
typedef enum { baLeave=0, baDelete, baEmpty, baCollect } TBinAction;

/* Application color indices */
typedef enum {
    ccWinFrame=0, ccIconBack, ccIconSel, ccShortArrow,
    ccAliasArrow, ccPercent, ccPercentBack, ccTaskbar, ccStartmenu,
    ccStartHighlight, ccChecklist,
    ccCOUNT   /* must be last */
} TCalColor;

/* File detail column flags */
#define fdSize  0x01
#define fdType  0x02
#define fdDate  0x04
#define fdTime  0x08
#define fdAttr  0x10
#define fdDesc  0x20
#define fdALL   0x3F
typedef BYTE TFileDetails;

/* Setting-change notification flags (passed to AnnounceSettingsChanged) */
#define scSystem      0x0001
#define scFileSystem  0x0002
#define scDesktop     0x0004
#define scStartMenu   0x0008
#define scBin         0x0010
#define scTaskbar     0x0020
#define scDisplay     0x0040
#define scINIFile     0x0080
#define sc4DOS        0x0100
#define scDevices     0x0200
typedef UINT TSettingChanges;

/* ======================================================================
 * Path utilities (computed at startup, not in INI)
 * ====================================================================== */

extern char g_szAppPath[144];    /* directory containing CALMIRA.EXE     */
extern char g_szWinPath[144];    /* Windows directory                    */
extern char g_szWritePath[144];  /* directory for CALMIRA.INI (= app or win) */
extern char g_szStartFile[144];  /* full path to START.INI               */

/* Global INI profile */
extern PROFILE g_Ini;            /* opened to g_szWritePath\calmira.ini  */

/* ======================================================================
 * System / general settings
 * ====================================================================== */

extern char  g_ComputerCaption[80];
extern BOOL  g_ShowSplash;
extern BOOL  g_RestoreSys;        /* restore Computer window after startup */
extern BOOL  g_SysWinQuit;        /* close Computer window to quit         */
extern BOOL  g_TrackThumb;        /* live scroll content while dragging    */
extern BOOL  g_GlobalHotkeys;
extern BOOL  g_CompIconStart;
extern BOOL  g_EnableWinScripts;
extern BOOL  g_EnableDosScripts;
extern char  g_DosScriptFilename[144];
extern char  g_WinScriptExtension[8];
extern BOOL  g_Win95Keyboard;
extern BOOL  g_ShowShutDownBMP;
extern char  g_Browser[144];
extern BOOL  g_IsShell;            /* TRUE if Calmira is the Windows shell */
extern BOOL  g_FirstRun;           /* TRUE on very first launch            */
extern BOOL  g_LoadFromWinDir;
extern int   g_DoubleClickSpeed;

/* ======================================================================
 * File system settings
 * ====================================================================== */

extern TSortOrder   g_DefaultSort;
extern TDisplayMode g_DefaultDisplay;
extern char  g_DefaultFilter[84];
extern char  g_IconStrings[84];    /* extensions with embedded icons       */
extern int   g_ListKBDecimals;
extern int   g_ListMBDecimals;
extern char  g_AliasExtension[8];
extern char  g_AliasExtUpper[8];

extern BOOL  g_FileHints;
extern int   g_HintDelay;
extern BOOL  g_UseDescriptions;
extern char  g_DescriptionFile[20];
extern BOOL  g_DescCaptions;
extern BOOL  g_Simul4DOS;
extern BOOL  g_FileSysChange;
extern BOOL  g_EnableLFN;

/* Confirmation flags */
extern BOOL  g_ConfirmCopyStart;
extern BOOL  g_ConfirmMoveStart;
extern BOOL  g_ConfirmDelStart;
extern BOOL  g_ConfirmCopyFile;
extern BOOL  g_ConfirmMoveFile;
extern BOOL  g_ConfirmDelFile;
extern BOOL  g_ConfirmReplace;
extern BOOL  g_ConfirmProtect;
extern BOOL  g_ConfirmCopyFolder;
extern BOOL  g_ConfirmMoveFolder;
extern BOOL  g_ConfirmDelFolder;
extern BOOL  g_ConfirmNewAlias;

/* Browser toolbar / display */
extern BOOL  g_ShowBrowserToolbar;
extern BOOL  g_ShowBrowserStatusbar;
extern BOOL  g_FlatToolbarButtons;
extern BOOL  g_ShortWinCaptions;
extern BOOL  g_ShowHidSys;
extern BOOL  g_ProgDrop;
extern BOOL  g_FindDlgIcons;
extern BOOL  g_AliasArrows;
extern BOOL  g_DefDragCopy;
extern BOOL  g_DrawShadedIcons;
extern BOOL  g_MiniIcons;
extern BOOL  g_HQMiniIcons;
extern BOOL  g_UpcaseFirstChar;
extern BOOL  g_NoRegExtensions;

/* Hint content flags */
extern BOOL  g_HintDesc;
extern BOOL  g_HintBytes;
extern BOOL  g_HintType;
extern BOOL  g_HintDate;
extern BOOL  g_HintTime;
extern BOOL  g_HintAttrib;

extern TFileDetails g_DefaultColumns;
extern int          g_FilePaneCols;

/* External programs */
extern char  g_InspectProg[144];
extern char  g_DefaultProg[144];
extern char  g_UndeleteProg[144];
extern char  g_DiskProg[144];

/* ======================================================================
 * Desktop settings
 * ====================================================================== */

extern TWindowOpen g_WindowOpen;
extern BOOL  g_SingleStatus;
extern BOOL  g_SaveWindows;
extern BOOL  g_ShortArrows;
extern BOOL  g_ShowDeskMenu;
extern BOOL  g_ExploreLastFolder;
extern BOOL  g_ConfirmDelShort;
extern BOOL  g_StickyShorts;
extern BOOL  g_OneClickShorts;
extern BOOL  g_BrowseSame;
extern BOOL  g_RightClose;
extern BOOL  g_RButtonUpClose;
extern BOOL  g_RunAutoClose;
extern BOOL  g_DesktopParent;
extern POINT g_DeskGrid;
extern POINT g_BrowseGrid;
extern int   g_LineHeight;
extern int   g_MinAppHeight;
extern int   g_DescWidth;
extern int   g_ColumnPadding;

/* Colors */
extern COLORREF g_Colors[ccCOUNT];

/* ======================================================================
 * Taskbar settings
 * ====================================================================== */

extern BOOL  g_DisableTaskbar;
extern BOOL  g_StayVisible;
extern BOOL  g_ShrinkMax;
extern BOOL  g_Clock24;
extern BOOL  g_PopupRes;
extern BOOL  g_PopupDate;
extern BOOL  g_Animate;
extern BOOL  g_FullAnimate;
extern BOOL  g_ButtonHints;
extern BOOL  g_ArrangeMin;
extern BOOL  g_HideMinApps;
extern BOOL  g_IconWindowTask;
extern BOOL  g_ExplorerTask;
extern BOOL  g_FullFolderPath;
extern BOOL  g_DocNameFirst;
extern BOOL  g_DocNameLower;
extern BOOL  g_ShowRealSysMenu;
extern BOOL  g_TopTaskbar;
extern int   g_RowCount;
extern int   g_AnimatePixels;
extern int   g_AnimateSlowDown;

/* ======================================================================
 * Bin settings
 * ====================================================================== */

extern char      g_BinCaption[80];
extern TBinAction g_BinAction;
extern int       g_BinCapacity;
extern BOOL      g_BinIcons;
extern BOOL      g_DeleteToBin;

/* ======================================================================
 * Start menu settings
 * ====================================================================== */

extern BOOL  g_StartMenu3D;
extern BOOL  g_ShellStartup;
extern BOOL  g_ShellDDE;
extern BOOL  g_BoldSelect;
extern BOOL  g_StartMouseUp;
extern BOOL  g_LargeRootMenu;
extern BOOL  g_ColouredBar;
extern int   g_LargeItemHeight;

/* ======================================================================
 * Display / layout
 * ====================================================================== */

extern int   g_NormalHintPause;
extern LOGFONT g_GlobalFont;

/* ======================================================================
 * Public interface
 * ====================================================================== */

/* Call once at startup (before creating any windows) */
void Settings_Init(void);

/* Call once at shutdown */
void Settings_Done(void);

/* Load all settings from INI */
void LoadSettings(void);

/* Individual section loaders (also called by property dialogs) */
void LoadSystemProp(void);
void LoadDeskProp(void);
void LoadFileSysProp(void);
void LoadBinProp(void);
void LoadTaskProp(void);
void LoadStartProp(void);

/* Individual section savers (called by property dialogs after OK) */
void SaveSystemProp(void);
void SaveDeskProp(void);
void SaveFileSysProp(void);
void SaveBinProp(void);
void SaveTaskProp(void);
void SaveStartProp(void);

/* Broadcast WM_SETTINGSCHANGED to all Calmira top-level windows */
void AnnounceSettingsChanged(TSettingChanges changes);

/* Utility: expand %envvar% tokens in a path string */
void ExpandEnvPath(LPCSTR szSrc, LPSTR szDst, int dstLen);

#endif /* SETTINGS_H */
