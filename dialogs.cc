#include "dialogs.h"

#include "globals.h"

// Dialog procedure for the "About" menu item
INT_PTR CALLBACK AboutDialogProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);

  // Message handler for about box.
  switch (uMsg) {
    case WM_INITDIALOG: {
      return (INT_PTR)TRUE;
    } break;
    case WM_COMMAND: {
      WORD cmd_id = LOWORD(wParam);
      BOOL clicked = FALSE;

      if (cmd_id == IDOK || cmd_id == IDCANCEL) {
        std::wcout << __func__ << std::endl;
        EndDialog(hWndDlg, (INT_PTR)cmd_id);
        clicked = TRUE;
      }
      return (INT_PTR)clicked;
    } break;
  }

  return (INT_PTR)FALSE;
}

// Show our "About" dialog box
void ShowAboutDialog(HWND hWnd) {
  std::wcout << __func__ << std::endl;
  DialogBoxW(g_hInstance, MAKEINTRESOURCE(IDD_ABOUT), hWnd, &AboutDialogProc);
}
