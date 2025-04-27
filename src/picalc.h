#ifndef PICALC_H_
#define PICALC_H_

#pragma once

#include "common.h"
#include "chudnovsky.h"

// String for testing DCHECK
#if defined DCHECK && DCHECK == 1
 static WCHAR kDCheckString[MAX_LOADSTRING];
#else
 static WCHAR kDCheckString[MAX_LOADSTRING];
#endif // defined DCHECK && DCHECK == 1

// To store our result in string format
static LPCWSTR kPiString;

// First line of text, our result
static LPCWSTR GDI_Text;
// Second line, debug message, if any
static LPCWSTR GDI_Text_D;
// Windows version message
static LPCWSTR GDI_Text_2;
// Other OS info
static LPCWSTR GDI_Text_3;
static LPCWSTR GDI_Text_4;
static LPCWSTR GDI_Text_5;

constexpr wchar_t kDcheckPiString[] =
    L"_DCHECK_3.141592653589793560087173318606801331043243408203125_";
// Dummy text for testing viewport client rect text alignment
constexpr wchar_t kDebugString[] = L"DebugPiString[] Pi WCHAR Test";

const std::wstring pi_intro = L"Pi, calculated to long double precision is: \n";

// Functions specific to painting
namespace Painter {
  void PaintHdc(HWND hWnd, HINSTANCE hInst);
}

// Included in this file but called by main.cc
extern LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void LogCompilerInfo(bool do_log);

#endif  // PICALC_H_
