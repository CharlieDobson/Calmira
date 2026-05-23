/***************************************************************************
 *  REPLACE.CPP - File replace confirmation   (MSVC 1.52c / Win16)
 ***************************************************************************/
#include <windows.h>
#include "REPLACE.H"
#include "LOCALE.H"
#include "SETTINGS.H"

int Replace_Ask(HINSTANCE hInst, HWND hwndParent, LPCSTR szExisting, LPCSTR szNew)
{
    char msg[320], fmt[256];
    LoadString(hInst, SQueryReplaceFile, fmt, sizeof(fmt));
    wsprintf(msg, fmt, szExisting);
    switch (MessageBox(hwndParent, msg, "Calmira",
                       MB_ICONQUESTION | MB_YESNOCANCEL)) {
        case IDYES:    return IDYES;
        case IDNO:     return IDNO;
        default:       return IDCANCEL;
    }
}
