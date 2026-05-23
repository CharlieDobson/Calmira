/***************************************************************************
 *
 *  TASK.H - Taskbar window declarations
 *  Translated from TASK.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  The Taskbar is a fixed strip at the bottom (or top) of the screen.
 *  It contains:
 *   - The Start button (left)
 *   - Task buttons for each top-level application window
 *   - Applet tray programs (optional icons in right side)
 *   - A digital clock (far right)
 *
 *  It hooks into WNDHOOKS.DLL to receive:
 *   WM_SHELLWNDCREATE  - new window appeared (add task button)
 *   WM_SHELLWNDDESTROY - window closed (remove task button)
 *   WM_WINACTIVE       - window activated (highlight its button)
 *   WM_MOUSEHOOK       - cursor moved near taskbar edge (auto-hide)
 *
 ***************************************************************************/

#ifndef TASK_H
#define TASK_H

#include <windows.h>
#include "SETTINGS.H"
#include "..\CALMSGS.H"

/* Window class name */
#define TASKBAR_CLASS   "CalmiraTaskbar"

/* Default taskbar height in pixels (two rows of buttons + border) */
#define TASKBAR_HEIGHT  28

/* Maximum task buttons */
#define MAX_TASKS  64

/* ------------------------------------------------------------------
 * Task button descriptor
 * ------------------------------------------------------------------ */
typedef struct tagTASKBTN {
    HWND  hwnd;          /* the window this button represents */
    char  szCaption[64]; /* button label (may be truncated)   */
    HICON hIcon;         /* small (16x16) icon for the window */
    BOOL  bActive;       /* TRUE when this window is foreground */
} TASKBTN;

/* ------------------------------------------------------------------
 * Public interface
 * ------------------------------------------------------------------ */

BOOL Taskbar_RegisterClass(HINSTANCE hInst);
HWND Taskbar_Create(HINSTANCE hInst, HWND hwndParent);

/* Called when settings change */
void Taskbar_OnSettingsChanged(HWND hwndTask, TSettingChanges changes);

/* Position the taskbar and notify WNDHOOKS of the screen limit */
void Taskbar_Configure(HWND hwndTask);

/* Show the Windows task manager (for CM_TASKMAN) */
void Taskbar_ShowTaskManager(HWND hwndTask);

/* Open Start Menu properties dialog (for CM_STARTPROP) */
void Taskbar_ShowStartProp(HWND hwndTask);

/* Called by WNDHOOKS callbacks to add/remove/activate task buttons */
void Taskbar_AddWindow(HWND hwndTask, HWND hwndApp);
void Taskbar_RemoveWindow(HWND hwndTask, HWND hwndApp);
void Taskbar_ActivateWindow(HWND hwndTask, HWND hwndApp, BOOL bActive);

/* Timer ID used for clock updates */
#define TIMER_CLOCK  1
#define TIMER_HIDE   2

#endif /* TASK_H */
