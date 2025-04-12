// picalc.cpp : Defines the painting logic for the application.

#include <string>

#include "picalc.h"
#include "resource.h"

void PaintHdc(HWND hWnd, HINSTANCE hInst) {
  PAINTSTRUCT ps;
  HDC hdc = BeginPaint(hWnd, &ps);

  long double to_convert = chudnovsky(iterations);
  swprintf(PiString, kKibi, L"%f", to_convert);

  gdi_text = PiString;
  HINSTANCE hInstance = hInst;

  if (is_debug) {
    if (is_dcheck) {
      LoadStringW(hInstance, IDS_PICALC_DUMMY_GDI, kDCheckString, MAX_LOADSTRING);
      gdi_text2 = kDCheckString;
    } else {
      gdi_text2 = kDebugPiString;
    }
  } else {
    if (is_dcheck) {
      LoadStringW(hInstance, IDS_PICALC_DUMMY_GDI, kDCheckString, MAX_LOADSTRING);
      gdi_text2 = kDCheckString;
    } else {
      gdi_text2 = nullptr;
    }
  }

  RECT rect;
  HDC wdc = GetWindowDC(hWnd);
  GetClientRect(hWnd, &rect);
  SetTextColor(wdc, COLOR_WINDOWTEXT);
  SetBkMode(wdc, TRANSPARENT);
  rect.left = 24;
  rect.top = 64;
  DrawTextW(wdc, gdi_text, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
  rect.top = 88;
  DrawTextW(wdc, gdi_text2, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
  DeleteDC(wdc);
  EndPaint(hWnd, &ps);
}
