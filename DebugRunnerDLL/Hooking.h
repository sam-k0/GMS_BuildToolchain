#pragma once
#include "MinHook.h"

// Function prototypes
typedef BOOL(WINAPI* setDlgItemTextW)(HWND hdlg, int item, LPCWSTR lpString);
typedef BOOL(WINAPI* peekMessageA)(LPMSG lpMsg, HWND hWnd, UINT wmsgfiltermin, UINT wmsgfiltermax, UINT mremovemsg);
