#include "mdi_handler.h"

#include "libpicalc/libpicalc_dll.h"

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
  WCHAR szFileName[MAX_PATH] = L"";

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

LRESULT CALLBACK MDIChildWndProc(HWND hWnd,
                                 UINT msg,
                                 WPARAM wParam,
                                 LPARAM lParam) {
  switch (msg) {
    case WM_CREATE: {
      HFONT hfDefault;
      HWND hEdit = NULL;
      HWND hDis = NULL;

      // Create Edit Control
      hEdit = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"",
                             WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL |
                                 ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                             0, 0, 100, 100, hWnd, (HMENU)IDC_CHILD_EDIT,
                             GetModuleHandle(NULL), NULL);
      if (!hEdit || hEdit == NULL) {
        MessageBoxW(hWnd, L"Could not create edit box.", L"Error",
                   MB_OK | MB_ICONERROR);
      }
      // Create regular control
      hDis = CreateWindowExW(WS_EX_MDICHILD, L"MDICLIENT", L"hawk",
                             WS_CHILD | WS_VSCROLL,
                             0, 0, 100, 100, hWnd, NULL,
                             GetModuleHandle(NULL), NULL);
      if (!hDis || hDis == NULL) {
        MessageBoxW(hWnd, L"Could not create Pi box.", L"Error",
                   MB_OK | MB_ICONERROR);
      }

      hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
     //SendMessage(hDis, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
    } break;
    case WM_MDIACTIVATE: {
    } break;
    case WM_COMMAND:
      break;
    case WM_SIZE: {
      HWND hEdit;
      RECT rcClient;

      // Calculate remaining height and size edit
      GetClientRect(hWnd, &rcClient);

      hEdit = GetDlgItem(hWnd, IDC_CHILD_EDIT);
      SetWindowPos(hEdit, HWND_TOP, 0, 0, rcClient.right, rcClient.bottom,
                   SWP_NOZORDER);
    } break;
    case WM_PAINT: {
      // Actually paint the contents of our window finally
      //PaintMDI(hWnd, g_hMDIClient);
    } break;
    case WM_MDIDESTROY:
      DestroyWindow(hWnd);
      break;
    default:
      return DefMDIChildProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}
