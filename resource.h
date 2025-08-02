//{{NO_DEPENDENCIES}}
// Microsoft Developer Studio generated include file.
// Used by picalc_win.rc

// These next few lines are where we control version number and copyright year
// Adhere to semver > semver.org
#define MAJOR_VERSION 1
#define MINOR_VERSION 9
#define BUILD_VERSION 9
#define ABOUT_VERSION "PiCalc-Win ver. 1.9.9"
#define ABOUT_COPYRIGHT "Copyright  2025 Alex313031"
#define LEGAL_COPYRIGHT "© 2025 Alex313031 (BSD-3)"

// Main resource IDs, typical values inferred from usag
#define IDR_MAINFRAME           103
#define IDR_MAINMENU            104

// Control IDs
#define IDC_CHILD_EDIT          1001
#define IDC_MAIN_STATUS         1002
#define IDC_MAIN_TOOL           1003
#define IDC_MAIN_MDI            1004

#define IDC_PICALC              2000

// Dialogs
#define IDD_ABOUT               30000

// Icons
#define IDI_PICALC              2001
#define IDI_SMALL               2002

// Menu items
#define ID_FILE_NEW             40001
#define ID_FILE_OPEN            40002
#define ID_FILE_SAVEAS          40003
#define ID_FILE_CLOSE           40004
#define ID_FILE_CLOSEALL        40005
#define ID_EDIT_CUT             40006
#define ID_EDIT_COPY            40007
#define ID_EDIT_PASTE           40008
#define ID_WINDOW_TILE          40009
#define ID_WINDOW_CASCADE       40010
// Default menu items
#define IDM_EXIT                40011
#define IDM_HELP                40012
#define IDM_ABOUT               40013

// MDI IDs
#define ID_MDI_FIRSTCHILD       50000
#define ID_MDI_SECONDCHILD      50001

// For consumers not wanting to supply an IDC_
// See https://stackoverflow.com/a/37919148/19023992
#ifndef IDC_STATIC
 #define IDC_STATIC             -1
#endif // IDC_STATIC

// Next default values for new objects
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
 // See https://learn.microsoft.com/en-us/cpp/mfc/tn035-using-multiple-resource-files-and-header-files-with-visual-cpp
 #define _APS_NO_MFC                 130
 #define _APS_NEXT_RESOURCE_VALUE    103
 #define _APS_NEXT_COMMAND_VALUE     32771
 #define _APS_NEXT_CONTROL_VALUE     1000
 #define _APS_NEXT_SYMED_VALUE       103
#endif // APSTUDIO_READONLY_SYMBOLS
#endif // APSTUDIO_INVOKED
