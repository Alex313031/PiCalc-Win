#pragma once

#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>

#include "common.h"
#include "framework.h"
#include "picalc.h"
#include "resource.h"

static WCHAR szTitle[MAX_LOADSTRING];  // The title bar text
static WCHAR szWindowClass[MAX_LOADSTRING];   // the main window class name

// Function exports
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow);

static WNDCLASSEXW winClassEx;

ATOM RegisterClass(HINSTANCE hInstance);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

static FILE* fDummyFile;

namespace main {
  INT_PTR CALLBACK AboutHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

  HINSTANCE getHinst(HWND hWnd);

  void MakeAboutDialogBox(HWND hWnd);
}

#endif  // MAIN_H_
