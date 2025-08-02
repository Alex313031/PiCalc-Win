#pragma once

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "framework.h"

/* Globals */

// Global instance handle
extern HINSTANCE g_hInstance;

// Main window handle, equal to g_hInstance
extern HWND g_hMainWindow;

// Child window handle so that main window can access it
extern HWND g_hMDIClient;

#endif // GLOBALS_H_
