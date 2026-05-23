/***************************************************************************
 *  DYNAMENU.CPP - Dynamic popup menu helpers (MSVC 1.52c / Win16)
 ***************************************************************************/
#include <windows.h>
#include "DYNAMENU.H"

void DynaMenu_Append(HMENU hmenu, UINT uId, LPCSTR szText)
{
    AppendMenu(hmenu, MF_STRING, uId, szText);
}

void DynaMenu_Sep(HMENU hmenu)
{
    AppendMenu(hmenu, MF_SEPARATOR, 0, NULL);
}

void DynaMenu_AppendSub(HMENU hmenu, HMENU hSub, LPCSTR szText)
{
    AppendMenu(hmenu, MF_POPUP, (UINT)hSub, szText);
}

UINT DynaMenu_Track(HMENU hmenu, HWND hwndOwner, int x, int y)
{
    /* Win16: TrackPopupMenu posts WM_COMMAND to hwndOwner; no TPM_RETURNCMD */
    TrackPopupMenu(hmenu,
                   TPM_LEFTALIGN | TPM_RIGHTBUTTON,
                   x, y, 0, hwndOwner, NULL);
    DestroyMenu(hmenu);
    return 0;
}
