#include "dialogs.h"

#include "globals.h"

// Dialog procedure for the "About" menu item
INT_PTR CALLBACK AboutDialogProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);

  // Message handler for about box.
  switch (uMsg) {
    case WM_INITDIALOG: {
      return (INT_PTR)TRUE;
    }
    case WM_COMMAND: {
      WORD cmd_id = LOWORD(wParam);

      if (cmd_id == IDOK || cmd_id == IDCANCEL) {
        EndDialog(hWndDlg, (INT_PTR)cmd_id);
        return (INT_PTR)TRUE;
      }
      break;
    }
  }

  return (INT_PTR)FALSE;
}

// Show our "About" dialog box
void ShowAboutDialog(HWND hWnd) {
  std::wcout << __func__ << std::endl;
  DialogBoxW(g_hInstance, MAKEINTRESOURCE(IDD_ABOUT), hWnd, &AboutDialogProc);
}
