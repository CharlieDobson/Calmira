/***************************************************************************
 *
 *  HELPERS.H - Win16 / MSVC 1.52c compatibility helpers
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Provides replacements for Win32 APIs not present in Win16,
 *  and common drawing / string helpers used across multiple modules.
 *
 ***************************************************************************/

#ifndef HELPERS_H
#define HELPERS_H

#include <windows.h>
#include <windowsx.h>
#include <string.h>
#include <stdio.h>
#include <dos.h>
#include <direct.h>

/* ------------------------------------------------------------------
 * SetForegroundWindow - Win32 only.
 * Win16 equivalent: BringWindowToTop + SetActiveWindow.
 * ------------------------------------------------------------------ */
#define SetForegroundWindow(hwnd)  \
    do { BringWindowToTop(hwnd); SetActiveWindow(hwnd); } while(0)

/* ------------------------------------------------------------------
 * SYSTEMTIME / GetLocalTime - Win32 only.
 * SYSTEMTIME is not in the Win16 API; neither is GetSystemTime.
 * Provide the struct and implement GetLocalTime via C runtime localtime().
 * ------------------------------------------------------------------ */
#include <time.h>

#ifndef _SYSTEMTIME_
#define _SYSTEMTIME_
typedef struct tagSYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;
typedef SYSTEMTIME FAR *LPSYSTEMTIME;
#endif

static void GetLocalTime(LPSYSTEMTIME pst)
{
    time_t t;
    struct tm *lt;
    time(&t);
    lt = localtime(&t);
    if (pst && lt) {
        pst->wYear         = (WORD)(lt->tm_year + 1900);
        pst->wMonth        = (WORD)(lt->tm_mon  + 1);
        pst->wDayOfWeek    = (WORD)lt->tm_wday;
        pst->wDay          = (WORD)lt->tm_mday;
        pst->wHour         = (WORD)lt->tm_hour;
        pst->wMinute       = (WORD)lt->tm_min;
        pst->wSecond       = (WORD)lt->tm_sec;
        pst->wMilliseconds = 0;
    }
}

/* ------------------------------------------------------------------
 * GetSysColorBrush - not in MSVC 1.52c import libs.
 * Implemented in HELPERS.CPP (single shared cache; callers must not
 * delete the returned handle, matching the Win32 contract).
 * ------------------------------------------------------------------ */
HBRUSH GetSysColorBrush(int nIndex);

/* ------------------------------------------------------------------
 * GetForegroundWindow - Win32 only.
 * Win16 equivalent: GetActiveWindow (returns the active top-level window).
 * ------------------------------------------------------------------ */
#define GetForegroundWindow()  GetActiveWindow()

/* ------------------------------------------------------------------
 * lstrstr - not in Win16 API, use _fstrstr from C runtime.
 * ------------------------------------------------------------------ */
#define lstrstr(s, sub)   _fstrstr((s), (sub))

/* ------------------------------------------------------------------
 * GCW_HICONSM - Win32 only.  On Win16 there is only one icon.
 * ------------------------------------------------------------------ */
#ifndef GCW_HICONSM
#define GCW_HICONSM  GCW_HICON
#endif

/* ------------------------------------------------------------------
 * GetLogicalDrives - Win32 only.
 * Win16: iterate drive letters using GetDriveType (0-based index).
 * Returns a bitmask; bit 0 = A, bit 1 = B, bit 2 = C, etc.
 * ------------------------------------------------------------------ */
static DWORD GetLogicalDrives(void)
{
    DWORD mask = 0;
    int   i;
    for (i = 0; i < 26; i++)
        if (GetDriveType(i) > 1)   /* > 1 means root exists */
            mask |= (1UL << i);
    return mask;
}

/* ------------------------------------------------------------------
 * HWND_TOPMOST / HWND_NOTOPMOST - Win32 only.
 * Win16 has no true always-on-top; map to HWND_TOP so the window
 * is brought to the top of the normal z-order instead.
 * ------------------------------------------------------------------ */
#ifndef HWND_TOPMOST
#define HWND_TOPMOST    HWND_TOP
#define HWND_NOTOPMOST  HWND_TOP
#endif

/* ------------------------------------------------------------------
 * SS_SUNKEN - Win32/Win95 static control style; not in Win16.
 * Define as 0: status bars compile normally, just without the
 * sunken border visual (Win16 draws the control flat).
 * ------------------------------------------------------------------ */
#ifndef SS_SUNKEN
#define SS_SUNKEN  0
#endif

/* ------------------------------------------------------------------
 * TPM_BOTTOMALIGN / TPM_TOPALIGN - Win32 TrackPopupMenu flags.
 * Win16 ignores unknown flag bits; define so code compiles unchanged.
 * ------------------------------------------------------------------ */
#ifndef TPM_BOTTOMALIGN
#define TPM_BOTTOMALIGN  0x0020
#define TPM_TOPALIGN     0x0040
#endif

/* ------------------------------------------------------------------
 * BST_CHECKED / BST_UNCHECKED - Win32 button state constants.
 * Win16 CheckDlgButton / IsDlgButtonChecked use 1/0 directly.
 * ------------------------------------------------------------------ */
#ifndef BST_CHECKED
#define BST_CHECKED    1
#define BST_UNCHECKED  0
#endif

/* ------------------------------------------------------------------
 * GetPrivateProfileSection - Win32 only.
 * Win16: enumerate key names with GetPrivateProfileString(sect,NULL,...)
 * then fetch each value individually, building a "key=val\0..." block.
 * ------------------------------------------------------------------ */
static int GetPrivateProfileSection(LPCSTR szSect, LPSTR szBuf,
                                    int nSize, LPCSTR szFile)
{
    char  keys[2048];
    LPCSTR k;
    LPSTR  out = szBuf;
    int    rem = nSize - 2;

    GetPrivateProfileString(szSect, NULL, "", keys, sizeof(keys), szFile);
    k = keys;
    while (*k && rem > 0) {
        char val[512];
        int  kLen, vLen, total;
        kLen = lstrlen(k);
        GetPrivateProfileString(szSect, k, "", val, sizeof(val), szFile);
        vLen  = lstrlen(val);
        total = kLen + 1 + vLen + 1;
        if (total > rem) {
            break;
        }
        lstrcpy(out, k);
        out[kLen] = '=';
        lstrcpy(out + kLen + 1, val);
        out += total;
        rem -= total;
        k   += kLen + 1;
    }
    *out++ = '\0';
    *out   = '\0';
    return (int)(out - szBuf);
}

/* ------------------------------------------------------------------
 * MoveToEx - Win32 name.  Win16 uses MoveTo() which returns DWORD.
 * Defined here before Draw3dRect which calls it.
 * ------------------------------------------------------------------ */
#ifndef MoveToEx
#define MoveToEx(hdc, x, y, pPt)  MoveTo(hdc, x, y)
#endif

/* ------------------------------------------------------------------
 * Draw3dRect - manual Win16 3D rectangle border.
 * top-left highlight, bottom-right shadow.
 * ------------------------------------------------------------------ */
static void Draw3dRect(HDC hdc, int x, int y, int w, int h,
                       COLORREF clrHi, COLORREF clrLo)
{
    HPEN hHi  = CreatePen(PS_SOLID, 1, clrHi);
    HPEN hLo  = CreatePen(PS_SOLID, 1, clrLo);
    HPEN hOld;
    hOld = (HPEN)SelectObject(hdc, hHi);
    MoveToEx(hdc, x,       y+h-1, NULL); LineTo(hdc, x,     y);
    MoveToEx(hdc, x,       y,     NULL); LineTo(hdc, x+w-1, y);
    SelectObject(hdc, hLo);
    MoveToEx(hdc, x+w-1,   y,     NULL); LineTo(hdc, x+w-1, y+h-1);
    MoveToEx(hdc, x+w-1,   y+h-1, NULL); LineTo(hdc, x-1,   y+h-1);
    SelectObject(hdc, hOld);
    DeleteObject(hHi);
    DeleteObject(hLo);
}

/* ------------------------------------------------------------------
 * DrawEdge - Win32 only.  Simple Win16 fallback.
 * ------------------------------------------------------------------ */
#ifndef DrawEdge
#define EDGE_SUNKEN  0x06
#define BF_RECT      0x0F
static void DrawEdge(HDC hdc, LPRECT prc, UINT edge, UINT flags)
{
    COLORREF hiColor = (edge & 0x04) ? GetSysColor(COLOR_BTNSHADOW)
                                     : GetSysColor(COLOR_BTNHIGHLIGHT);
    COLORREF loColor = (edge & 0x04) ? GetSysColor(COLOR_BTNHIGHLIGHT)
                                     : GetSysColor(COLOR_BTNSHADOW);
    Draw3dRect(hdc, prc->left, prc->top,
               prc->right - prc->left, prc->bottom - prc->top,
               hiColor, loColor);
}
#endif

/* ------------------------------------------------------------------
 * DrawButtonFace - draw a Win95-style button with a text label.
 * bPressed: TRUE for inset (depressed) appearance.
 * ------------------------------------------------------------------ */
static void DrawButtonFace(HDC hdc, const RECT FAR *prc,
                            BOOL bPressed, LPCSTR szText)
{
    RECT rc = *prc;
    COLORREF clrFace = GetSysColor(COLOR_BTNFACE);
    HBRUSH hBr = CreateSolidBrush(clrFace);
    FillRect(hdc, &rc, hBr);
    DeleteObject(hBr);
    Draw3dRect(hdc,
               rc.left, rc.top,
               rc.right - rc.left, rc.bottom - rc.top,
               bPressed ? GetSysColor(COLOR_BTNSHADOW)
                        : GetSysColor(COLOR_BTNHIGHLIGHT),
               bPressed ? GetSysColor(COLOR_BTNHIGHLIGHT)
                        : GetSysColor(COLOR_BTNSHADOW));
    if (szText && *szText) {
        RECT rcText = rc;
        if (bPressed) {
            rcText.left++;  rcText.top++;
            rcText.right++; rcText.bottom++;
        }
        SetBkMode(hdc, TRANSPARENT);
        SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
        DrawText(hdc, szText, -1, &rcText,
                 DT_CENTER | DT_VCENTER | DT_SINGLELINE |
                 DT_NOPREFIX);
    }
}

/* ------------------------------------------------------------------
 * GetDlgCtrlDC - get DC for a dialog control.
 * ------------------------------------------------------------------ */
#define GetDlgCtrlDC(hwndCtrl)  GetDC(hwndCtrl)

/* ------------------------------------------------------------------
 * lstrcatn - bounded string concatenate (not in Win16 API).
 * ------------------------------------------------------------------ */
static void lstrcatn(LPSTR dst, LPCSTR src, int maxLen)
{
    int dlen = lstrlen(dst);
    int slen = lstrlen(src);
    int copy = min(slen, maxLen - dlen - 1);
    if (copy > 0) {
        _fmemcpy(dst + dlen, src, copy);
        dst[dlen + copy] = '\0';
    }
}

/* lstrcpyn - declared in windows.h (MSVC 1.52c Win16 SDK); no redefinition needed. */

/* ------------------------------------------------------------------
 * g_hInst - application instance handle (defined in CALMIRA.CPP).
 * Use in place of Win32-style GetModuleHandle(NULL).
 * ------------------------------------------------------------------ */
extern HINSTANCE g_hInst;

/* ------------------------------------------------------------------
 * CopyFileRaw - low-level binary file copy.
 * Declared here so both FILEMAN.CPP and WASTEBN.CPP can use it.
 * Implemented in FILEMAN.CPP.
 * ------------------------------------------------------------------ */
BOOL CopyFileRaw(LPCSTR szSrc, LPCSTR szDest);

/* ------------------------------------------------------------------
 * _far_qsort - shell sort for FAR arrays.
 * MSVC 1.52c has no _fqsort.  Comparison callback uses FAR PASCAL
 * so it can receive far pointers to array elements.
 * tmp[256] fits any struct used in this project (max TREENODE=166 B).
 * ------------------------------------------------------------------ */
typedef int (FAR PASCAL *FARCOMPARE)(const void FAR *, const void FAR *);

static void _far_qsort(void FAR *base, size_t num, size_t width,
                       FARCOMPARE cmp)
{
    char FAR *b = (char FAR *)base;
    char       tmp[256];
    size_t     gap, i, j;

    for (gap = num >> 1; gap > 0; gap >>= 1) {
        for (i = gap; i < num; i++) {
            _fmemcpy((void FAR *)tmp, b + i * width, width);
            j = i;
            while (j >= gap &&
                   cmp(b + (j - gap) * width, (void FAR *)tmp) > 0) {
                _fmemcpy(b + j * width, b + (j - gap) * width, width);
                j -= gap;
            }
            _fmemcpy(b + j * width, (void FAR *)tmp, width);
        }
    }
}

/* ------------------------------------------------------------------
 * Far-pointer wrappers for C runtime file functions.
 * In Win16 medium model, fopen/remove/rename/_dos_* take near char*
 * but LPCSTR parameters are far.  Copy path to a near buffer first.
 * Wrappers are defined before the #define overrides so the wrapper
 * bodies use the real C runtime functions, not themselves.
 * ------------------------------------------------------------------ */
static int _far_remove(LPCSTR p)
{ char b[144]; lstrcpyn(b,p,144); return remove(b); }

static int _far_rename(LPCSTR a, LPCSTR b_)
{ char na[144],nb[144]; lstrcpyn(na,a,144); lstrcpyn(nb,b_,144); return rename(na,nb); }

static FILE * _far_fopen(LPCSTR p, const char *m)
{ char b[144]; lstrcpyn(b,p,144); return fopen(b,m); }

static unsigned _far_dos_findfirst(LPCSTR p, unsigned a, struct _find_t *f)
{ char b[144]; lstrcpyn(b,p,144); return _dos_findfirst(b,a,f); }

static unsigned _far_dos_setfileattr(LPCSTR p, unsigned a)
{ char b[144]; lstrcpyn(b,p,144); return _dos_setfileattr(b,a); }

static int _far_rmdir(LPCSTR p)
{ char b[144]; lstrcpyn(b,p,144); return _rmdir(b); }

#define remove(p)              _far_remove((LPCSTR)(p))
#define rename(a,b)            _far_rename((LPCSTR)(a),(LPCSTR)(b))
#define fopen(p,m)             _far_fopen((LPCSTR)(p),(m))
#define _dos_findfirst(p,a,f)  _far_dos_findfirst((LPCSTR)(p),(a),(f))
#define _dos_setfileattr(p,a)  _far_dos_setfileattr((LPCSTR)(p),(a))
#define _rmdir(p)              _far_rmdir((LPCSTR)(p))

/* ------------------------------------------------------------------
 * _rmdir / _mkdir - may need direct.h on MSVC 1.52c
 * ------------------------------------------------------------------ */
#include <direct.h>

#endif /* HELPERS_H */
