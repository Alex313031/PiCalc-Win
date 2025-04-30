#pragma once

#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#ifdef __cpp_lib_string_view
 // std::string_view is supported
 #include <string_view>
#endif

#define NOTREACHED() \
        std::string func_name(__FUNC__); \
        NotReachedImpl(func_name);

#include "constants.h"
#include "framework.h"

// Maximum .res loadstring length
#ifndef MAX_LOADSTRING
 #define MAX_LOADSTRING MAX_PATH // Traditionally 255
#endif // MAX_LOADSTRING

constexpr int SUCCESS = SUCCESS_CODE;
constexpr int FAILED = ERROR_CODE;

constexpr float128 zero = 0.000;

// Bool to be used externally to test if DCHECK is on
constexpr bool is_dcheck = DCHECK && DCHECK == 1;

// For testing the different trap functions
const bool test_trap = false;

// Same as above but for DEBUG/NDEBUG
#if defined DEBUG || defined _DEBUG
 constexpr bool is_debug = true;
#else
 constexpr bool is_debug = false;
#endif // defined DEBUG || defined DEBUG

// Alias for default window positioning
constexpr int kUseDefault = CW_USEDEFAULT;

// Functions every module should be able to access
namespace common {
  // 32 bit long pointer for 2^10
  constexpr LONG32 kKibi = MAX_DECIMAL_PLACES;

  // Convert std::string to std::wstring
  std::wstring StringToWstring(std::string in_string);

  // Convert std::wstring to std::string
  std::string WStringToString(std::wstring in_wstring);

  // Convert a std::string to LPCWSTR
  LPCWSTR ConvertStringToLPCWSTR(std::string in_string);

  // Convert a std::wstring to LPCWSTR
  LPCWSTR ConvertWstringToLPCWSTR(std::wstring in_wstring);

  // Convert long double to wide character string
  std::wstring ConvertDoubleToWstring(float128 to_convert);

  std::string GetEnvVariable(const char* env_var_name);

  // Message box types
  int MakeInfoMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents);

  int MakeWarnMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents);

  int MakeErrorMessageBox(HWND hWnd, std::string msbox_title, std::string msbox_contents);

  // Kill or set debug breakpoint
  void ExecuteNoNo(bool trap);
}

// WINDOWEX styles aliases, not an exhaustive list
namespace WinStyle {
  constexpr DWORD kTiled = WS_TILEDWINDOW;
  constexpr DWORD kBordered = WS_BORDER;
  constexpr DWORD kChild = WS_CHILDWINDOW;
  constexpr DWORD kDialog = WS_DLGFRAME;
  constexpr DWORD kThickFrame = WS_THICKFRAME;
  constexpr DWORD kSizeAble = WS_SIZEBOX;
  constexpr DWORD kSysMenu = WS_SYSMENU;
  constexpr DWORD kVScroll = WS_VSCROLL;
}

namespace TextStyle {
  // Color constants.
  constexpr COLORREF rgbRed    = 0x000000FF;
  constexpr COLORREF rgbGreen  = 0x0000FF00;
  constexpr COLORREF rgbYellow = 0x0000FFFF;
  constexpr COLORREF rgbBlue   = 0x00FF0000;
  constexpr COLORREF rgbBlack  = 0x00000000;
  constexpr COLORREF rgbWhite  = 0x00FFFFFF;
}

// Execute an assembly instruction to halt execution
inline void AsmIllegalInstr();

void NotReachedImpl(std::string func_name);

#endif // COMMON_H_
