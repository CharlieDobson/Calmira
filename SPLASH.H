/***************************************************************************
 *
 *  SPLASH.H - Splash screen declarations
 *  Translated from SPLASH.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#ifndef SPLASH_H
#define SPLASH_H

#include <windows.h>

/* Create and show the splash screen.  Returns window handle. */
HWND Splash_Create(HINSTANCE hInst, HWND hwndParent);

/* Hide and move off-screen (keeps it alive as a placeholder window) */
void Splash_Hide(HWND hwndSplash);

/* Set the status text shown at the bottom of the splash */
void Splash_SetStatus(HWND hwndSplash, LPCSTR szText);

#endif /* SPLASH_H */
