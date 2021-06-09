<<<<<<< Updated upstream
﻿// Paint.cpp : Defines the entry point for the application.
//
=======
﻿#include "Paint.h"
>>>>>>> Stashed changes

#include "framework.h"
#include "Paint.h"
#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include <commdlg.h>
#include "Utility.h"
#include <fstream>

#define IMAGE_WIDTH     18
#define IMAGE_HEIGHT    18
#define BUTTON_WIDTH    0
#define BUTTON_HEIGHT   0

#define MAX_LOADSTRING 100

CHOOSECOLOR  cc; // Thông tin màu chọn
COLORREF  acrCustClr[16]; // Mảng custom color
DWORD  rgbCurrent = RGB(255, 0, 0); // Red
HBRUSH  hbrush; // Tạo ra brush từ màu đã chọn

CHOOSEFONT  cf;
LOGFONT  lf;
HFONT  hfNew, hfOld, hfont;
HGDIOBJ hfontPrev;
COLORREF rgbPrev;
HDC hdc;

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
<<<<<<< Updated upstream
=======
bool mouseDown = false;
bool selected = false;
bool selectButton = false;
bool isMoving = false;
int mouseX = 0;
int mouseY = 0;

vector<shared_ptr<Object>> shapes;
shared_ptr<Object> obj;
shared_ptr<Object> selectedObj; //for cut, copy, paste maybe?
Object* selectedPtr = NULL;
int oriFx;
int oriFy;

PAINTSTRUCT ps;
POINT p;
RECT rc;
>>>>>>> Stashed changes

int id_button = ID_DRAW_RECTANGLE;
shared_ptr<Object> obj = nullptr;

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

//  FUNCTION: MyRegisterClass()
//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

    return RegisterClassExW(&wcex);
}

<<<<<<< Updated upstream
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
=======
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
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
=======
>>>>>>> Stashed changes
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        //HANDLE_WM_LBUTTONDOWN
        //WM_LBUTTONUP
        //WM_MOUSEMOVE
        //Rect *myRect = new Rect();



        obj = ObjectFactory::instance()->create(id_button); //rect

        HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);

       
        HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
        HANDLE_MSG(hWnd, WM_LBUTTONUP, OnLButtonUp);
        HANDLE_MSG(hWnd, WM_MOUSEMOVE, OnMouseMove);

    case WM_ERASEBKGND:
        return (LRESULT)1;

    default:
        obj = ObjectFactory::instance()->create(ID_DRAW_RECTANGLE);
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
        { 3, ID_DRAW_TEXT,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { 4, ID_SELECT,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
    };
    TBADDBITMAP	tbBitmap = { hInst, IDB_BITMAP1 };
    
    int idx = SendMessage(hToolBarWnd, TB_ADDBITMAP, (WPARAM)sizeof(tbBitmap) / sizeof(TBADDBITMAP),
        (LPARAM)(LPTBADDBITMAP)&tbBitmap);
    
    userButtons[1].iBitmap += idx;
    userButtons[2].iBitmap += idx;
    userButtons[3].iBitmap += idx;
    userButtons[4].iBitmap += idx;
    userButtons[5].iBitmap += idx;

    // Thêm nút bấm vào toolbar
    SendMessage(hToolBarWnd, TB_ADDBUTTONS, (WPARAM)sizeof(userButtons) / sizeof(TBBUTTON),
        (LPARAM)(LPTBBUTTON)&userButtons);

    //STATUS BAR
    HWND StatusBar = CreateWindowEx(0,STATUSCLASSNAME,(PCTSTR)NULL,SBARS_SIZEGRIP |WS_CHILD | WS_VISIBLE,0, 0, 0, 0,
        hwnd, (HMENU)IDC_STATUSBAR, hInst,NULL);

    int StatusSize[] = { 100, 200 , -1 };
    SendMessage(StatusBar, SB_SETPARTS, 3, (LPARAM)&StatusSize);

    //Get RECT
    GetClientRect(hwnd, &rc);

    return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case ID_CHOOSE_COLOR:
        ZeroMemory(&cc, sizeof(CHOOSECOLOR));
        cc.lStructSize = sizeof(CHOOSECOLOR);
        cc.hwndOwner = hwnd;
        cc.lpCustColors = (LPDWORD)acrCustClr;
        cc.rgbResult = rgbCurrent;
        cc.Flags = CC_FULLOPEN | CC_RGBINIT;
        if (ChooseColor(&cc))
        {
            hbrush = CreateSolidBrush(cc.rgbResult);
            rgbCurrent = cc.rgbResult;
        }
        break;

    case ID_CHOOSE_FONT:
        ZeroMemory(&cf, sizeof(CHOOSEFONT));
        cf.lStructSize = sizeof(CHOOSEFONT);
        cf.hwndOwner = hwnd;
        cf.lpLogFont = &lf;
        cf.Flags = CF_SCREENFONTS | CF_EFFECTS;

        if (ChooseFont(&cf) == TRUE)
        {
            hfont = CreateFontIndirect(cf.lpLogFont);
            hfontPrev = SelectObject(hdc, hfont);
            rgbCurrent = cf.rgbColors;
            rgbPrev = SetTextColor(hdc, rgbCurrent);
        }
        break;

    case ID_DRAW_ELLIPSE:
        selectButton = false;
        id_button = ID_DRAW_ELLIPSE;
        SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)L"Ellipse");
        
        break;
    case ID_DRAW_LINE:
        selectButton = false;
        id_button = ID_DRAW_LINE;
        SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)L"Line");

        break;
    case ID_DRAW_RECTANGLE:
        selectButton = false;
        id_button = ID_DRAW_RECTANGLE;
        SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)L"Rectangle");

        break;

    case ID_EDIT_DELETE:
        InvalidateRect(hwnd, NULL, TRUE);
        //isPreview = false;
        //BeginPaint(hdc, &ps);
        break;
    case ID_SELECT:
        selectButton = true;

        SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)L"Select");
        break;
    }  
}

void OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

void OnPaint(HWND hwnd)
{
    obj = Factory::instance()->create(id_button);
    HDC hdcMem;
    HBITMAP hbmMem, hbmOld;
    HPEN hPen;
    //HFONT hfntOld;

    HDC hdc = BeginPaint(hwnd, &ps);
    // Create a compatible DC.
    hdcMem = CreateCompatibleDC(hdc);
    // Create a bitmap big enough for our client rectangle.
    hbmMem = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
    // Select the bitmap into the off-screen DC.
    hbmOld = (HBITMAP)SelectObject(hdcMem, hbmMem);
    // Erase the background.
    FillRect(hdcMem, &rc, HBRUSH(RGB(255, 255, 255)));
    
    //if (hfnt) {hfntOld = SelectObject(hdcMem, hfnt);};

    // Render the image into the offscreen DC.
    //SetBkMode(hdcMem, TRANSPARENT);
    hPen = CreatePen(PS_SOLID, 1, rgbCurrent);
    SelectObject(hdcMem, hPen);

    obj->setColor(rgbCurrent);
    obj->setFrom(Point(fromX, fromY));
    obj->setTo(Point(toX, toY));
    obj->setSize(1);
    obj->setStyle(0); //solid = 0;

    for (int i = 0; i < shapes.size(); i++)
    {
       HPEN hNewPen = CreatePen(shapes[i]->getStyle(), shapes[i]->getSize(), shapes[i]->getcolor());

<<<<<<< Updated upstream
        // Báo hiệu cần xóa đi toàn bộ màn hình
        InvalidateRect(hwnd, NULL, TRUE);
    }
    //obj->OnMouseMove(hwnd, x, y, keyFlags);
}

void OnPaint(HWND hwnd)
{
    //obj->setFromX(fromX);
    //obj->setFromY(fromY);
    //obj->setToX(toX);
    //obj->setToY(toY);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
=======
       SelectObject(hdcMem, hNewPen);
       shapes[i]->draw(hdcMem);
       DeleteObject(hNewPen);
    }
    SelectObject(hdcMem, hPen);
    if (isPreview)
        obj->draw(hdcMem);   

    // Blt the changes to the screen DC.
    BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hdcMem, 0, 0, SRCCOPY);

    // Done with off-screen bitmap and DC.
    //SwapBuffers(hdc);
    SelectObject(hdcMem, hbmOld);
    DeleteObject(hbmMem);
    DeleteDC(hdcMem);
    DeleteObject(hPen);
    ReleaseDC(hwnd, hdc);

    EndPaint(hwnd, &ps);
}

//starting point
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    if (selectButton)
        isPreview = false;
    else isPreview = true;
    mouseDown = true;
>>>>>>> Stashed changes

    if (isPreview)
    {
        fromX = x;
        fromY = y;
    }
    else if (selectButton)
    {
        for(int i=0; i<shapes.size(); i++)
        if (shapes[i]->checkCollision(mouseX, mouseY))
        {
            selected = true;

            selectedPtr = (shapes[i]).get();
            selectedPtr->setColor(RGB(0, 0, 0));
            selectedPtr->setStyle(1);
        }
    }
}

<<<<<<< Updated upstream
    if (id_button == ID_DRAW_RECTANGLE)
        Rectangle(hdc, fromX, fromY, toX, toY);
    else if (id_button == ID_DRAW_ELLIPSE)
        Ellipse(hdc, fromX, fromY, toX, toY);
    else if (id_button == ID_DRAW_LINE)
    {
        MoveToEx(hdc, fromX, fromY, NULL);
        LineTo(hdc, toX, toY);
    }
=======
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    mouseX = x;
    mouseY = y;
>>>>>>> Stashed changes

    WCHAR text[30];
    wsprintf(text, L"%d, %dpx", mouseX, mouseY);
    SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 0, (LPARAM)text);

    if (isPreview)
    {
        toX = x;
        toY = y;
        InvalidateRect(hwnd, &rc, FALSE);
    }
    else if (!isPreview && selected && mouseDown)
    {
        oriFx = selectedPtr->getTo().x;
        oriFy = selectedPtr->getTo().y;

        isMoving = true;
        selectedPtr->Moving(mouseX, mouseY, oriFx, oriFy);
        InvalidateRect(hwnd, &rc, FALSE);
    }
}

void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    if (isPreview)
    {
        shapes.push_back(obj);
        isPreview = false;
    }

    mouseDown = false;
    isMoving = false;
    selected = false;
    selectedPtr = NULL;
        
    InvalidateRect(hwnd, &rc, FALSE);
}