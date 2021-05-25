// Paint.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Paint.h"
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include <commdlg.h>

#define IMAGE_WIDTH     18
#define IMAGE_HEIGHT    18
#define BUTTON_WIDTH    0
#define BUTTON_HEIGHT   0

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//From windowsx.h
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnDestroy(HWND hwnd);
void OnPaint(HWND hwnd);
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);

//variable
int fromX = 0;
int fromY = 0;
int toX = 0;
int toY = 0;
bool isPreview = false;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PAINT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        //HANDLE_WM_LBUTTONDOWN
        //WM_LBUTTONUP
        //WM_MOUSEMOVE
        HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
        HANDLE_MSG(hWnd, WM_LBUTTONUP, OnLButtonUp);
        HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    // Lấy font hệ thống
    LOGFONT lf;
    GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
    HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
        lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
        lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
        lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
        lf.lfPitchAndFamily, lf.lfFaceName);

    InitCommonControls();
    TBBUTTON tbButtons[] =
    {
        { STD_FILENEW,	ID_FILE_NEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { STD_FILEOPEN,	ID_FILE_OPEN, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { STD_FILESAVE,	ID_FILE_SAVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
    };

    HWND hToolBarWnd = CreateToolbarEx(hwnd,
        WS_CHILD | WS_VISIBLE | CCS_ADJUSTABLE | TBSTYLE_TOOLTIPS,
        ID_TOOLBAR, sizeof(tbButtons) / sizeof(TBBUTTON), HINST_COMMCTRL,
        0, tbButtons, sizeof(tbButtons) / sizeof(TBBUTTON),
        BUTTON_WIDTH, BUTTON_HEIGHT, IMAGE_WIDTH, IMAGE_HEIGHT,
        sizeof(TBBUTTON));

    TBBUTTON buttonsToAdd[] =
    {
        { 0, 0,	TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 }, // Nút phân cách
        { STD_CUT,	ID_EDIT_CUT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { STD_COPY,	ID_EDIT_COPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { STD_PASTE, ID_EDIT_PASTE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { STD_DELETE, ID_EDIT_DELETE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
    };
    SendMessage(hToolBarWnd, TB_ADDBUTTONS, (WPARAM)sizeof(buttonsToAdd) / sizeof(TBBUTTON),
        (LPARAM)(LPTBBUTTON)&buttonsToAdd);

    TBBUTTON userButtons[] =
    {
        { 0, 0,	TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
        { 0, ID_DRAW_ELLIPSE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { 1, ID_DRAW_RECTANGLE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { 2, ID_DRAW_LINE,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { 3, ID_DRAW_TEXT,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
    };
    TBADDBITMAP	tbBitmap = { hInst, IDB_BITMAP1 };
    
    int idx = SendMessage(hToolBarWnd, TB_ADDBITMAP, (WPARAM)sizeof(tbBitmap) / sizeof(TBADDBITMAP),
        (LPARAM)(LPTBADDBITMAP)&tbBitmap);
    
    userButtons[1].iBitmap += idx;
    userButtons[2].iBitmap += idx;
    userButtons[3].iBitmap += idx;
    userButtons[4].iBitmap += idx;
    // Thêm nút bấm vào toolbar
    SendMessage(hToolBarWnd, TB_ADDBUTTONS, (WPARAM)sizeof(userButtons) / sizeof(TBBUTTON),
        (LPARAM)(LPTBBUTTON)&userButtons);

    return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{

}

void OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

//starting point
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    isPreview = true;
    fromX = x;
    fromY = y;
    HDC hdc = GetDC(hwnd);
    MoveToEx(hdc, x, y, NULL);
}

//release mouse
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    isPreview = false;
    // Báo hiệu cần xóa đi toàn bộ màn hình & vẽ lại
    InvalidateRect(hwnd, NULL, TRUE);
}

//preview
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    if (isPreview) 
    {
        toX = x;
        toY = y;

        // Báo hiệu cần xóa đi toàn bộ màn hình
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

void OnPaint(HWND hwnd)
{

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);

    //Rectangle(hdc, 100, 200, 300, 300);

    Rectangle(hdc, fromX, fromY, toX, toY);

    //Ellipse(hdc, fromX, fromY, toX, toY);

    EndPaint(hwnd, &ps);
}




