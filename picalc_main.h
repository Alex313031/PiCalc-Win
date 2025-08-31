#pragma once

#ifndef PICALC_MAIN_H_
#define PICALC_MAIN_H_

#include <iostream>

#include "framework.h"
#include "mdi_handler.h"

namespace {
// Dummy file output for conhost
static FILE* fDummyFile;

// Window class registration error is 0, whereas normally 0 means good or TRUE
static WORD kRegClassErr = 0;

// Name of shared library .dll file to load
static const LPCWSTR kPiCalcDll = L"libpicalc.dll";

// Main window title
static const LPCWSTR lpszWindowTitle = L"PiCalc-Win";
};

// Main window class
const WCHAR g_szClassName[] = L"PiCalcWndClass";

/* Function Definitions */
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

ATOM RegisterMainClass(HINSTANCE hInstance);

void printStaticPiForTesting();

void printSharedPiForTesting(HMODULE hDll);

LRESULT CALLBACK MainWndProc(HWND hwnd,
                             UINT msg,
                             WPARAM wParam,
                             LPARAM lParam);

// Define a typedef for the function signature
// of any imported function from libpicalc.dll for safety
typedef char* (*CHAR_PI)();  /* For char_pi_chudnovsky() */
typedef wchar_t* (*WCHAR_PI)();  /* For wchar_pi_chudnovsky() */
typedef std::string* (*STRING_PI)();  /* For string_pi_chudnovsky() */
typedef std::wstring* (*WSTRING_PI)();  /* For wstring_pi_chudnovsky() */

#endif // PICALC_MAIN_H_
