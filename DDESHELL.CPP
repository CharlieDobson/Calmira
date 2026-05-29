/***************************************************************************
 *
 *  DDESHELL.CPP - Program Manager DDE shell interface
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Responds to "PROGMAN"/"PROGMAN" DDE conversations on the hidden
 *  CALMIRA_CLASS window (g_hwMain).  Installer-issued commands are
 *  translated into edits on START.INI so that created groups and items
 *  appear in Calmira's start menu immediately.
 *
 *  START.INI mapping:
 *    CreateGroup("MyApp")  -->  [Start\Programs] gains "MyApp*="
 *                               current group  = "Start\Programs\MyApp"
 *    AddItem(cmd, name)    -->  [Start\Programs\MyApp] gains
 *                               "name=cmd;workdir;showmode;iconfile;iconidx"
 *    DeleteItem(name)      -->  removes that key
 *    DeleteGroup(name)     -->  removes the section and its folder entry
 *
 *  Win16 DDE message conventions (16-bit):
 *    WM_DDE_EXECUTE  wParam=hwndClient  lParam=(LPARAM)hData
 *    WM_DDE_ACK      wParam=hwndServer  lParam=MAKELONG(wStatus,hData)
 *                      wStatus bit 0x8000 = positive ACK
 *
 ***************************************************************************/

#include <windows.h>
#include <dde.h>
#include <string.h>
#include "DDESHELL.H"
#include "SETTINGS.H"
#include "HELPERS.H"

/* DDE service and topic names (must match PROGMAN exactly) */
#define DDE_APP    "PROGMAN"
#define DDE_TOPIC  "PROGMAN"

/* Root section for DDE-created groups */
#define DDE_ROOT_SECT   "Start"
#define DDE_PROG_SECT   "Start\\Programs"

/* Maximum simultaneously open DDE conversations */
#define MAX_CONVS  8

/* Per-conversation state */
typedef struct {
    HWND hwndClient;            /* 0 = slot is free */
    char szCurGroup[144];       /* current START.INI section path */
} DDECONV;

static DDECONV s_convs[MAX_CONVS];

/* -----------------------------------------------------------------------
 * Conversation slot management
 * ----------------------------------------------------------------------- */

static int FindConv(HWND hwndClient)
{
    int i;
    for (i = 0; i < MAX_CONVS; i++)
        if (s_convs[i].hwndClient == hwndClient)
            return i;
    return -1;
}

static int AllocConv(HWND hwndClient)
{
    int i;
    for (i = 0; i < MAX_CONVS; i++) {
        if (!s_convs[i].hwndClient) {
            s_convs[i].hwndClient = hwndClient;
            s_convs[i].szCurGroup[0] = '\0';
            return i;
        }
    }
    return -1;  /* full */
}

static void FreeConv(int idx)
{
    if (idx >= 0 && idx < MAX_CONVS) {
        s_convs[idx].hwndClient = 0;
        s_convs[idx].szCurGroup[0] = '\0';
    }
}

/* -----------------------------------------------------------------------
 * Argument parsing
 *
 * Expects the parser position to be just after the opening '(' of a
 * command.  Parses one comma-separated argument (optionally quoted) and
 * advances past it (stopping before the next ',' or ')').
 * ----------------------------------------------------------------------- */

static LPCSTR SkipWS(LPCSTR p)
{
    while (*p == ' ' || *p == '\t') p++;
    return p;
}

/* Returns pointer past the parsed argument (just after the trailing comma
   if one follows, otherwise at ')' or end of string). */
static LPCSTR ParseArg(LPCSTR p, LPSTR buf, int bufLen)
{
    int i = 0;
    p = SkipWS(p);

    if (*p == '"') {
        /* Quoted: read until closing quote */
        p++;
        while (*p && *p != '"' && i < bufLen - 1)
            buf[i++] = *p++;
        if (*p == '"') p++;
    } else {
        /* Unquoted: read until comma or closing paren */
        while (*p && *p != ',' && *p != ')' && i < bufLen - 1)
            buf[i++] = *p++;
        /* Trim trailing whitespace */
        while (i > 0 && (buf[i-1] == ' ' || buf[i-1] == '\t'))
            i--;
    }
    buf[i] = '\0';

    p = SkipWS(p);
    if (*p == ',') p++;
    return p;
}

/* -----------------------------------------------------------------------
 * START.INI helpers
 * ----------------------------------------------------------------------- */

/* Add a folder entry "name*=" to szSect if it is not already there. */
static void EnsureFolderEntry(LPCSTR szSect, LPCSTR szName)
{
    char key[84];
    char probe[4];

    lstrcpyn(key, szName, (int)sizeof(key) - 2);
    lstrcat(key, "*");

    /* Use a sentinel value that is never a valid folder entry */
    GetPrivateProfileString(szSect, key, "\x01", probe, sizeof(probe),
                            g_szStartFile);
    if (probe[0] == '\x01') {
        /* Key absent: add it with an empty value */
        WritePrivateProfileString(szSect, key, "", g_szStartFile);
    }
}

/* -----------------------------------------------------------------------
 * DDE command implementations
 * ----------------------------------------------------------------------- */

/* [CreateGroup(name[,path])]
   Creates or selects a program group under Start\Programs.
   The optional path argument (PROGMAN .GRP file path) is ignored. */
static void CmdCreateGroup(LPCSTR pArgs, int ci)
{
    char name[80];
    ParseArg(pArgs, name, sizeof(name));
    if (!*name) return;

    /* Ensure the path to this group exists in START.INI */
    EnsureFolderEntry(DDE_ROOT_SECT, "Programs");
    EnsureFolderEntry(DDE_PROG_SECT, name);

    /* Track as the current group for subsequent AddItem calls */
    lstrcpy(s_convs[ci].szCurGroup, DDE_PROG_SECT "\\");
    lstrcat(s_convs[ci].szCurGroup, name);
}

/* [ShowGroup(name,show_cmd)]
   Selects the named group as current (show_cmd is advisory only;
   we have no separate group windows to show/hide). */
static void CmdShowGroup(LPCSTR pArgs, int ci)
{
    char name[80];
    ParseArg(pArgs, name, sizeof(name));
    if (!*name) return;

    lstrcpy(s_convs[ci].szCurGroup, DDE_PROG_SECT "\\");
    lstrcat(s_convs[ci].szCurGroup, name);
}

/* [AddItem(cmd[,name[,icon_file[,icon_idx[,hotkey[,minimize[,work_dir]]]]]])]
   Adds a program item to the current group.
   PROGMAN minimize flag: 0=normal, non-zero=minimized on launch. */
static void CmdAddItem(LPCSTR pArgs, int ci)
{
    char cmd[144], name[80], iconFile[144];
    char iconIdxStr[8], hotkeyStr[8], minimizeStr[8], workDir[144];
    char szVal[400];
    int  iconIdx, showMode;
    LPCSTR szSect;

    pArgs = ParseArg(pArgs, cmd,         sizeof(cmd));
    pArgs = ParseArg(pArgs, name,        sizeof(name));
    pArgs = ParseArg(pArgs, iconFile,    sizeof(iconFile));
    pArgs = ParseArg(pArgs, iconIdxStr,  sizeof(iconIdxStr));
    pArgs = ParseArg(pArgs, hotkeyStr,   sizeof(hotkeyStr));
    pArgs = ParseArg(pArgs, minimizeStr, sizeof(minimizeStr));
    ParseArg(pArgs, workDir, sizeof(workDir));

    if (!*cmd) return;

    /* Derive display name from the exe filename if not provided */
    if (!*name) {
        LPCSTR p = cmd + lstrlen(cmd);
        while (p > cmd && *p != '\\' && *p != ':') p--;
        if (*p == '\\' || *p == ':') p++;
        lstrcpyn(name, p, sizeof(name));
        /* Strip the extension */
        {
            char *dot = name + lstrlen(name);
            while (dot > name && *dot != '.') dot--;
            if (dot > name) *dot = '\0';
        }
    }

    iconIdx = *iconIdxStr ? (int)(*iconIdxStr - '0') : 0;
    /* Map PROGMAN minimize flag to Calmira ShowMode (SW_* value) */
    showMode = (*minimizeStr && *minimizeStr != '0') ? 2 : 1;

    szSect = *s_convs[ci].szCurGroup
             ? s_convs[ci].szCurGroup
             : DDE_PROG_SECT;

    /* value format: command;workdir;showmode;iconfile;iconindex */
    wsprintf(szVal, "%s;%s;%d;%s;%d",
             cmd, workDir, showMode, iconFile, iconIdx);
    WritePrivateProfileString(szSect, name, szVal, g_szStartFile);
}

/* [ReplaceItem(name)]
   Removes the named item (caller will follow with AddItem). */
static void CmdReplaceItem(LPCSTR pArgs, int ci)
{
    char name[80];
    LPCSTR szSect;
    ParseArg(pArgs, name, sizeof(name));
    if (!*name) return;
    szSect = *s_convs[ci].szCurGroup
             ? s_convs[ci].szCurGroup
             : DDE_PROG_SECT;
    WritePrivateProfileString(szSect, name, NULL, g_szStartFile);
}

/* [DeleteItem(name)]
   Removes a program item from the current group. */
static void CmdDeleteItem(LPCSTR pArgs, int ci)
{
    CmdReplaceItem(pArgs, ci);   /* identical operation */
}

/* [DeleteGroup(name)]
   Removes a group section and its folder entry from START.INI. */
static void CmdDeleteGroup(LPCSTR pArgs, int ci)
{
    char name[80], sect[144], key[84];
    ParseArg(pArgs, name, sizeof(name));
    if (!*name) return;

    /* Wipe the section */
    lstrcpy(sect, DDE_PROG_SECT "\\");
    lstrcat(sect, name);
    WritePrivateProfileString(sect, NULL, NULL, g_szStartFile);

    /* Remove the folder entry from the Programs section */
    lstrcpyn(key, name, (int)sizeof(key) - 2);
    lstrcat(key, "*");
    WritePrivateProfileString(DDE_PROG_SECT, key, NULL, g_szStartFile);

    /* Clear current group if it was this one */
    if (ci >= 0 && lstrcmpi(s_convs[ci].szCurGroup, sect) == 0)
        s_convs[ci].szCurGroup[0] = '\0';
}

/* [ExitProgman([save])]
   Shuts Calmira down (posts WM_QUIT so WinMain cleans up normally). */
static void CmdExitProgman(LPCSTR pArgs)
{
    PostQuitMessage(0);
}

/* -----------------------------------------------------------------------
 * Command string executor
 *
 * Parses "[Cmd(args)][Cmd(args)]..." and dispatches each command.
 * Returns TRUE if every command was recognised.
 * ----------------------------------------------------------------------- */
static BOOL ExecCommands(LPCSTR psz, int ci)
{
    BOOL ok = TRUE;

    while (*psz) {
        char cmd[32];
        LPCSTR pArgs;
        int    i;

        /* Advance to opening bracket */
        while (*psz && *psz != '[') psz++;
        if (!*psz) break;
        psz++;

        /* Read command name up to '(' */
        i = 0;
        while (*psz && *psz != '(' && *psz != ']' && i < (int)sizeof(cmd) - 1)
            cmd[i++] = *psz++;
        cmd[i] = '\0';

        /* Trim trailing whitespace from command name */
        while (i > 0 && (cmd[i-1] == ' ' || cmd[i-1] == '\t'))
            cmd[--i] = '\0';

        if (*psz != '(') {
            ok = FALSE;
            continue;
        }
        pArgs = psz + 1;  /* just past '(' */

        /* Dispatch */
        if      (lstrcmpi(cmd, "CreateGroup")  == 0) CmdCreateGroup(pArgs, ci);
        else if (lstrcmpi(cmd, "ShowGroup")    == 0) CmdShowGroup(pArgs, ci);
        else if (lstrcmpi(cmd, "AddItem")      == 0) CmdAddItem(pArgs, ci);
        else if (lstrcmpi(cmd, "ReplaceItem")  == 0) CmdReplaceItem(pArgs, ci);
        else if (lstrcmpi(cmd, "DeleteItem")   == 0) CmdDeleteItem(pArgs, ci);
        else if (lstrcmpi(cmd, "DeleteGroup")  == 0) CmdDeleteGroup(pArgs, ci);
        else if (lstrcmpi(cmd, "ExitProgman")  == 0) CmdExitProgman(pArgs);
        /* Reload: silently accepted - menu is built lazily from START.INI */

        /* Skip to the closing ']' */
        while (*psz && *psz != ']') psz++;
        if (*psz == ']') psz++;
    }
    return ok;
}

/* -----------------------------------------------------------------------
 * Public interface
 * ----------------------------------------------------------------------- */

/*
 * DdeShell_OnInitiate
 *
 * Called from CalmiraWndProc for WM_DDE_INITIATE.
 * Accepts connections for service="PROGMAN", topic="PROGMAN" (or wildcards).
 * Responds with WM_DDE_ACK sent back to the client.
 *
 * Win16: WM_DDE_INITIATE is a SendMessage (synchronous) broadcast.
 */
BOOL DdeShell_OnInitiate(HWND hwndServer, HWND hwndClient,
                         ATOM aApp, ATOM aTopic)
{
    ATOM aProgman;
    BOOL bMatch;

    /* Add our own atom temporarily so we can compare by value */
    aProgman = GlobalAddAtom(DDE_APP);

    bMatch = ((aApp   == 0 || aApp   == aProgman) &&
              (aTopic == 0 || aTopic == aProgman));

    GlobalDeleteAtom(aProgman);  /* decrement ref count only */

    if (!bMatch) return FALSE;

    AllocConv(hwndClient);

    /* Acknowledge: send back the same lParam (atoms stay owned by client) */
    SendMessage(hwndClient, WM_DDE_ACK, (WPARAM)hwndServer,
                MAKELONG(aApp, aTopic));
    return TRUE;
}

/*
 * DdeShell_OnExecute
 *
 * Called from CalmiraWndProc for WM_DDE_EXECUTE.
 * Reads the command string from hData, executes it, then posts WM_DDE_ACK.
 * The client is responsible for freeing hData after receiving the ACK.
 *
 * Win16: wParam=hwndClient, lParam=(LPARAM)hData (handle in low word).
 * ACK:   lParam=MAKELONG(wStatus, hData)  bit 0x8000 = positive ACK.
 */
BOOL DdeShell_OnExecute(HWND hwndServer, HWND hwndClient, HANDLE hData)
{
    BOOL  ok  = FALSE;
    int   ci;
    LPSTR psz;

    ci = FindConv(hwndClient);
    if (ci < 0) ci = AllocConv(hwndClient);  /* accept unannounced client */

    if (hData) {
        psz = (LPSTR)GlobalLock(hData);
        if (psz) {
            ok = ExecCommands(psz, ci >= 0 ? ci : 0);
            GlobalUnlock(hData);
        }
    }

    /* Return hData to client; it frees it after reading the ACK status */
    PostMessage(hwndClient, WM_DDE_ACK, (WPARAM)hwndServer,
                MAKELONG(ok ? (UINT)0x8000 : 0, (UINT)hData));
    return TRUE;
}

/*
 * DdeShell_OnTerminate
 *
 * Called from CalmiraWndProc for WM_DDE_TERMINATE.
 * Closes the conversation and echoes WM_DDE_TERMINATE back.
 */
void DdeShell_OnTerminate(HWND hwndServer, HWND hwndClient)
{
    FreeConv(FindConv(hwndClient));
    PostMessage(hwndClient, WM_DDE_TERMINATE, (WPARAM)hwndServer, 0L);
}
