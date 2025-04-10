// picalc.cpp : Defines the entry point for the application.

#include "chudnovsky.h"
#include "picalc.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;  // current instance

static WCHAR szTitle[MAX_LOADSTRING];  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];   // the main window class name

#if defined DCHECK && DCHECK == 1
 WCHAR PiString[MAX_LOADSTRING];        // Text to display in the Window
#else
  //wchar_t PiString[] = L"3.1415926";
  wchar_t PiString[1024];
#endif

// The string that appears in the application's title bar.
//static WCHAR szTitle[] = L"Windows Desktop Guided Tour Application";

// The main window class name.
//static const WCHAR szWindowClass[] = L"PiCalcWindow";

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Place code here.

  // Initialize global strings
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_PICALC, szWindowClass, MAX_LOADSTRING);

#if defined DCHECK && DCHECK == 1
  LoadStringW(hInstance, IDS_PICALC_DUMMY_GDI, PiString, MAX_LOADSTRING);
#else
  std::wcout << PiString;
#endif

  MyRegisterClass(hInstance);

  // Perform application initialization:
  if (!InitInstance (hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PICALC));
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

//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc    = WndProc;
  wcex.cbClsExtra     = 0;
  wcex.cbWndExtra     = 0;
  wcex.hInstance      = hInstance;
  wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PICALC));
  wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PICALC);
  wcex.lpszClassName  = szWindowClass;
  wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//     In this function, we save the instance handle in a global variable and
//     create and display the main program window.
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass,
                             szTitle,
                             WS_TILEDWINDOW,
                             CW_USEDEFAULT,
                             CW_USEDEFAULT,
                             640,
                             480,
                             nullptr,
                             nullptr,
                             hInstance,
                             nullptr);

   if (!hWnd) {
     return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
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
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
          case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
          default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
      }
      break;
    case WM_PAINT:
      {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        long double to_convert = chudnovsky(iterations);
        swprintf(PiString, 1024, L"%f", to_convert);
        LPCWSTR gdi_text = PiString;
        RECT rect;
        HDC wdc = GetWindowDC(hWnd);
        GetClientRect(hWnd, &rect);
        SetTextColor(wdc, COLOR_WINDOWTEXT);
        SetBkMode(wdc, TRANSPARENT);
        rect.left = 24;
        rect.top = 64;
        DrawTextW(wdc, gdi_text, -1, &rect, DT_SINGLELINE | DT_NOCLIP);
        DeleteDC(wdc);
        EndPaint(hWnd, &ps);
      }
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
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
