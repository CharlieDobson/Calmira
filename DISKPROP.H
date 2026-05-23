/***************************************************************************
 *  DISKPROP.H - Disk Properties dialog       (MSVC 1.52c / Win16)
 ***************************************************************************/
#ifndef DISKPROP_H
#define DISKPROP_H
#include <windows.h>
/* szDriveRoot e.g. "C:\" */
BOOL DiskProp_ShowDialog(HINSTANCE hInst, HWND hwndParent, LPCSTR szDriveRoot);
#endif
