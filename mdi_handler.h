#pragma once

#ifndef MDI_HANDLER_H_
#define MDI_HANDLER_H_

#include <string>

#include "common.h"
#include "framework.h"

// Child window class
const TCHAR g_szChildClassName[] = _T("PiCalcMDIWndClass");

// Default child window title
const LPCTSTR szEmptyFileName = _T("[Untitled]");

/* Function Definitions */
BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName);

void DoFileOpen(HWND hwnd);

HWND CreateNewMDIChild(HWND hMDIClient);

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // MDI_HANDLER_H_
