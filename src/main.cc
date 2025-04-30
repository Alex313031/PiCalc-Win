// main.cc : Defines the entry point for the application.

#include "log.h"
#include "main.h"

// Global Variables:
HINSTANCE hInst;  // current instance

// Forward declarations of functions included in this code module:
ATOM                RegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// Main entry point function, creates the GUI, equivalent to main() or _tmain()
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // Allow and allocate conhost
  AllocConsole();
  // File handler pointer to a dummy file, possibly an actual logfile
  FILE* fNonExistFile = fDummyFile;
  freopen_s(&fNonExistFile, "CONOUT$", "w", stdout);
  freopen_s(&fNonExistFile, "CONOUT$", "w", stderr);

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_PICALC_WIN, szWindowClass, MAX_LOADSTRING);

  // Log welcome message
  base::LOG(INFO, "This is the PiCalc-Win logging console \n");

  // Log compiler #defines right before we register the Window Class
  common::LogCompilerInfo(true);

  // Register the Window
  if (!RegisterClass(hInstance)) {
    common::MakeErrorMessageBox(NULL, "Error", "Window Class Registration Failed!");
    return false;
  }

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow)) {
    std::wcerr << "InitInstance() failed!" << ENDL;
    return false;
  }

  // Load  keyboard shortcuts
  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PICALC_WIN));
  MSG msg;

  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  return (int) msg.wParam;
}

//  FUNCTION: RegisterClass(HINSTANCE)
//
//  PURPOSE: Registers the window class (class here does not mean C++ class).
ATOM RegisterClass(HINSTANCE hInstance) {

  winClassEx.cbSize = sizeof(WNDCLASSEX);

  winClassEx.style          = CS_HREDRAW | CS_VREDRAW;
  winClassEx.lpfnWndProc    = WndProc;
  winClassEx.cbClsExtra     = 0;
  winClassEx.cbWndExtra     = 0;
  winClassEx.hInstance      = hInstance;
  winClassEx.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PICALC_WIN));
  winClassEx.hCursor        = LoadCursor(nullptr, IDC_ARROW);
  winClassEx.hbrBackground  = (HBRUSH)(nullptr);
  winClassEx.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
  winClassEx.lpszMenuName   = MAKEINTRESOURCEW(IDC_PICALC_WIN);
  winClassEx.lpszClassName  = szWindowClass;
  winClassEx.hIconSm        = LoadIcon(winClassEx.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&winClassEx);
}

//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//     In this function, we save the instance handle in a global variable and
//     create and display the main program window.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
 // hInst = hInstance; // Store instance handle in our global variable

  DWORD dwExStyle;
  if (is_debug) {
    dwExStyle = WS_EX_RIGHTSCROLLBAR;
  } else {
    dwExStyle = WS_EX_RIGHTSCROLLBAR;
  }

  // Create our window, really just a handle on win32
  HWND hWnd = CreateWindowExW(dwExStyle,
                              szWindowClass,
                              szTitle,
                              WinStyle::kTiled,
                              kUseDefault,
                              kUseDefault,
                              640,
                              480,
                              nullptr,
                              nullptr,
                              hInstance,
                              nullptr);

  if (!hWnd || hWnd == NULL) {
    common::MakeErrorMessageBox(hWnd, "Error", "Window Creation Failed!");
    common::ExecuteNoNo(true);
    return false;
  } else {
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return true;
  }
}

// Message handler for about box.
INT_PTR CALLBACK main::AboutHandler(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);

  switch (message) {
    case WM_INITDIALOG:
      return (INT_PTR)TRUE;
    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
      }
      break;
    default:
      return (INT_PTR)FALSE;
  }
  return (INT_PTR)FALSE;
}

// Get hInstance from hWnd Window Handle
HINSTANCE main::getHinst(HWND hWnd) {
  if (hWnd) {
    return hInst;
  } else {
    if (is_debug) {
      bool trap = is_dcheck || test_trap;
      common::ExecuteNoNo(trap);
    }
    return nullptr;
  }
}

void main::MakeAboutDialogBox(HWND hWnd) {
  DialogBoxW(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, main::AboutHandler);
}
