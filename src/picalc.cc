// picalc.cc : Defines the painting logic for the application.

#include <string>

#include "common.h"
#include "main.h"
#include "picalc.h"
#include "resource.h"

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//  FUNCTION: PaintHdc(HWND hWnd, HINSTANCE hInst)
//
//  PURPOSE: Paints the contents of the main window.
//
//  PAINTSTRUCT
void Painter::PaintHdc(HWND hWnd, HINSTANCE hInst) {
  // Structure to store painting info for hbrush handle
  PAINTSTRUCT paintStruct;

  // Begin painting the contents of the window
  // HDC is a device context handle for GDI
  HDC hdc = BeginPaint(hWnd, &paintStruct);
  // Start logging as we are painting
  if (hdc) {
    std::wcout << __FUNC__ << "() BeginPaint() started painting " << std::endl;
  }

  // Run the desired algorithm and store Pi result in to_convert
  float128 pi_result;
  bool flag = true;
  if (flag) {
    pi_result = algorithms::chudnovsky(iterations);
  } else {
    NOTREACHED();
  }
  // Convert result to string of max length kKibi (1024)
  std::wstring pi_userf_string =
      pi_intro + common::ConvertDoubleToWstring(pi_result);
  kPiString = common::ConvertWstringToLPCWSTR(pi_userf_string);

  // Initialize and define the first line
  if (is_debug && is_dcheck) {
    GDI_Text = kDcheckPiString;
  } else {
    GDI_Text = kPiString;
  }

  // We need to grab our window instance handle from main.cc
  HINSTANCE hInstance = hInst;
  // Decide what to do for the second line, depending on build config
  if (is_debug) {
    if (is_dcheck) {
      // Load resource string into WCHAR pointer
      LoadStringW(hInstance, IDS_PICALC_DUMMY_GDI, kDCheckString, MAX_LOADSTRING);
      GDI_Text_D = kDCheckString;
    } else {
      // Debug without DCHECK, load dummy wchar_t string
      GDI_Text_D = kDebugString;
    }
  } else {
    if (is_dcheck) {
      // Same as above
      LoadStringW(hInstance, IDS_PICALC_DUMMY_GDI, kDCheckString, MAX_LOADSTRING);
      GDI_Text_D = kDCheckString;
    } else {
      // Release build - don't show any second line
      GDI_Text_D = nullptr;
    }
  }

  // To store result of GetOSVersion
  std::string os_ver_userf_string = "OS: ";
  os_ver_userf_string.append(common::GetEnvVariable("OS"));
  os_ver_userf_string.append(common::GetOSVersion());
  static LPCWSTR WINNT_VERSION =
      common::ConvertStringToLPCWSTR(os_ver_userf_string);
  GDI_Text_2 = WINNT_VERSION;

  GDI_Text_3 = common::ConvertStringToLPCWSTR("Windir: " + common::GetEnvVariable("WINDIR"));
  std::string cpu_string =
      "CPU: " + common::GetEnvVariable("NUMBER_OF_PROCESSORS") + " Core " + common::GetEnvVariable("PROCESSOR_IDENTIFIER");
  GDI_Text_4 = common::ConvertStringToLPCWSTR(cpu_string);
  GDI_Text_5 = NULL;

  // Declare rect to use for all future window layout
  RECT kWinRect;
  // Get context of entire window
  const HDC winDC = GetWindowDC(hWnd);
  // Get rect size of window including titlebar
  GetClientRect(hWnd, &kWinRect);
  // Set color of text
  SetTextColor(winDC, COLOR_WINDOWTEXT);
  // Set window backgroun painting behavior
  SetBkMode(winDC, TRANSPARENT);

  // Center our text inside the viewport
  kWinRect.left = 24;
  kWinRect.top = 64;
  // Print Pi result
  DrawTextW(winDC,
              GDI_Text,
              -1,
              &kWinRect,
              DT_LEFT | DT_WORD_ELLIPSIS | DT_NOCLIP);
  // Move down 42 dips
  kWinRect.top = 106;
  SetTextColor(winDC, TextStyle::rgbRed);
  DrawTextW(winDC,
              GDI_Text_2,
              -1,
              &kWinRect,
              // Don't respect carriage returns or window size
              DT_CENTER | DT_SINGLELINE | DT_NOCLIP);
  // Move down 42 dips
  kWinRect.top = 148;
  SetTextColor(winDC, TextStyle::rgbRed);
  DrawTextW(winDC,
              GDI_Text_3,
              -1,
              &kWinRect,
              // Don't respect carriage returns or window size
              DT_CENTER | DT_SINGLELINE | DT_NOCLIP);
  // Move down 42 dips
  kWinRect.top = 190;
  SetTextColor(winDC, TextStyle::rgbRed);
  DrawTextW(winDC,
              GDI_Text_4,
              -1,
              &kWinRect,
              // Don't respect carriage returns or window size
              DT_CENTER | DT_SINGLELINE | DT_NOCLIP);
  // Print debug message in center
  SetTextColor(winDC, TextStyle::rgbBlue);
  DrawTextW(winDC,
              GDI_Text_D,
              -1,
              &kWinRect,
              // Don't respect carriage returns or window size
              DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);

  // Delete GetWindowDC() object
  DeleteDC(winDC);
  // End painting to satisfy UpdateWindow()
  if (EndPaint(hWnd, &paintStruct)) {
    // Log EndPaint()
    std::wcout << __FUNC__ << "() EndPaint() stopped painting " << std::endl;
  }
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
    case WM_COMMAND:
      {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId) {
          case IDM_ABOUT:
            main::MakeAboutDialogBox(hWnd);
            break;
          case IDM_HELP:
            common::MakeInfoMessageBox(hWnd, "Help", "No Help so far");
            break;
          case IDM_EXIT:
            if (!DestroyWindow(hWnd)) {
              PostQuitMessage(FAILED);
            }
            break;
          case IDM_MESSAGEBOX:
            common::MakeInfoMessageBox(hWnd, "Info Box", "Everything OK");
            common::MakeWarnMessageBox(hWnd, "Warning Box", "Everything OK?");
            common::MakeErrorMessageBox(hWnd, "Error Box", "Something wrong happened");
            break;
          case IDM_CHUDNOVSKY:
            algorithms::chudnovsky(iterations);
            break;
          default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
      }
      break;
    case WM_PAINT:
      {
        // Get hInstance
        HINSTANCE paintHinst = main::getHinst(hWnd);
        // Actually paint the contents of our window finally
        Painter::PaintHdc(hWnd, paintHinst);
      }
      break;
    case WM_DESTROY:
      PostQuitMessage(SUCCESS);
      break;
    default:
      return DefWindowProcW(hWnd, message, wParam, lParam);
  }
  return 0;
}

void LogCompilerInfo(bool do_log) {
  if (do_log) {
    // Log some compiler preprocessor constants to know what type of build we have
    if (is_debug && !is_dcheck) {
      std::wcout << "Buildtype: DEBUG" << std::endl;
    } else if (!is_debug && !is_dcheck) {
      std::wcout << "Buildtype: RELEASE" << std::endl;
    } else if (!is_debug && is_dcheck) {
      std::wcout << "Buildtype: DCHECK RELEASE" << std::endl;
    } else if (is_debug && is_dcheck) {
      std::wcout << "Buildtype: DCHECK DEBUG" << std::endl;
    } else {
      NOTREACHED();
    }
#ifdef __STDCPP_THREADS__ // Whether threading is enabled
    std::wcout << "__STDCPP_THREADS__ = " << __STDCPP_THREADS__ << std::endl;
#else
    std::wcout << "__STDCPP_THREADS__ = " << FALSE << std::endl;
#endif // __STDCPP_THREADS__
#ifdef __SSE42__ // Whether we compiled with /arch:SSE4.2 (or -msse4.2)
    std::wcout << "__SSE42__ = " << __SSE42__ << std::endl;
#else
    std::wcout << "__SSE42__ = " << FALSE << std::endl;
#endif // __SSE42__
#ifdef __AVX__ // Whether we compiled with /arch:AVX (or -mavx)
    std::wcout << "__AVX__ = " << __AVX__ << std::endl;
#else
    std::wcout << "__AVX__ = " << FALSE << std::endl;
#endif // __AVX__
#ifdef __AVX2__ // Whether we compiled with /arch:AVX2 (or -mavx2)
    std::wcout << "__AVX2__ = " << __AVX2__ << std::endl;
#else
    std::wcout << "__AVX2__ = " << FALSE << std::endl;
#endif // __AVX2__
#ifdef __AVX512BW__ // Whether we compiled with /arch:AVX512 (or -mavx512bw)
    std::wcout << "__AVX512BW__ = " << __AVX512BW__ << std::endl;
#else
    std::wcout << "__AVX512BW__ = " << FALSE << std::endl;
#endif // __AVX512BW__
#ifdef _CHAR_UNSIGNED // Whether char* is considered unsigned
    std::wcout << "_CHAR_UNSIGNED = " << _CHAR_UNSIGNED << std::endl;
#else
    std::wcout << "_CHAR_UNSIGNED = " << FALSE << std::endl;
#endif // _CHAR_UNSIGNED
#ifdef __CLR_VER // The Common Language Runtime used to compile the app
    std::wcout << "__CLR_VER = " << __CLR_VER << std::endl;
#endif // __CLR_VER
    std::wcout << L"\n";
  } else {
    NULLOPT;
  }
}
