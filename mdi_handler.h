#pragma once

#ifndef MDI_HANDLER_H_
#define MDI_HANDLER_H_

#include <string>

#include "common.h"
#include "framework.h"

// Child window class
const WCHAR g_szChildClassName[] = L"PiCalcMDIWndClass";

// Default child window title
const LPCWSTR szEmptyFileName = L"[Untitled]";

/* Function Definitions */
BOOL LoadTextFileToEdit(HWND hEdit, LPCWSTR pszFileName);

void DoFileOpen(HWND hwnd);

HWND CreateNewMDIChild(HWND hMDIClient);

LRESULT CALLBACK MDIChildWndProc(HWND hWnd,
                                 UINT msg,
                                 WPARAM wParam,
                                 LPARAM lParam);

#endif // MDI_HANDLER_H_
