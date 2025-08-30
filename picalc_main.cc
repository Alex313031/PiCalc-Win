#include "picalc_main.h"

#include "libpicalc/gmp/gmp.h"
#include "libpicalc/gmp/gmpxx.h"
#include "libpicalc/libpicalc_dll.h"

#include "common.h"
#include "dialogs.h"
#include "globals.h"
#include "resource.h"

/* Global instance handle */
HINSTANCE g_hInstance = NULL;

HWND g_hMainWindow = NULL;

BOOL SaveTextFileFromEdit(HWND hEdit, LPCTSTR pszFileName) {
  HANDLE hFile;
  BOOLEAN bSuccess = FALSE;

  hFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                     FILE_ATTRIBUTE_NORMAL, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    DWORD dwTextLength;

    dwTextLength = GetWindowTextLength(hEdit);
    // No need to bother if there's no text.
    if (dwTextLength > 0) {
      LPTSTR pszText;
      DWORD dwBufferSize = dwTextLength + 1;

      pszText = (LPTSTR)GlobalAlloc(GPTR, dwBufferSize);
      if (pszText != NULL) {
        if (GetWindowText(hEdit, pszText, dwBufferSize)) {
          DWORD dwWritten;

          if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL)) {
            bSuccess = TRUE;
          }
        }
        GlobalFree(pszText);
      }
    }
    CloseHandle(hFile);
  }
  return bSuccess;
}

void DoFileSave(HWND hwnd) {
  OPENFILENAME ofn;
  WCHAR szFileName[MAX_PATH] = L"";

  ZeroMemory(&ofn, sizeof(ofn));

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
  ofn.lpstrFile = szFileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.lpstrDefExt = _T("txt");
  ofn.Flags =
      OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

  if (GetSaveFileName(&ofn)) {
    HWND hEdit = GetDlgItem(hwnd, IDC_CHILD_EDIT);
    if (SaveTextFileFromEdit(hEdit, szFileName)) {
      SendDlgItemMessage(g_hMainWindow, IDC_MAIN_STATUS, SB_SETTEXT, 0,
                         (LPARAM)_T("Saved..."));
      SendDlgItemMessage(g_hMainWindow, IDC_MAIN_STATUS, SB_SETTEXT, 1,
                         (LPARAM)szFileName);

      SetWindowText(hwnd, szFileName);
    }
  }
}

LRESULT CALLBACK MDIChildWndProcOld(HWND hwnd,
                                 UINT msg,
                                 WPARAM wParam,
                                 LPARAM lParam) {
  switch (msg) {
    case WM_CREATE: {
      HFONT hfDefault;
      HWND hEdit = NULL;

      // Create Edit Control
      hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""),
                             WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                                 ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                             0, 0, 100, 100, hwnd, (HMENU)IDC_CHILD_EDIT,
                             GetModuleHandle(NULL), NULL);
      if (!hEdit || hEdit == NULL) {
        MessageBox(hwnd, _T("Could not create edit box."), _T("Error"),
                   MB_OK | MB_ICONERROR);
      }

      hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
    } break;
    case WM_MDIACTIVATE: {
      HMENU hMenu, hFileMenu;
      UINT EnableFlag;

      hMenu = GetMenu(g_hMainWindow);
      if (hwnd == (HWND)lParam) {  // being activated, enable the menus
        EnableFlag = MF_ENABLED;
      } else {  // being de-activated, gray the menus
        EnableFlag = MF_GRAYED;
      }

      EnableMenuItem(hMenu, 1, MF_BYPOSITION | EnableFlag);
      EnableMenuItem(hMenu, 2, MF_BYPOSITION | EnableFlag);

      hFileMenu = GetSubMenu(hMenu, 0);
      EnableMenuItem(hFileMenu, ID_FILE_SAVEAS, MF_BYCOMMAND | EnableFlag);

      EnableMenuItem(hFileMenu, ID_FILE_CLOSE, MF_BYCOMMAND | EnableFlag);
      EnableMenuItem(hFileMenu, ID_FILE_CLOSEALL, MF_BYCOMMAND | EnableFlag);

      DrawMenuBar(g_hMainWindow);
    } break;
    case WM_COMMAND:
      switch (LOWORD(wParam)) {
        case ID_FILE_OPEN:
          DoFileOpen(hwnd);
          break;
        case ID_FILE_SAVEAS:
          DoFileSave(hwnd);
          break;
        case ID_EDIT_CUT:
          SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_CUT, 0, 0);
          break;
        case ID_EDIT_COPY:
          SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_COPY, 0, 0);
          break;
        case ID_EDIT_PASTE:
          SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_PASTE, 0, 0);
          break;
      }
      break;
    case WM_SIZE: {
      HWND hEdit;
      RECT rcClient;

      // Calculate remaining height and size edit
      GetClientRect(hwnd, &rcClient);

      hEdit = GetDlgItem(hwnd, IDC_CHILD_EDIT);
      SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom,
                   SWP_NOZORDER);
    }
    default:
      return DefMDIChildProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

BOOL SetUpMDIChildWindowClass(HINSTANCE hInstance) {
  WNDCLASSEX mdiwc;

  // Window class details
  mdiwc.cbSize = sizeof(WNDCLASSEX);
  mdiwc.style = CS_HREDRAW | CS_VREDRAW;
  mdiwc.lpfnWndProc = MDIChildWndProc;
  mdiwc.cbClsExtra = 0;
  mdiwc.cbWndExtra = 0;
  mdiwc.hInstance = hInstance;
  mdiwc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PICALC));
  mdiwc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  mdiwc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  mdiwc.lpszMenuName = NULL;
  mdiwc.lpszClassName = g_szChildClassName;
  mdiwc.hIconSm = LoadIcon(mdiwc.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  if (!RegisterClassEx(&mdiwc)) {
    MessageBox(0, _T("Could not register MDIChild Window"), _T("Uh Oh..."),
               MB_ICONEXCLAMATION | MB_OK);
    return FALSE;
  } else {
    return TRUE;
  }
}

int handleReturnCode(int return_code) {
  std::wcout << "Return code is: " << return_code << std::endl;
  system("pause");
  return return_code;
}

//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
ATOM RegisterMainClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style          = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc    = MainWndProc;
  wcex.cbClsExtra     = 0;
  wcex.cbWndExtra     = 0;
  wcex.hInstance      = hInstance;
  wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PICALC));
  wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_MAINMENU);
  wcex.lpszClassName  = g_szClassName;
  wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

  ATOM class_registered = RegisterClassEx(&wcex);

  if (!class_registered || class_registered == kRegClassErr) {
    return kRegClassErr;
  } else {
    return class_registered;
  }
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
  switch (msg) {
    case WM_CREATE: {
      HWND hTool = NULL;
      HWND hChildWin = NULL;
      TBBUTTON tbb[3];
      TBADDBITMAP tbab;

      HWND hStatus;
      int statwidths[] = {100, -1};

      CLIENTCREATESTRUCT ccs;

      // Find window menu where children will be listed
      ccs.hWindowMenu = GetSubMenu(GetMenu(hwnd), 2);
      ccs.idFirstChild = ID_MDI_FIRSTCHILD;

      // Create MDI Client
      hChildWin = CreateWindowEx(
          WS_EX_CLIENTEDGE, _T("mdiclient"), NULL,
          WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE,
          CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd,
          (HMENU)IDC_MAIN_MDI, GetModuleHandle(NULL), (LPVOID)&ccs);

      if (!hChildWin || hChildWin == NULL) {
        MessageBox(hwnd, _T("Could not create MDI client."), _T("Error"),
                   MB_OK | MB_ICONERROR);
      } else {
        g_hMDIClient = hChildWin;
      }

      // Create Toolbar
      hTool = CreateWindowEx(0, TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE,
                             0, 0, 0, 0, hwnd, (HMENU)IDC_MAIN_TOOL,
                             GetModuleHandle(NULL), NULL);
      if (!hTool || hTool == NULL) {
        MessageBox(hwnd, _T("Could not create tool bar."), _T("Error"),
                   MB_OK | MB_ICONERROR);
      }

      // Send the TB_BUTTONSTRUCTSIZE message, which is required for
      // backward compatibility.
      SendMessage(hTool, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);

      tbab.hInst = HINST_COMMCTRL;
      tbab.nID = IDB_STD_SMALL_COLOR;
      SendMessage(hTool, TB_ADDBITMAP, 0, (LPARAM)&tbab);

      ZeroMemory(tbb, sizeof(tbb));
      tbb[0].iBitmap = STD_FILENEW;
      tbb[0].fsState = TBSTATE_ENABLED;
      tbb[0].fsStyle = TBSTYLE_BUTTON;
      tbb[0].idCommand = ID_FILE_NEW;

      tbb[1].iBitmap = STD_FILEOPEN;
      tbb[1].fsState = TBSTATE_ENABLED;
      tbb[1].fsStyle = TBSTYLE_BUTTON;
      tbb[1].idCommand = ID_FILE_OPEN;

      tbb[2].iBitmap = STD_FILESAVE;
      tbb[2].fsState = TBSTATE_ENABLED;
      tbb[2].fsStyle = TBSTYLE_BUTTON;
      tbb[2].idCommand = ID_FILE_SAVEAS;

      SendMessage(hTool, TB_ADDBUTTONS, sizeof(tbb) / sizeof(TBBUTTON),
                  (LPARAM)&tbb);

      // Create Status bar
      hStatus = CreateWindowEx(
          0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0,
          0, 0, 0, hwnd, (HMENU)IDC_MAIN_STATUS, GetModuleHandle(NULL), NULL);

      SendMessage(hStatus, SB_SETPARTS, sizeof(statwidths) / sizeof(int),
                  (LPARAM)statwidths);
      SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)_T("Status"));
    } break;
    case WM_SIZE: {
      HWND hTool;
      RECT rcTool;
      LONG iToolHeight;

      HWND hStatus;
      RECT rcStatus;
      LONG iStatusHeight;

      HWND hMDI;
      RECT rcClient;
      LONG iMDIHeight;

      // Size toolbar and get height
      hTool = GetDlgItem(hwnd, IDC_MAIN_TOOL);
      SendMessage(hTool, TB_AUTOSIZE, 0, 0);

      GetWindowRect(hTool, &rcTool);
      iToolHeight = rcTool.bottom - rcTool.top;

      // Size status bar and get height
      hStatus = GetDlgItem(hwnd, IDC_MAIN_STATUS);
      SendMessage(hStatus, WM_SIZE, 0, 0);

      GetWindowRect(hStatus, &rcStatus);
      iStatusHeight = rcStatus.bottom - rcStatus.top;

      // Calculate remaining height and size edit
      GetClientRect(hwnd, &rcClient);

      iMDIHeight = rcClient.bottom - iToolHeight - iStatusHeight;

      // Find and set window metrics and position
      hMDI = GetDlgItem(hwnd, IDC_MAIN_MDI);
      SetWindowPos(hMDI, NULL, 0, iToolHeight, rcClient.right, iMDIHeight,
                   SWP_NOZORDER);
    } break;
    // When close signal is recieved i.e. from close button
    case WM_CLOSE: {
      DestroyWindow(hwnd);
    } break;
    // Destroy handler
    case WM_DESTROY: {
      PostQuitMessage(SUCC);
    } break;
    // For if OS is shutting down, Windows broadcasts to all hwnd on the desktop
    // to let them know the workstation is going bye bye.
    case WM_GETMINMAXINFO: {
      lpMMI->ptMinTrackSize.x = 300;
      lpMMI->ptMinTrackSize.y = 200;
    } break;
    case WM_COMMAND: {
      std::wcout <<  "WM_COMMAND" << std::endl;
      switch (LOWORD(wParam)) {
        case IDM_EXIT:
          PostMessage(hwnd, WM_CLOSE, 0, 0);
          break;
        case ID_FILE_NEW:
          CreateNewMDIChild(g_hMDIClient);
          break;
        case ID_FILE_OPEN: {
          HWND hChild = CreateNewMDIChild(g_hMDIClient);
          if (hChild) {
            DoFileOpen(hChild);
          }
        } break;
        case ID_FILE_CLOSE: {
          HWND hChild = (HWND)SendMessage(g_hMDIClient, WM_MDIGETACTIVE, 0, 0);
          if (hChild) {
            SendMessage(hChild, WM_CLOSE, 0, 0);
          }
        } break;
        case ID_WINDOW_TILE:
          SendMessage(g_hMDIClient, WM_MDITILE, 0, 0);
          break;
        case ID_WINDOW_CASCADE:
          SendMessage(g_hMDIClient, WM_MDICASCADE, 0, 0);
          break;
        case IDM_HELP:
          break;
        case IDM_ABOUT:
          ShowAboutDialog(hwnd);
          break;
        default: {
          if (LOWORD(wParam) >= ID_MDI_FIRSTCHILD) {
            DefFrameProc(hwnd, g_hMDIClient, WM_COMMAND, wParam, lParam);
          } else {
            HWND hChild =
                (HWND)SendMessage(g_hMDIClient, WM_MDIGETACTIVE, 0, 0);
            if (hChild) {
              SendMessage(hChild, WM_COMMAND, wParam, lParam);
            }
          }
        }
      }
    } break;
    default:
      return DefFrameProc(hwnd, g_hMDIClient, msg, wParam, lParam);
  }
  return 0;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  HWND hwnd = NULL;

  // Create main window
  hwnd = CreateWindowEx(0, g_szClassName, lpszWindowTitle,
                        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT,
                        CW_USEDEFAULT, 480, 320, NULL, NULL, hInstance, NULL);

   if (!hwnd || hwnd == NULL) {
     return false;
   } else {
     g_hMainWindow = hwnd;
     ShowWindow(hwnd, nCmdShow);
     UpdateWindow(hwnd);
   }

   return true;
}

void printPiForTesting() {
  bool oss_success = false;
  bool woss_success = false;
  std::ostringstream osspi;
  std::wostringstream wosspi;
  if (oss_pi_chudnovsky(osspi)) {
    oss_success = true;
  }
  if (woss_pi_chudnovsky(wosspi)){
    woss_success = true;
  }
  std::cout << std::endl;
  std::cout << "oss_pi_chudnovsky = " << std::fixed << std::setprecision(1000) << osspi.str()
                                      << "\n" << std::endl;
  std::wcout << L"woss_pi_chudnovsky = " << wosspi.str() << L"\n" << std::endl;

  std::wstring* wstring_pi = wstring_pi_chudnovsky();
  std::wcout << L"wstring_pi_chudnovsky = " << wstring_pi->c_str() << L"\n" << std::endl;
  wstring_pi = nullptr;

  std::string* string_pi = string_pi_chudnovsky();
  std::cout << "string_pi_chudnovsky = " << string_pi->c_str() << "\n" << std::endl;
  string_pi = nullptr;

  char* char_pi = char_pi_chudnovsky();
  std::cout << "char_pi_chudnovsky = " << char_pi_chudnovsky() << "\n" << std::endl;
  delete[] char_pi;
  char_pi = nullptr;

  wchar_t* wchar_pi = wchar_pi_chudnovsky();
  std::wcout << L"wchar_pi_chudnovsky = " << wchar_pi_chudnovsky() << L"\n" << std::endl;
  delete[] wchar_pi;
  wchar_pi = nullptr;

  static const bool success =
      oss_success && woss_success;
  if (!success) {
    std::wcout << __func__ << L" Failed at one or more steps" << std::endl;
  }
}

int WINAPI wWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine,
                     int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  /* Assign global HINSTANCE */
  g_hInstance = hInstance;
  MSG Msg;

  // Import ComCtl32.dll
  InitCommonControls();

  // Set locale
  _wsetlocale(LC_ALL, L"en-US");

  // Allow and allocate conhost
  if (!AllocConsole()) {
    return handleReturnCode(FAIL);
  }
  // File handler pointer to a dummy file, possibly an actual logfile
  FILE* fNonExistFile = fDummyFile;
  freopen_s(&fNonExistFile, "CONOUT$", "w", stdout);
  freopen_s(&fNonExistFile, "CONOUT$", "w", stderr);

  // Register the window class
  if (RegisterMainClass(g_hInstance) == kRegClassErr) {
    MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"),
               MB_ICONEXCLAMATION | MB_OK);
    return handleReturnCode(FAIL);
  }

  // Perform application initialization:
  if (!InitInstance(g_hInstance, nCmdShow)) {
    MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"),
               MB_ICONEXCLAMATION | MB_OK);
    return handleReturnCode(FAIL);
  } else {
    std::wcout << "Welcome to PiCalc-Win v." << getVersionString() << std::endl;
    HWND printHwnd = getMainHwnd();
    std::wcout << "getMainHwnd() reported " << printHwnd << std::endl;
  }

  // And the child window class
  if (!SetUpMDIChildWindowClass(g_hInstance)) {
    return handleReturnCode(FAIL);
  }

  // Load  keyboard shortcuts
  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAINMENU));

#ifndef COMPONENT_BUILD
  std::wcout << L"static libpicalc call" << std::endl;;
#else
  std::wcout << L"shared DLL libpicalc call" << std::endl;;
  HMODULE hPiDll;
  hPiDll = LoadLibraryW(kPiCalcDll);
  if (!hPiDll || hPiDll == NULL) {
    MessageBoxW(NULL, L"Failed to load libpicalc.dll", L"Error loading DLL",
               MB_ICONERROR | MB_OK);
    handleReturnCode(FAIL);
  } else {
    std::wcout << L"Successfully loaded " << kPiCalcDll << std::endl;
  }
#endif // COMPONENT_BUILD

  printPiForTesting();

  while (GetMessage(&Msg, nullptr, 0, 0) > 0) {
    if (!TranslateAccelerator(g_hMainWindow, hAccelTable, &Msg)) {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    }
  }

  const UINT_PTR result = Msg.wParam;
  const unsigned int uiResult = result;
  const int kMessageResult = static_cast<int>(uiResult);

  if (!FreeLibrary(hPiDll)) {
    MessageBoxW(NULL, L"Failed to free libpicalc.dll", L"Error freeing DLL",
               MB_ICONERROR | MB_OK);
    handleReturnCode(FAIL);
  } else {
    std::wcout << L"Successfully freed library " << kPiCalcDll << std::endl;
  }

  if (kMessageResult == SUCC) {
    return SUCC;
  }
  return handleReturnCode(kMessageResult);
}


HWND getMainHwnd() {
  if (g_hMainWindow != NULL) {
    return g_hMainWindow;
  } else {
    std::wcout << "g_hMainWindow was NULL!" << std::endl;
    return nullptr;
  }
}
