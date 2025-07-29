#pragma once

#ifndef PICALC_MAIN_H_
#define PICALC_MAIN_H_

#include <iostream>

#include "framework.h"
#include "resource.h"
#include "mdi_handler.h"

// Main window handle 
static HWND g_hMainWindow = NULL;

// Dummy file output for conhost
static FILE* fDummyFile;

static WORD kRegClassErr = 0;

// Main window class
const TCHAR g_szClassName[] = _T("PiCalcWndClass");

// Main window title
static const LPCTSTR lpszWindowTitle = _T("PiCalc-Win");

/* Function Definitions */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

ATOM RegisterMainClass(HINSTANCE hInstance);

#endif // PICALC_MAIN_H_
