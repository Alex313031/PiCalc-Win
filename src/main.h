#ifndef MAIN_H_
#define MAIN_H_

#pragma once

#include <iostream>
#include <iomanip>

#include "common.h"
#include "framework.h"
#include "resource.h"

// Global Variables:
HINSTANCE hInst;  // current instance

static WCHAR szTitle[MAX_LOADSTRING];  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];   // the main window class name

// Color constants.
const COLORREF rgbRed    =  0x000000FF;
const COLORREF rgbGreen  =  0x0000FF00;
const COLORREF rgbYellow =  0x0000FFFF;
const COLORREF rgbBlue   =  0x00FF0000;
const COLORREF rgbBlack  =  0x00000000;
const COLORREF rgbWhite  =  0x00FFFFFF;

// Function exports
int APIENTRY wWinMain(HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                      LPWSTR lpCmdLine,
                      int nCmdShow);

ATOM MyRegisterClass(HINSTANCE hInstance);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#endif  // MAIN_H_
