/***************************************************************************
 *
 *  ICONIC.H - Icon item base declarations
 *  Translated from ICONIC.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  An ICONITEM represents one file, folder, or shortcut in an icon
 *  window (ICONWIN.CPP).  It knows how to draw itself in large icon,
 *  small icon, or details (list) view.
 *
 ***************************************************************************/

#ifndef ICONIC_H
#define ICONIC_H

#include <windows.h>
#include "SETTINGS.H"

/* ------------------------------------------------------------------
 * Item type flags
 * ------------------------------------------------------------------ */
#define ITYPE_FILE    0x00
#define ITYPE_DIR     0x01
#define ITYPE_ALIAS   0x02   /* .ALS alias file          */
#define ITYPE_SHORT   0x04   /* desktop shortcut (.LNK?) */

/* ------------------------------------------------------------------
 * An ICONITEM holds everything needed to display one entry in an
 * icon window.  The array of items is managed by ICONWIN.CPP.
 * ------------------------------------------------------------------ */
typedef struct tagICONITEM {
    char  szName[13];       /* 8.3 filename (NUL terminated)      */
    char  szDesc[64];       /* 4DOS description (empty if unused)  */
    DWORD dwSize;           /* file size in bytes (0 for dirs)     */
    WORD  wDate;            /* DOS packed date                     */
    WORD  wTime;            /* DOS packed time                     */
    BYTE  bAttr;            /* DOS file attributes                 */
    BYTE  bType;            /* ITYPE_* flags                       */
    HICON hIcon;            /* icon extracted from EXE/DLL         */
    BOOL  bSelected;        /* TRUE when item is selected          */
    int   nX;              /* icon position (large icon view)     */
    int   nY;
} ICONITEM;

/* ------------------------------------------------------------------
 * Column widths for details view (pixels, configurable)
 * ------------------------------------------------------------------ */
typedef struct tagCOLWIDTHS {
    int Name;
    int Size;
    int Type;
    int Date;
    int Attr;
    int Desc;
} COLWIDTHS;

/* ------------------------------------------------------------------
 * Drawing metrics
 * ------------------------------------------------------------------ */
#define LARGE_ICON_W   32   /* pixels per large icon cell (width)  */
#define LARGE_ICON_H   48   /* pixels per large icon cell (height) */
#define SMALL_ICON_W   16
#define SMALL_ICON_H   16
#define DETAILS_H      16   /* height of one detail row            */

/* ------------------------------------------------------------------
 * Public functions
 * ------------------------------------------------------------------ */

/* Draw a single item in the given view mode */
void  Iconic_DrawItem(HDC hdc, const ICONITEM FAR *pi,
                      int x, int y, TDisplayMode mode,
                      BOOL bFocused, HINSTANCE hInst);

/* Load (or look up from cache) the icon for a file */
HICON Iconic_GetIcon(LPCSTR szFullPath, BOOL bSmall);

/* Release an icon handle (decrements ref count in cache) */
void  Iconic_ReleaseIcon(HICON hIcon);

/* Compare two items for sorting */
int FAR PASCAL Iconic_Compare(const ICONITEM FAR *a,
                               const ICONITEM FAR *b,
                               TSortOrder order);

/* Calculate the default column widths from the global font */
void  Iconic_CalcColWidths(COLWIDTHS FAR *pcw);

/* Format a file size as a human-readable string (KB / MB) */
void  Iconic_FormatSize(DWORD dwSize, LPSTR buf, int bufLen);

/* Format a DOS packed date+time as "DD/MM/YYYY HH:MM" */
void  Iconic_FormatDate(WORD wDate, WORD wTime, LPSTR buf, int bufLen);

/* Format file attributes as "RHSA" string */
void  Iconic_FormatAttr(BYTE bAttr, LPSTR buf, int bufLen);

#endif /* ICONIC_H */
