#pragma once

#ifndef DIALOGS_H_
#define DIALOGS_H_

#include "common.h"
#include "framework.h"
#include "resource.h"

// Dialog procedure for the "About" menu item
INT_PTR CALLBACK AboutDialogProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Show our "About" dialog box
void ShowAboutDialog(HWND hWnd);

#endif // DIALOGS_H_
