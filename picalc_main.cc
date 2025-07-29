#include "picalc_main.h"

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
  TCHAR szFileName[MAX_PATH] = _T("");

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

LRESULT CALLBACK MDIChildWndProc(HWND hwnd,
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
  mdiwc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  mdiwc.hCursor = LoadCursor(NULL, IDC_ARROW);
  mdiwc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
  mdiwc.lpszMenuName = NULL;
  mdiwc.lpszClassName = g_szChildClassName;
  mdiwc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

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
  wcex.lpszMenuName   = MAKEINTRESOURCE(IDR_MAINMENU);
  wcex.lpszClassName  = g_szClassName;
  wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

  ATOM class_registered = RegisterClassEx(&wcex);

  if (!class_registered || class_registered == kRegClassErr) {
    return kRegClassErr;
  } else {
    return class_registered;
  }
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  HWND hwnd = NULL;

  hwnd = CreateWindowEx(0, g_szClassName, lpszWindowTitle,
                        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, CW_USEDEFAULT,
                        CW_USEDEFAULT, 480, 320, NULL, NULL, hInstance, NULL);

   if (!hwnd || hwnd == NULL) {
     return false;
   } else {
     ShowWindow(hwnd, nCmdShow);
     UpdateWindow(hwnd);
   }

   return true;
}

int WINAPI _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR lpCmdLine,
                     int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  MSG Msg;

  InitCommonControls();

  // Allow and allocate conhost
  if (!AllocConsole()) {
    constexpr int return_code = 1;
    return handleReturnCode(return_code);
  }
  // File handler pointer to a dummy file, possibly an actual logfile
  FILE* fNonExistFile = fDummyFile;
  freopen_s(&fNonExistFile, "CONOUT$", "w", stdout);
  freopen_s(&fNonExistFile, "CONOUT$", "w", stderr);

  // Register the window class
  if (RegisterMainClass(hInstance) == kRegClassErr) {
    MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"),
               MB_ICONEXCLAMATION | MB_OK);
    constexpr int return_code = STATUS_BAD;
    return handleReturnCode(return_code);
  }

  // And the child window class
  if (!SetUpMDIChildWindowClass(hInstance)) {
    constexpr int return_code = STATUS_BAD;
    return handleReturnCode(return_code);
  }

  // Perform application initialization:
  if (!InitInstance(hInstance, nCmdShow)) {
    MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"),
               MB_ICONEXCLAMATION | MB_OK);
    constexpr int return_code = STATUS_BAD;
    return handleReturnCode(return_code);
  } else {
    std::wcout << "Welcome to PiCalc-Win v." << std::endl;
  }

  while (GetMessage(&Msg, NULL, 0, 0) > 0) {
    HWND hMDIClient = getChildHwnd();
    if (!TranslateMDISysAccel(hMDIClient, &Msg)) {
      TranslateMessage(&Msg);
      DispatchMessage(&Msg);
    }
  }
  const UINT_PTR result = Msg.wParam;
  const unsigned int uiResult = result;
  const int kMessageResult = static_cast<int>(uiResult);
  if (kMessageResult == STATUS_GOOD) {
    return STATUS_GOOD;
  }
  return handleReturnCode(kMessageResult);
}


HWND getMainHwnd() {
  if (g_hMainWindow != NULL) {
    return g_hMainWindow;
  } else {
    return nullptr;
  }
}
