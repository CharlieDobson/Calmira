/***************************************************************************
 *  FILTER.H - File filter (wildcard mask) dialog (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef FILTER_H
#define FILTER_H
#include <windows.h>
/* Show filter dialog. Fills buf with chosen mask. Returns TRUE on OK. */
BOOL Filter_Show(HINSTANCE hInst, HWND hwndParent, LPSTR buf, int bufLen);
#endif
