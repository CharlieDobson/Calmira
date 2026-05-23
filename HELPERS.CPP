/***************************************************************************
 *
 *  HELPERS.CPP - Win16 / MSVC 1.52c compatibility helper implementations
 *  Target: MSVC 1.52c / Windows 3.1 16-bit
 *
 *  Functions that require a single shared instance (e.g. those with
 *  static caches) live here rather than in HELPERS.H, so that each
 *  translation unit does not get its own private copy.
 *
 ***************************************************************************/

#include <windows.h>
#include "HELPERS.H"

/* ------------------------------------------------------------------
 * GetSysColorBrush - single shared cache across all translation units.
 * Brushes are never deleted (matching Win32 contract); Windows reclaims
 * them when the process exits.
 * ------------------------------------------------------------------ */
HBRUSH GetSysColorBrush(int nIndex)
{
    static HBRUSH s_cache[32];   /* one shared array for the whole EXE */
    if ((unsigned)nIndex < 32) {
        if (!s_cache[nIndex]) {
            s_cache[nIndex] = CreateSolidBrush(GetSysColor(nIndex));
        }
        return s_cache[nIndex];
    }
    /* Index out of the cached range: create and return without caching.
       Callers that pass wild indices must delete the brush themselves. */
    return CreateSolidBrush(GetSysColor(nIndex));
}
