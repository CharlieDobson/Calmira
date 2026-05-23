/***************************************************************************
 *
 *  ICONIC.CPP - Icon item drawing and utilities
 *  Translated from ICONIC.PAS (Delphi/Object Pascal)
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 ***************************************************************************/

#include <windows.h>
#include <shellapi.h>
#include <string.h>
#include "ICONIC.H"
#include "SETTINGS.H"
#include "DIRECTRY.H"
#include "HELPERS.H"

/* -----------------------------------------------------------------------
 * Icon cache: simple fixed-size table keyed by full path + index
 * ----------------------------------------------------------------------- */
#define ICON_CACHE_SIZE 128

typedef struct tagICONENTRY {
    char  szPath[144];
    int   nIndex;
    HICON hIcon;
    int   nRefs;
} ICONCACHEENTRY;

static ICONCACHEENTRY s_cache[ICON_CACHE_SIZE];
static int            s_cacheInited = FALSE;

static void CacheInit(void)
{
    if (!s_cacheInited) {
        memset(s_cache, 0, sizeof(s_cache));
        s_cacheInited = TRUE;
    }
}

HICON Iconic_GetIcon(LPCSTR szFullPath, BOOL bSmall)
{
    int   i, free = -1;
    HICON hIcon;
    CacheInit();
    for (i = 0; i < ICON_CACHE_SIZE; i++) {
        if (!s_cache[i].hIcon) { if (free < 0) free = i; continue; }
        if (lstrcmpi(s_cache[i].szPath, szFullPath) == 0) {
            s_cache[i].nRefs++;
            return s_cache[i].hIcon;
        }
    }
    /* Not cached - extract */
    hIcon = ExtractIcon(g_hInst, szFullPath, 0);
    if ((UINT)hIcon <= 1) {
        hIcon = NULL;
    }
    if (hIcon && free >= 0) {
        lstrcpyn(s_cache[free].szPath, szFullPath, sizeof(s_cache[free].szPath));
        s_cache[free].nIndex = 0;
        s_cache[free].hIcon  = hIcon;
        s_cache[free].nRefs  = 1;
    }
    return hIcon;
}

void Iconic_ReleaseIcon(HICON hIcon)
{
    int i;
    if (!hIcon) {
        return;
    }
    for (i = 0; i < ICON_CACHE_SIZE; i++) {
        if (s_cache[i].hIcon == hIcon) {
            if (--s_cache[i].nRefs <= 0) {
                DestroyIcon(s_cache[i].hIcon);
                memset(&s_cache[i], 0, sizeof(s_cache[i]));
            }
            return;
        }
    }
    /* Not in cache - just destroy */
    DestroyIcon(hIcon);
}

/* -----------------------------------------------------------------------
 * Formatting helpers
 * ----------------------------------------------------------------------- */
void Iconic_FormatSize(DWORD dwSize, LPSTR buf, int bufLen)
{
    if (dwSize >= 1048576UL) {
        wsprintf(buf, "%lu.%02lu MB", dwSize / 1048576UL,
                 (dwSize % 1048576UL) / 10486UL);
    } else if (dwSize >= 1024) {
        wsprintf(buf, "%lu KB", (dwSize + 512) / 1024);
    } else {
        wsprintf(buf, "%lu B", dwSize);
    }
}

void Iconic_FormatDate(WORD wDate, WORD wTime, LPSTR buf, int bufLen)
{
    int year  = ((wDate >> 9) & 0x7F) + 1980;
    int month = (wDate >> 5) & 0x0F;
    int day   =  wDate & 0x1F;
    int hour  = (wTime >> 11) & 0x1F;
    int min   = (wTime >>  5) & 0x3F;
    wsprintf(buf, "%02d/%02d/%04d %02d:%02d", day, month, year, hour, min);
}

void Iconic_FormatAttr(BYTE bAttr, LPSTR buf, int bufLen)
{
    int i = 0;
    if (bAttr & _A_RDONLY) {
        buf[i++] = 'R';
    }
    if (bAttr & _A_HIDDEN) {
        buf[i++] = 'H';
    }
    if (bAttr & _A_SYSTEM) {
        buf[i++] = 'S';
    }
    if (bAttr & _A_ARCH) {
        buf[i++] = 'A';
    }
    buf[i] = '\0';
}

/* -----------------------------------------------------------------------
 * Sort compare
 * ----------------------------------------------------------------------- */
int FAR PASCAL Iconic_Compare(const ICONITEM FAR *a, const ICONITEM FAR *b,
                               TSortOrder order)
{
    if ((a->bType & ITYPE_DIR) != (b->bType & ITYPE_DIR)) {
        return (a->bType & ITYPE_DIR) ? -1 : 1;
    }
    switch (order) {
        case soName: return lstrcmpi(a->szName, b->szName);
        case soType: {
            int r = lstrcmpi(Directory_GetExt(a->szName),
                             Directory_GetExt(b->szName));
            return r ? r : lstrcmpi(a->szName, b->szName);
        }
        case soSize: {
            if (a->dwSize < b->dwSize) {
                return -1;
            }
            if (a->dwSize > b->dwSize) {
                return  1;
            }
            return lstrcmpi(a->szName, b->szName);
        }
        case soDate: {
            DWORD da = MAKELONG(a->wTime, a->wDate);
            DWORD db = MAKELONG(b->wTime, b->wDate);
            if (da < db) {
                return  1;
            }
            if (da > db) {
                return -1;
            }
            return lstrcmpi(a->szName, b->szName);
        }
    }
    return 0;
}

/* -----------------------------------------------------------------------
 * Column widths
 * ----------------------------------------------------------------------- */
void Iconic_CalcColWidths(COLWIDTHS FAR *pcw)
{
    /* Use the settings from CALMIRA.INI (or sensible defaults) */
    pcw->Name = Profile_GetInt(&g_Ini, "File system", "NameColWidth",  102);
    pcw->Size = Profile_GetInt(&g_Ini, "File system", "SizeColWidth",   66);
    pcw->Type = Profile_GetInt(&g_Ini, "File system", "TypeColWidth",   82);
    pcw->Date = Profile_GetInt(&g_Ini, "File system", "DateColWidth",   85);
    pcw->Attr = Profile_GetInt(&g_Ini, "File system", "AttrColWidth",   52);
    pcw->Desc = g_DescWidth > 0 ? g_DescWidth : 100;
}

/* -----------------------------------------------------------------------
 * Draw one item in the specified view mode
 * ----------------------------------------------------------------------- */
void Iconic_DrawItem(HDC hdc, const ICONITEM FAR *pi,
                     int x, int y, TDisplayMode mode,
                     BOOL bFocused, HINSTANCE hInst)
{
    COLORREF clrBack = pi->bSelected
                       ? GetSysColor(COLOR_HIGHLIGHT)
                       : GetSysColor(COLOR_WINDOW);
    COLORREF clrText = pi->bSelected
                       ? GetSysColor(COLOR_HIGHLIGHTTEXT)
                       : GetSysColor(COLOR_WINDOWTEXT);

    SetBkColor(hdc, clrBack);
    SetTextColor(hdc, clrText);

    switch (mode) {
        case dmLargeIcons: {
            /* 32x32 icon centred in LARGE_ICON_W cell, name below */
            int ix = x + (LARGE_ICON_W - 32) / 2;
            int iy = y + 2;
            if (pi->hIcon) {
                DrawIcon(hdc, ix, iy, pi->hIcon);
            }
            else {
                HICON hDef = LoadIcon(NULL, (pi->bType & ITYPE_DIR)
                                            ? IDI_APPLICATION : IDI_ASTERISK);
                DrawIcon(hdc, ix, iy, hDef);
            }
            {
                RECT rc;
                rc.left = x; rc.top = y + 32 + 4;
                rc.right = x + LARGE_ICON_W; rc.bottom = y + LARGE_ICON_H;
                DrawText(hdc, pi->szName, -1, &rc,
                         DT_CENTER | DT_WORDBREAK | DT_NOPREFIX);
            }
            break;
        }
        case dmSmallIcons: {
            /* 16x16 icon left, name right */
            if (pi->hIcon) {
                DrawIcon(hdc, x, y, pi->hIcon);
            }
            {
                RECT rc;
                rc.left = x + 20; rc.top = y;
                rc.right = x + g_BrowseGrid.x; rc.bottom = y + SMALL_ICON_H;
                DrawText(hdc, pi->szName, -1, &rc,
                         DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
            }
            break;
        }
        case dmDetails: {
            /* Details row: icon, name, size, type, date, attr, desc */
            RECT rc;
            char buf[64];
            int  cx = x;
            if (pi->hIcon) {
                DrawIcon(hdc, cx, y, pi->hIcon);
            }
            cx += 20;
            /* Name */
            rc.left = cx; rc.top = y;
            rc.right = cx + 80; rc.bottom = y + DETAILS_H;
            DrawText(hdc, pi->szName, -1, &rc,
                     DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
            cx += 80;
            /* Size */
            if (!(pi->bType & ITYPE_DIR)) {
                Iconic_FormatSize(pi->dwSize, buf, sizeof(buf));
                rc.left = cx; rc.right = cx + 60;
                DrawText(hdc, buf, -1, &rc,
                         DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
            }
            cx += 60;
            /* Date */
            Iconic_FormatDate(pi->wDate, pi->wTime, buf, sizeof(buf));
            rc.left = cx; rc.right = cx + 80;
            DrawText(hdc, buf, -1, &rc,
                     DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            cx += 80;
            /* Attr */
            Iconic_FormatAttr(pi->bAttr, buf, sizeof(buf));
            rc.left = cx; rc.right = cx + 40;
            DrawText(hdc, buf, -1, &rc,
                     DT_LEFT | DT_VCENTER | DT_SINGLELINE);
            cx += 40;
            /* Description */
            if (g_UseDescriptions && *pi->szDesc) {
                rc.left = cx; rc.right = cx + 100;
                DrawText(hdc, pi->szDesc, -1, &rc,
                         DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
            }
            break;
        }
    }
}
