/***************************************************************************
 *
 *  CALMIRA.CPP - Calmira Shell - Main entry point (WinMain)
 *  Translated from CALMIRA.DPR (Delphi/Object Pascal)
 *
 *  Calmira shell for Microsoft Windows(TM) 3.1
 *  Copyright (C) 1998-2002 Calmira Online!
 *  Copyright (C) 1997-1998 Li-Hsin Huang
 *  C++ port for MSVC 1.52c / Windows 3.1 16-bit
 *  Copyright (c) 2026 Charlie Dobson
 *
 *  Startup sequence (mirrors CALMIRA.DPR):
 *    1.  Single-instance check via FindWindow
 *    2.  Logon dialog if multi-user enabled
 *    3.  Initialize paths and INI file  (Settings_Init)
 *    4.  Load all settings from INI     (LoadSettings)
 *    5.  Load shared bitmaps / cursors  (LoadResources)
 *    6.  Show splash screen
 *    7.  Create Desktop, StartMenu, Computer, Bin, Taskbar
 *    8.  Load desktop items and start menu
 *    9.  Execute WIN.INI Load= / Run= programs (if shell)
 *   10.  Execute Startup folder
 *   11.  Execute command-line argument
 *   12.  Message loop
 *   13.  ExitWindows if running as shell
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include "..\CALMSGS.H"
#include "RESOURCE.H"
#include "LOCALE.H"
#include "SETTINGS.H"

/* Forward declarations for subsystem modules */
#include "DESK.H"
#include "TASK.H"
#include "START.H"
#include "COMPSYS.H"
#include "WASTEBN.H"
#include "SPLASH.H"
#include "ABOUT.H"
#include "HOOKS.H"
#include "HELPERS.H"
#include "DDESHELL.H"

/* ------------------------------------------------------------------
 * Application-wide globals
 * ------------------------------------------------------------------ */
HINSTANCE g_hInst    = 0;
UINT      g_wmCalmira = 0;    /* RegisterWindowMessage result */

/* Shared resources (loaded by LoadAppResources, freed at exit) */
HBITMAP   g_hbmShortArrow = 0;   /* shortcut overlay arrow    */
HBITMAP   g_hbmAliasArrow = 0;   /* alias overlay arrow       */
HBITMAP   g_hbmSizeBox    = 0;   /* desktop size-grip corner  */

/* Window handles for the main subsystems */
HWND g_hwDesk    = 0;   /* Desktop (DESK_CLASS shortcut manager)    */
HWND g_hwMain    = 0;   /* Hidden CALMIRA_CLASS message-sink window */
HWND g_hwTaskbar = 0;   /* Taskbar                                  */
HWND g_hwComputer= 0;   /* Computer / file-manager window           */
HWND g_hwBin     = 0;   /* Recycle bin window                       */
HWND g_hwSplash  = 0;   /* Splash screen (hidden)                   */

/* ------------------------------------------------------------------
 * LoadAppResources - load shared bitmaps from the EXE resources
 * Corresponds to LoadResources in RESOURCE.PAS
 * ------------------------------------------------------------------ */
static BOOL LoadAppResources(void)
{
    g_hbmShortArrow = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SHORTARROW));
    g_hbmAliasArrow = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_ALIASARROW));
    g_hbmSizeBox    = LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_SIZEBOX));
    return (g_hbmShortArrow && g_hbmAliasArrow && g_hbmSizeBox);
}

static void FreeAppResources(void)
{
    if (g_hbmShortArrow) { DeleteObject(g_hbmShortArrow); g_hbmShortArrow = 0; }
    if (g_hbmAliasArrow) { DeleteObject(g_hbmAliasArrow); g_hbmAliasArrow = 0; }
    if (g_hbmSizeBox)    { DeleteObject(g_hbmSizeBox);    g_hbmSizeBox    = 0; }
}

/* ------------------------------------------------------------------
 * LoadAndRun - execute WIN.INI Load= and Run= programs
 * Corresponds to LoadAndRun in CALMIRA.DPR
 * ------------------------------------------------------------------ */
static void LoadAndRun(void)
{
    char line[512], token[144];
    LPCSTR p;
    int i;

    /* Load= (minimized) */
    GetProfileString("Windows", "Load", "", line, sizeof(line));
    p = line;
    while (*p) {
        while (*p == ' ') p++;
        if (!*p) {
            break;
        }
        i = 0;
        while (*p && *p != ' ' && i < sizeof(token)-1)
            token[i++] = *p++;
        token[i] = '\0';
        if (i > 0) {
            WinExec(token, SW_SHOWMINNOACTIVE);
        }
    }

    /* Run= (normal) */
    GetProfileString("Windows", "Run", "", line, sizeof(line));
    p = line;
    while (*p) {
        while (*p == ' ') p++;
        if (!*p) {
            break;
        }
        i = 0;
        while (*p && *p != ' ' && i < sizeof(token)-1)
            token[i++] = *p++;
        token[i] = '\0';
        if (i > 0) {
            WinExec(token, SW_SHOWNORMAL);
        }
    }
}

/* ------------------------------------------------------------------
 * RunCmdLine - execute command-line parameter passed to Calmira
 * Corresponds to RunCmdLine in CALMIRA.DPR
 * ------------------------------------------------------------------ */
static void RunCmdLine(LPCSTR lpCmd)
{
    char prog[144], args[256];
    LPCSTR p;
    int i;
    if (!lpCmd || !*lpCmd) {
        return;
    }

    /* First token = program */
    p = lpCmd;
    while (*p == ' ') p++;
    i = 0;
    while (*p && *p != ' ' && i < sizeof(prog)-1) prog[i++] = *p++;
    prog[i] = '\0';
    while (*p == ' ') p++;
    lstrcpyn(args, p, sizeof(args));

    if (i > 0) {
        WinExec(prog, SW_SHOW);
    }
}

/* ------------------------------------------------------------------
 * Main window procedure for the "hidden" Calmira application window.
 * This window receives WM_CALMIRA registered messages from the
 * launcher utilities (EXPLORER.EXE, CTASKMAN.EXE, SHOWDESK.EXE).
 * ------------------------------------------------------------------ */
LRESULT CALLBACK __export CalmiraWndProc(HWND hwnd, UINT msg,
                                          WPARAM wParam, LPARAM lParam)
{
    if (msg == g_wmCalmira) {
        switch ((int)wParam) {
            case CM_PREVINSTANCE: {
                /* Another instance started - bring us to front */
                if (g_hwComputer) {
                    SetForegroundWindow(g_hwComputer);
                }
                return 0;
            }
            case CM_EXPLORER: {
                /* Open / navigate the Computer window */
                if (g_hwComputer) {
                    if (lParam) {
                        /* lParam is a global atom containing the path */
                        char path[144];
                        GlobalGetAtomName((ATOM)lParam, path, sizeof(path));
                        GlobalDeleteAtom((ATOM)lParam);
                        Computer_NavigateTo(g_hwComputer, path);
                    } else {
                        ShowWindow(g_hwComputer, SW_SHOWNORMAL);
                        SetForegroundWindow(g_hwComputer);
                    }
                }
                return 0;
            }
            case CM_TASKMAN: {
                /* Show task manager */
                Taskbar_ShowTaskManager(g_hwTaskbar);
                return 0;
            }
            case CM_TOGGLEDESKTOP: {
                /* Minimize / restore all desktop icon windows */
                Desktop_ToggleAll(g_hwDesk);
                return 0;
            }
            case CM_STARTPROP: {
                /* Open Start menu properties dialog */
                Taskbar_ShowStartProp(g_hwTaskbar);
                return 0;
            }
        }
        return 0;
    }

    switch (msg) {
        case WM_SETTINGSCHANGED: {
            /* Propagate to subsystems */
            Desktop_OnSettingsChanged(g_hwDesk,  (TSettingChanges)wParam);
            Taskbar_OnSettingsChanged(g_hwTaskbar,(TSettingChanges)wParam);
            return 0;
        }

        case WM_DESTROY: {
            /* App lifetime is owned by DeskWndProc; no PostQuitMessage here. */
            return 0;
        }

        /* -----------------------------------------------------------
         * PROGMAN DDE shell interface.
         * Installers open a conversation with service="PROGMAN",
         * topic="PROGMAN" and send [CreateGroup]/[AddItem]/... commands.
         * ----------------------------------------------------------- */
        case WM_DDE_INITIATE: {
            DdeShell_OnInitiate(hwnd, (HWND)wParam,
                                LOWORD(lParam), HIWORD(lParam));
            return 0;
        }

        case WM_DDE_EXECUTE: {
            DdeShell_OnExecute(hwnd, (HWND)wParam, (HANDLE)lParam);
            return 0;
        }

        case WM_DDE_TERMINATE: {
            DdeShell_OnTerminate(hwnd, (HWND)wParam);
            return 0;
        }
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

/* ------------------------------------------------------------------
 * RegisterWindowClasses - register all custom window classes
 * ------------------------------------------------------------------ */
static BOOL RegisterWindowClasses(HINSTANCE hInst)
{
    WNDCLASS wc;

    /* Main application (hidden) window - receives registered messages */
    wc.style         = 0;
    wc.lpfnWndProc   = CalmiraWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_CALMIRA));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = CALMIRA_CLASS;
    if (!RegisterClass(&wc)) {
        return FALSE;
    }

    /* Register subsystem window classes */
    if (!Desktop_RegisterClass(hInst)) {
        return FALSE;
    }
    if (!Taskbar_RegisterClass(hInst)) {
        return FALSE;
    }
    if (!Computer_RegisterClass(hInst)) {
        return FALSE;
    }
    if (!Bin_RegisterClass(hInst)) {
        return FALSE;
    }

    return TRUE;
}

/* ------------------------------------------------------------------
 * WinMain
 * ------------------------------------------------------------------ */
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
                   LPSTR lpCmdLine, int nCmdShow)
{
    MSG  msg;
    UINT errMode;

    g_hInst     = hInst;
    g_wmCalmira = RegisterWindowMessage(CALMIRA_REGMSG);

    /* -----------------------------------------------------------
     * Single-instance guard: if Calmira is already running,
     * notify it and exit immediately.
     * ----------------------------------------------------------- */
    if (hPrevInst) {
        HWND hwPrev = FindWindow(CALMIRA_CLASS, CALMIRA_TITLE);
        if (hwPrev) {
            PostMessage(hwPrev, g_wmCalmira, CM_PREVINSTANCE, 0L);
        }
        return 0;
    }

    /* -----------------------------------------------------------
     * Initialize paths, INI file, and all settings.
     * ----------------------------------------------------------- */
    Settings_Init();
    LoadSettings();

    /* -----------------------------------------------------------
     * Suppress Windows error dialog boxes during startup.
     * ----------------------------------------------------------- */
    errMode = SetErrorMode(SEM_FAILCRITICALERRORS);

    /* -----------------------------------------------------------
     * Load shared bitmaps / cursors.
     * ----------------------------------------------------------- */
    if (!LoadAppResources()) {
        SetErrorMode(errMode);
        MessageBox(NULL, "Failed to load required resources.",
                   CALMIRA_TITLE, MB_ICONSTOP | MB_OK);
        Settings_Done();
        return 1;
    }

    /* -----------------------------------------------------------
     * Register window classes for all subsystems.
     * ----------------------------------------------------------- */
    if (!RegisterWindowClasses(hInst)) {
        SetErrorMode(errMode);
        FreeAppResources();
        Settings_Done();
        return 1;
    }

    /* -----------------------------------------------------------
     * Create the hidden CALMIRA message-sink window.
     * External launchers locate it via FindWindow(CALMIRA_CLASS).
     * ----------------------------------------------------------- */
    g_hwMain = CreateWindow(CALMIRA_CLASS, CALMIRA_TITLE,
                            WS_OVERLAPPED,
                            -9999, -9999, 1, 1,
                            NULL, NULL, hInst, NULL);
    if (!g_hwMain) {
        SetErrorMode(errMode);
        FreeAppResources();
        Settings_Done();
        return 1;
    }

    /* -----------------------------------------------------------
     * Load WNDHOOKS.DLL before creating subsystem windows.
     * Non-fatal if absent; all callers are guarded by HOOKS_OK().
     * Must be called before Desktop_Create so that the desktop
     * hook is attached when the window is first created.
     * ----------------------------------------------------------- */
    Hooks_Load();

    /* -----------------------------------------------------------
     * Create the Desktop window (DESK_CLASS).
     * This window holds DESKSTATE in its extra bytes and owns all
     * shortcut icon windows.  g_hwDesk is now the proper DESK_CLASS
     * window, not the CALMIRA_CLASS message-sink.
     * ----------------------------------------------------------- */
    g_hwDesk = Desktop_Create(hInst, g_hwMain);
    if (!g_hwDesk) {
        SetErrorMode(errMode);
        DestroyWindow(g_hwMain);
        FreeAppResources();
        Settings_Done();
        return 1;
    }

    /* -----------------------------------------------------------
     * Show splash screen (if enabled).
     * ----------------------------------------------------------- */
    if (g_ShowSplash) {
        g_hwSplash = Splash_Create(hInst, g_hwMain);
        if (g_hwSplash) {
            ShowWindow(g_hwSplash, SW_SHOW);
            UpdateWindow(g_hwSplash);
        }
    }

    /* -----------------------------------------------------------
     * Create the main subsystem windows.
     * Ordering matters: Desktop before Taskbar, Computer before Bin.
     * ----------------------------------------------------------- */
    g_hwComputer = Computer_Create(hInst, g_hwMain);
    g_hwBin      = Bin_Create(hInst, g_hwMain);
    g_hwTaskbar  = Taskbar_Create(hInst, g_hwMain);

    /* -----------------------------------------------------------
     * Load desktop shortcuts and start menu items.
     * ----------------------------------------------------------- */
    Desktop_Load(g_hwDesk);
    StartMenu_Load(g_hwDesk);

    /* -----------------------------------------------------------
     * Restore error mode; release the splash.
     * ----------------------------------------------------------- */
    SetErrorMode(errMode);
    if (g_hwSplash) {
        ShowWindow(g_hwSplash, SW_HIDE);
        /* Move far off-screen so it still exists as the "main" window
           placeholder but is never visible. */
        SetWindowPos(g_hwSplash, NULL, -9999, -9999, 1, 1,
                     SWP_NOZORDER | SWP_NOACTIVATE);
    }

    /* -----------------------------------------------------------
     * Execute WIN.INI Load= / Run= (shell mode only).
     * ----------------------------------------------------------- */
    if (g_IsShell) {
        LoadAndRun();
    }

    /* -----------------------------------------------------------
     * Execute startup folder items.
     * ----------------------------------------------------------- */
    if ((g_IsShell || !g_ShellStartup) &&
        !(GetAsyncKeyState(VK_SHIFT) & 0x8000))
        StartMenu_RunStartup(g_hwDesk);

    /* -----------------------------------------------------------
     * Execute command-line parameter.
     * ----------------------------------------------------------- */
    RunCmdLine(lpCmdLine);

    /* -----------------------------------------------------------
     * First-run about dialog.
     * ----------------------------------------------------------- */
    if (g_FirstRun) {
        About_ShowDialog(hInst, g_hwDesk);
    }

    /* -----------------------------------------------------------
     * Optionally restore the Computer window.
     * ----------------------------------------------------------- */
    if (g_RestoreSys && g_hwComputer) {
        ShowWindow(g_hwComputer, SW_SHOWNORMAL);
    }

    /* -----------------------------------------------------------
     * Main message loop.
     * ----------------------------------------------------------- */
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    /* -----------------------------------------------------------
     * Shutdown.
     * ----------------------------------------------------------- */
    FreeAppResources();
    Settings_Done();

    if (g_IsShell)
        ExitWindows(0, 0);   /* terminate Windows if we were the shell */

    return (int)msg.wParam;
}
