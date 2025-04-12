#ifndef PICALC_H_
#define PICALC_H_

#pragma once

#include "common.h"
#include "chudnovsky.h"

#if defined DCHECK && DCHECK == 1
 static WCHAR kDCheckString[MAX_LOADSTRING];
#else
  static wchar_t kDCheckString[MAX_LOADSTRING];
#endif // defined DCHECK && DCHECK == 1

static wchar_t PiString[MAX_DECIMAL_PLACES];

static LPCWSTR gdi_text;
static LPCWSTR gdi_text2;

extern void PaintHdc(HWND hWnd, HINSTANCE hInst);

#endif  // PICALC_H_
