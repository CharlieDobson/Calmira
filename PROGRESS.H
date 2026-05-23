/***************************************************************************
 *  PROGRESS.H - Modeless progress dialog
 *  Translated from PROGRESS.PAS
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 ***************************************************************************/
#ifndef PROGRESS_H
#define PROGRESS_H
#include <windows.h>

/* Create (but don't show) the progress dialog.  Call once at startup. */
HWND Progress_Create(HINSTANCE hInst, HWND hwndParent);

/* Show the dialog with an operation name */
void Progress_Begin(HWND hwndProg, LPCSTR szOp);

/* Update the filename label and progress bar (0-100) */
void Progress_Update(HWND hwndProg, LPCSTR szFile, int nPercent);

/* Hide the dialog */
void Progress_End(HWND hwndProg);

/* Returns TRUE if the user pressed Abort */
BOOL Progress_Aborted(HWND hwndProg);

#endif
