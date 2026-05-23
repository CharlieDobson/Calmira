/***************************************************************************
 *
 *  HOOKS.H - WNDHOOKS.DLL runtime loader
 *  Translated from HOOKS.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Loads WNDHOOKS.DLL by full path and resolves all function pointers
 *  into a global WNDHOOKS structure (declared in WNDHOOKS\WNDHOOKS.H).
 *
 ***************************************************************************/

#ifndef HOOKS_H
#define HOOKS_H

#include <windows.h>
#include "..\WNDHOOKS\WNDHOOKS.H"

/* Global hook function table - filled by Hooks_Load(), cleared by Hooks_Free() */
extern WNDHOOKS g_Hooks;
extern HINSTANCE g_hWndHooksDll;

/* Load WNDHOOKS.DLL from the same directory as CALMIRA.EXE.
   Returns TRUE on success.  On failure g_Hooks members are all NULL. */
BOOL Hooks_Load(void);

/* Unload the DLL (called at shutdown) */
void Hooks_Free(void);

/* Convenience: TRUE if the DLL was loaded successfully */
#define HOOKS_OK()  (g_hWndHooksDll != 0)

#endif /* HOOKS_H */
