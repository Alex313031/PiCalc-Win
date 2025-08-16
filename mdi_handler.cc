#include "mdi_handler.h"

#include "libpicalc/libpicalc.h"

#include "dialogs.h"
#include "globals.h"
#include "resource.h"

HWND g_hMDIClient = NULL;

BOOL LoadTextFileToEdit(HWND hEdit, LPCTSTR pszFileName) {
  HANDLE hFile;
  BOOLEAN bSuccess = FALSE;

  hFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
                     OPEN_EXISTING, 0, NULL);
  if (hFile != INVALID_HANDLE_VALUE) {
    DWORD dwFileSize;

    dwFileSize = GetFileSize(hFile, NULL);
    if (dwFileSize != 0xFFFFFFFF) {
      LPTSTR pszFileText;

      pszFileText = (LPTSTR)GlobalAlloc(GPTR, dwFileSize + 1);
      if (pszFileText != NULL) {
        DWORD dwRead;

        if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL)) {
          pszFileText[dwFileSize] = 0;  // Add null terminator
          if (SetWindowText(hEdit, pszFileText)) {
            bSuccess = TRUE;  // It worked!
          }
        }
        GlobalFree(pszFileText);
      }
    }
    CloseHandle(hFile);
  }
  return bSuccess;
}

void DoFileOpen(HWND hwnd) {
  OPENFILENAME ofn;
  TCHAR szFileName[MAX_PATH] = _T("");

  ZeroMemory(&ofn, sizeof(ofn));

  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFilter = _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
  ofn.lpstrFile = szFileName;
  ofn.nMaxFile = MAX_PATH;
  ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
  ofn.lpstrDefExt = _T("txt");
  if (GetOpenFileName(&ofn)) {
    HWND hEdit = GetDlgItem(hwnd, IDC_CHILD_EDIT);
    if (LoadTextFileToEdit(hEdit, szFileName)) {
      SendDlgItemMessage(g_hMainWindow, IDC_MAIN_STATUS, SB_SETTEXT, 0,
                         (LPARAM)_T("Opened..."));
      SendDlgItemMessage(g_hMainWindow, IDC_MAIN_STATUS, SB_SETTEXT, 1,
                         (LPARAM)szFileName);

      SetWindowText(hwnd, szFileName);
    }
  }
}

HWND CreateNewMDIChild(HWND hMDIClient) {
  MDICREATESTRUCT mcs;
  HWND hChild = NULL;

  mcs.szTitle = szEmptyFileName;
  mcs.szClass = g_szChildClassName;
  mcs.hOwner = GetModuleHandle(NULL);
  mcs.x = mcs.cx = CW_USEDEFAULT;
  mcs.y = mcs.cy = CW_USEDEFAULT;
  mcs.style = MDIS_ALLCHILDSTYLES;

  hChild = (HWND)SendMessage(hMDIClient, WM_MDICREATE, 0, (LONG_PTR)&mcs);
  if (!hChild || hChild ==  NULL) {
    MessageBox(hMDIClient, _T("MDI Child creation failed."), _T("Uh Oh..."),
               MB_ICONEXCLAMATION | MB_OK);
  }
  return hChild;
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
    case WM_CLOSE: {
      DestroyWindow(hwnd);
    } break;
    case WM_DESTROY: {
      PostQuitMessage(STATUS_GOOD);
    } break;
    case WM_GETMINMAXINFO: {
      lpMMI->ptMinTrackSize.x = 300;
      lpMMI->ptMinTrackSize.y = 200;
    } break;
    case WM_COMMAND: {
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
