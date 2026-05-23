/***************************************************************************
 *  FILEPROP.H - File / Folder Properties dialog (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef FILEPROP_H
#define FILEPROP_H
#include <windows.h>
BOOL FileProp_ShowDialog(HINSTANCE hInst, HWND hwndParent, LPCSTR szPath);
#endif
