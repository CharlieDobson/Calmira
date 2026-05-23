/***************************************************************************
 *
 *  START.CPP - Start Menu
 *  Translated from START.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Reads START.INI and builds a hierarchical popup menu.
 *  Items are owner-drawn so we can show icons.
 *
 *  START.INI entry format:
 *    Caption=Command;WorkDir;ShowMode;IconFile;IconIndex
 *
 *  Special caption prefixes:
 *    *  (trailing asterisk) = folder with sub-menu
 *    -n                     = separator
 *    *n (leading asterisk)  = special control panel depth
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include "START.H"
#include "SETTINGS.H"
#include "RESOURCE.H"
#include "RUNPROG.H"
#include "SHUTDWN.H"
#include "FILEFIND.H"
#include "STRTPROP.H"
#include "TASKPROP.H"
#include "DESKPROP.H"
#include "FSYSPROP.H"
#include "BINPROP.H"
#include "SYSPROP.H"
#include "FILEMAN.H"
#include "HELPERS.H"

extern HINSTANCE g_hInst;
extern HWND      g_hwDesk;
extern HWND      g_hwComputer;

/* -----------------------------------------------------------------------
 * Parse one START.INI value line:
 *   "command;workdir;showmode;iconfile;iconindex"
 * into a STARTITEM.  The caption is passed separately.
 * ----------------------------------------------------------------------- */
static void ParseItem(LPCSTR szCaption, LPCSTR szValue,
                      LPCSTR szSect, STARTITEM FAR *pi)
{
    char val[START_LINE_MAX];
    char *p, *f;
    int   field;

    lstrcpyn(pi->Caption, szCaption, sizeof(pi->Caption));
    lstrcpyn(val, szValue, sizeof(val));

    pi->Command[0]  = '\0';
    pi->Params[0]   = '\0';
    pi->WorkDir[0]  = '\0';
    pi->ShowMode    = SW_SHOWNORMAL;
    pi->IconFile[0] = '\0';
    pi->IconIndex   = 0;
    pi->bFolder     = FALSE;
    pi->bSep        = FALSE;
    pi->SectName[0] = '\0';

    /* Check for separator: caption starts with '-' followed by digit(s) */
    if (szCaption[0] == '-') {
        pi->bSep = TRUE;
        return;
    }

    /* Check for folder: caption ends with '*' */
    {
        int len = lstrlen(pi->Caption);
        if (len > 0 && pi->Caption[len-1] == '*') {
            pi->Caption[len-1] = '\0';
            pi->bFolder = TRUE;
            /* Build section name: parent section + '\' + caption (no '*') */
            lstrcpyn(pi->SectName, szSect, sizeof(pi->SectName));
            lstrcat(pi->SectName, "\\");
            lstrcat(pi->SectName, pi->Caption);
        }
    }

    /* Parse semicolon-delimited fields */
    p = val;
    field = 0;
    while (*p) {
        f = p;
        while (*p && *p != ';') p++;
        if (*p == ';') {
            *p++ = '\0';
        }
        switch (field) {
            case 0: lstrcpyn(pi->Command,  f, sizeof(pi->Command));  break;
            case 1: lstrcpyn(pi->WorkDir,  f, sizeof(pi->WorkDir));  break;
            case 2: pi->ShowMode = (*f ? (int)(*f - '0') : SW_SHOWNORMAL); break;
            case 3: lstrcpyn(pi->IconFile, f, sizeof(pi->IconFile)); break;
            case 4: pi->IconIndex = (*f ? (int)(*f - '0') : 0);     break;
        }
        field++;
    }
}

/* -----------------------------------------------------------------------
 * Execute a built-in Calmira command
 * ----------------------------------------------------------------------- */
void StartMenu_ExecBuiltin(HWND hwndOwner, LPCSTR szCmd)
{
    if (lstrcmpi(szCmd, SC_RUN) == 0) {
        RunProg_ShowDialog(g_hInst, hwndOwner);
    }
    else if (lstrcmpi(szCmd, SC_SHUTDOWN) == 0) {
        int action = Shutdown_ShowDialog(g_hInst, hwndOwner);
        if (action != SD_CANCEL) {
            Shutdown_Execute(action, g_ShowShutDownBMP, g_hInst);
        }
    }
    else if (lstrcmpi(szCmd, SC_FIND) == 0) {
        HWND hwFind = FileFind_Create(g_hInst, hwndOwner);
        if (hwFind) {
            FileFind_Show(hwFind);
        }
    }
    else if (lstrcmpi(szCmd, SC_EXPLORE) == 0) {
        if (g_hwComputer) {
            ShowWindow(g_hwComputer, SW_SHOWNORMAL);
            SetForegroundWindow(g_hwComputer);
        }
    }
    else if (lstrcmpi(szCmd, SC_SYSPROP) == 0) {
        SysProp_ShowDialog(g_hInst, hwndOwner);
    }
    else if (lstrcmpi(szCmd, SC_DESKPROP) == 0) {
        DeskProp_ShowDialog(g_hInst, hwndOwner);
    }
    else if (lstrcmpi(szCmd, SC_FILESYSPROP) == 0) {
        FSysProp_ShowDialog(g_hInst, hwndOwner);
    }
    else if (lstrcmpi(szCmd, SC_BINPROP) == 0) {
        BinProp_ShowDialog(g_hInst, hwndOwner);
    }
    else if (lstrcmpi(szCmd, SC_STARTPROP) == 0) {
        StrtProp_ShowDialog(g_hInst, hwndOwner);
    }
    else if (lstrcmpi(szCmd, SC_TASKPROP) == 0) {
        TaskProp_ShowDialog(g_hInst, hwndOwner);
    }
    else if (lstrcmpi(szCmd, SC_HELP) == 0) {
        char szHelp[144];
        lstrcpy(szHelp, g_szAppPath);
        lstrcat(szHelp, "CALMIRA.HLP");
        WinHelp(hwndOwner, szHelp, HELP_CONTENTS, 0L);
    }
}

/* -----------------------------------------------------------------------
 * Execute a start menu item
 * ----------------------------------------------------------------------- */
void StartMenu_ExecItem(HWND hwndOwner, const STARTITEM FAR *pItem)
{
    char cmd[144];
    lstrcpy(cmd, pItem->Command);

    if (!*cmd) {
        return;
    }

    /* Built-in command? */
    if (cmd[0] == '$') {
        StartMenu_ExecBuiltin(hwndOwner, cmd + 1);
        return;
    }

    /* URL? */
    {
        char lower[144];
        lstrcpy(lower, cmd);
        AnsiLower(lower);
        if (lstrstr(lower, "://") || lstrstr(lower, "mailto:")) {
            char browser[144];
            lstrcpy(browser, g_Browser);
            if (!*browser) {
                lstrcpy(browser, "iexplore.exe");
            }
            FileMan_Exec(hwndOwner, browser, cmd, NULL, SW_SHOWNORMAL);
            return;
        }
    }

    /* Regular program */
    FileMan_DefaultExec(hwndOwner, cmd,
                        pItem->Params[0] ? pItem->Params : NULL,
                        pItem->WorkDir[0] ? pItem->WorkDir : NULL,
                        pItem->ShowMode ? pItem->ShowMode : SW_SHOWNORMAL);
}

/* -----------------------------------------------------------------------
 * Build a popup HMENU from a START.INI section
 * ----------------------------------------------------------------------- */
static HMENU BuildMenu(HINSTANCE hInst, LPCSTR szSection);

/* Forward declaration for recursive sub-menus */

static HMENU BuildMenu(HINSTANCE hInst, LPCSTR szSection)
{
    LPCSTR  p;
    HMENU   hmenu;
    UINT    id = 0x8000;
    HANDLE  hBuf;
    char FAR *section_buf;

    hmenu = CreatePopupMenu();
    if (!hmenu) {
        return NULL;
    }

    /* Allocate the 4 KB section buffer on the global heap so that recursive
       calls do not accumulate it on the stack (prevents stack overflow with
       deeply nested START.INI folder hierarchies). */
    hBuf = GlobalAlloc(GHND, 4096);
    if (!hBuf) {
        return hmenu;   /* return empty menu rather than crashing */
    }
    section_buf = (char FAR *)GlobalLock(hBuf);

    GetPrivateProfileSection(szSection, section_buf, 4096, g_szStartFile);
    p = section_buf;
    while (p && *p) {
        char key[128], val[START_LINE_MAX];
        LPCSTR eq = p;
        while (*eq && *eq != '=') eq++;
        if (*eq == '=') {
            int keyLen = (int)(eq - p);
            lstrcpyn(key, p, keyLen + 1);
            lstrcpyn(val, eq + 1, sizeof(val));
        } else {
            lstrcpy(key, p);
            val[0] = '\0';
        }

        {
            STARTITEM si;
            ParseItem(key, val, szSection, &si);

            if (si.bSep) {
                AppendMenu(hmenu, MF_SEPARATOR, 0, NULL);
            } else if (si.bFolder) {
                HMENU hSub = BuildMenu(hInst, si.SectName);
                AppendMenu(hmenu, MF_POPUP, (UINT)hSub, si.Caption);
            } else {
                AppendMenu(hmenu, MF_STRING, id++, si.Caption);
            }
        }
        p += lstrlen(p) + 1;
    }

    GlobalUnlock(hBuf);
    GlobalFree(hBuf);
    return hmenu;
}

/* -----------------------------------------------------------------------
 * Public interface
 * ----------------------------------------------------------------------- */

void StartMenu_Load(HWND hwndOwner)
{
    /* START.INI is already referenced via g_szStartFile.
       No pre-loading needed; BuildMenu reads it on demand. */
}

void StartMenu_Show(HWND hwndOwner, int x, int y)
{
    HMENU hmenu = BuildMenu(g_hInst, "Start");
    if (!hmenu) {
        return;
    }

    /* Track popup. We use a modeless approach: TrackPopupMenu
       blocks until dismissed on Win16. */
    {
        UINT nFlags = TPM_LEFTALIGN | TPM_BOTTOMALIGN;
        if (g_TopTaskbar) {
            nFlags = TPM_LEFTALIGN | TPM_TOPALIGN;
        }
        TrackPopupMenu(hmenu, nFlags | TPM_RIGHTBUTTON,
                       x, y, 0, hwndOwner, NULL);
    }
    DestroyMenu(hmenu);
}

void StartMenu_Configure(HWND hwndOwner)
{
    /* Nothing persistent to reconfigure at runtime */
}

void StartMenu_RunStartup(HWND hwndOwner)
{
    HANDLE    hBuf;
    char FAR *buf;
    LPCSTR    p;

    hBuf = GlobalAlloc(GHND, 4096);
    if (!hBuf) {
        return;
    }
    buf = (char FAR *)GlobalLock(hBuf);

    GetPrivateProfileSection("Start\\&Programs\\Startup", buf,
                             4096, g_szStartFile);
    p = buf;
    while (p && *p) {
        LPCSTR eq = p;
        while (*eq && *eq != '=') eq++;
        if (*eq == '=') {
            char val[START_LINE_MAX], cmd[144];
            char *semi;
            lstrcpyn(val, eq + 1, sizeof(val));
            semi = val;
            while (*semi && *semi != ';') semi++;
            *semi = '\0';
            lstrcpy(cmd, val);
            if (*cmd && cmd[0] != '<')   /* skip placeholder items */
                WinExec(cmd, SW_SHOWNORMAL);
        }
        p += lstrlen(p) + 1;
    }

    GlobalUnlock(hBuf);
    GlobalFree(hBuf);
}
