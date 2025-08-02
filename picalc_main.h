#pragma once

#ifndef PICALC_MAIN_H_
#define PICALC_MAIN_H_

#include <iostream>

#include "framework.h"
#include "mdi_handler.h"

// Dummy file output for conhost
static FILE* fDummyFile;

// Window class registration error is 0, whereas normally 0 means good or TRUE
static WORD kRegClassErr = 0;

// Main window class
const TCHAR g_szClassName[] = _T("PiCalcWndClass");

// Main window title
static const LPCTSTR lpszWindowTitle = _T("PiCalc-Win");

/* Function Definitions */

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

ATOM RegisterMainClass(HINSTANCE hInstance);

#endif // PICALC_MAIN_H_
