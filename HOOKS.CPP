/***************************************************************************
 *
 *  HOOKS.CPP - WNDHOOKS.DLL runtime loader
 *  Translated from HOOKS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include <string.h>
#include "HOOKS.H"
#include "SETTINGS.H"

WNDHOOKS  g_Hooks;
HINSTANCE g_hWndHooksDll = 0;

BOOL Hooks_Load(void)
{
    char szPath[144];
    HINSTANCE hDll;

    memset(&g_Hooks, 0, sizeof(g_Hooks));

    /* Build full path: same directory as CALMIRA.EXE + WNDHOOKS.DLL */
    lstrcpy(szPath, g_szAppPath);
    lstrcat(szPath, "WNDHOOKS.DLL");

    hDll = LoadLibrary(szPath);
    if ((UINT)hDll < 32) {
        /* Try just the name (relies on PATH) */
        hDll = LoadLibrary("WNDHOOKS.DLL");
        if ((UINT)hDll < 32) {
            return FALSE;
        }
    }

#define GP(name) GetProcAddress(hDll, name)

    g_Hooks.StartTaskMonitor    = (PFNSTARTTASKMON)   GP("StartTaskMonitor");
    g_Hooks.StopTaskMonitor     = (PFNSTOPTASKMON)    GP("StopTaskMonitor");
    g_Hooks.SetWndHook          = (PFNSETWNDHOOK)     GP("SetWndHook");
    g_Hooks.UnhookWndHook       = (PFNUNHOOKWNDHOOK)  GP("UnhookWndHook");
    g_Hooks.SetYLimit           = (PFNSETYLIMIT)      GP("SetYLimit");
    g_Hooks.StartMouseMonitor   = (PFNSTARTMOUSEMON)  GP("StartMouseMonitor");
    g_Hooks.StopMouseMonitor    = (PFNSTOPMOUSEMON)   GP("StopMouseMonitor");
    g_Hooks.EnableMouseMonitor  = (PFNENABLEMOUSEMON) GP("EnableMouseMonitor");
    g_Hooks.DisableMouseMonitor = (PFNDISABLEMOUSEMON)GP("DisableMouseMonitor");
    g_Hooks.SetCallBackWnd      = (PFNSETCALLBACKWND) GP("SetCallBackWnd");
    g_Hooks.SetMaxEnabled       = (PFNSETMAXENABLED)  GP("SetMaxEnabled");
    g_Hooks.SetRCloseEnabled    = (PFNSETRCLOSEENABLED)GP("SetRCloseEnabled");
    g_Hooks.SetDesktopHook      = (PFNSETDESKTOPHOOK) GP("SetDesktopHook");
    g_Hooks.ReleaseDesktopHook  = (PFNRELEASEDESKTOPHOOK)GP("ReleaseDesktopHook");
    g_Hooks.StartKeyboardHook   = (PFNSTARTKBHOOK)    GP("StartKeyboardHook");
    g_Hooks.StopKeyboardHook    = (PFNSTOPKBHOOK)     GP("StopKeyboardHook");
    g_Hooks.SetRButtonUpClose   = (PFNSETRBUTTONUPCLOSE)GP("SetRButtonUpClose");
    g_Hooks.IsHotKey            = (PFNISHOTKEY)       GP("IsHotKey");
    g_Hooks.SetWin95KeyEnabled  = (PFNSETWIN95KEY)    GP("SetWin95KeyEnabled");
    g_Hooks.StartFileMonitor    = (PFNSTARTFILEMON)   GP("StartFileMonitor");
    g_Hooks.StopFileMonitor     = (PFNSTOPFILEMON)    GP("StopFileMonitor");

#undef GP

    /* Only mark hooks as available if ALL entry points loaded.
       g_hWndHooksDll must be set after validation so HOOKS_OK() is
       consistent with whether all function pointers are valid. */
    if (g_Hooks.StartTaskMonitor  == NULL ||
        g_Hooks.SetWndHook        == NULL ||
        g_Hooks.StartMouseMonitor == NULL ||
        g_Hooks.SetCallBackWnd    == NULL ||
        g_Hooks.SetDesktopHook    == NULL ||
        g_Hooks.SetRCloseEnabled  == NULL ||
        g_Hooks.SetRButtonUpClose == NULL) {
        FreeLibrary(hDll);
        memset(&g_Hooks, 0, sizeof(g_Hooks));
        return FALSE;
    }
    g_hWndHooksDll = hDll;
    return TRUE;
}

void Hooks_Free(void)
{
    if (g_hWndHooksDll) {
        FreeLibrary(g_hWndHooksDll);
        g_hWndHooksDll = 0;
    }
    memset(&g_Hooks, 0, sizeof(g_Hooks));
}
