
//
#include "Paint.h"

#define MAX_LOADSTRING  100
#define MAX_BUFF		255

int fromX = 0;
int fromY = 0;
int toX = 0;
int toY = 0;
bool isDown = false;
bool isPreview = false;
shared_ptr<Object> obj;
vector<shared_ptr<Object>> objects;
PAINTSTRUCT ps;

int id_button = ID_DRAW_RECTANGLE;


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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PAINT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON2));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hPaint = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hPaint)
   {
      return FALSE;
   }

   ShowWindow(hPaint, nCmdShow);
   UpdateWindow(hPaint);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
       
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
    //how to define the button ?
    // Thêm nút bấm vào toolbar
    SendMessage(hToolBarWnd, TB_ADDBUTTONS, (WPARAM)sizeof(userButtons) / sizeof(TBBUTTON),
        (LPARAM)(LPTBBUTTON)&userButtons);

    return TRUE;
}

void view_file(TCHAR* path)
{

}

void openFileDialog(HWND hwnd);

void saveFileDialog(HWND hwnd);

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

    case ID_FILE_OPEN:
        //MessageBox( 0, 0, 0, 0 );
        openFileDialog(hwnd);
        break;
    case ID_FILE_NEW:
        MessageBox( 0, 0, 0, 0 );
        break;
    case ID_FILE_SAVE:
        saveFileDialog(hwnd);
        //MessageBox( 0, 0, 0, 0 );
        break;
    case ID_DRAW_ELLIPSE:

        id_button = ID_DRAW_ELLIPSE;
        
        break;
    case ID_DRAW_LINE:

        id_button = ID_DRAW_LINE;
        break;
    case ID_DRAW_RECTANGLE:
        id_button = ID_DRAW_RECTANGLE;
        break;

    case ID_EDIT_DELETE:
        //quick debug
        //Rect
        InvalidateRect(hwnd, NULL, FALSE );
        break;
    }
}

void OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

//starting point
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    isDown = true;
    isPreview = true;
    fromX = x;
    fromY = y;
    HDC hdc = GetDC(hwnd);
    MoveToEx(hdc, x, y, NULL);


}

//release mouse
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    isDown = false;
    isPreview = false;
    // Báo hiệu cần xóa đi toàn bộ màn hình & vẽ lại
    InvalidateRect(hwnd, NULL, TRUE);

   // obj->OnLButtonUp(hwnd, x, y, keyFlags);
    objects.push_back(obj);
}

//preview
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    if (isPreview) 
    {
        toX = x;
        toY = y;

        InvalidateRect(hwnd, NULL, TRUE);
    }

}



void OnPaint(HWND hwnd)
{
    
    obj = Factory::instance()->create(id_button);
    HDC hdc = BeginPaint(hwnd, &ps);



    HPEN hPen = CreatePen(PS_DASHDOT, 3, rgbCurrent);

    SelectObject(hdc, hPen);

    obj->setColor(rgbCurrent);
    obj->setFrom(Point(fromX, fromY));
    obj->setTo(Point(toX, toY));
    obj->draw(hdc);

    for (int i = 0; i < objects.size(); i++)
    {
        hPen = CreatePen(PS_DASHDOT, 3, objects[i]->getcolor());
        SelectObject(hdc, hPen);
        objects[i]->draw(hdc);
    }

    EndPaint(hwnd, &ps);
}

void saveToBinaryFile(string filePath) {
    std::ofstream out;
    out.open(filePath, std::iostream::out | std::iostream::binary | std::iostream::trunc);

    if (out.is_open()) {
        int size = objects.size();
        out.write(reinterpret_cast<const char*>(&size), sizeof(size));

        for (shared_ptr<Object> shape : objects) {
            int id = shape->getID();
            COLORREF color = shape->getcolor();
            RECT* rect = shape->getDimens();
            out.write(reinterpret_cast<const char*>(&id), sizeof(id));
            out.write(reinterpret_cast<const char*>(&color), sizeof(COLORREF));
            out.write(reinterpret_cast<const char*>(rect), sizeof(RECT));
        }
    }
    else {
        OutputDebugString(L"Can't open data.bin to write");
    }

    out.close();
}

void loadFromBinaryFile(string filePath) {
    ifstream in;
    in.open(filePath, ios::in | ios::binary);

    if (in.is_open()) {
        char* buffer = new char[MAX_BUFF];
        int size;
        in.read(buffer, sizeof(size));

        size = buffer[0];
        objects.clear();


        for (int i = 0; i < size; i++)
        {
            char* item_buff = new char[MAX_BUFF];

            shared_ptr<Object> shape;

            int id;
            COLORREF color;
            in.read(item_buff, sizeof(id));
            id = item_buff[0];
            in.read(item_buff, sizeof(COLORREF));
            color = item_buff[0];
            int r = GetRValue(color);
            color = item_buff[1];
            int g = GetGValue(color);
            color = item_buff[2];
            int b = GetBValue(color);

            color = RGB(r, g, b);

            shape = Factory::instance()->create(id);

            shape->setColor(color);

            RECT* rect;
            in.read(item_buff, sizeof(RECT));
            rect = reinterpret_cast<RECT*>(item_buff);
            shape->setDimens(rect);

            objects.push_back(shape);

            delete[] item_buff;
            item_buff = NULL;
        }

        delete[] buffer;
    }
    else {

        OutputDebugString(L"Can't open data.bin to read");
    }

    in.close();
}

void openFileDialog(HWND hwnd)
{
    OPENFILENAME ofn;
    WCHAR szFilePath[MAX_PATH] = L"";
    WCHAR szFileTitle[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Binary Files (*.bin)\0*.bin\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFilePath;
    ofn.lpstrFileTitle = szFileTitle;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"bin";

    if (GetOpenFileName(&ofn))
    {
        // Do something usefull with the filename stored in szFileName 
        wstring ws(szFilePath);
        // your new String
        string fileName(ws.begin(), ws.end());
        loadFromBinaryFile(fileName);
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

void saveFileDialog(HWND hwnd)
{
    OPENFILENAME ofn;
    WCHAR szFileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Binary Files (*.bin)\0*.bin\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = L"bin";

    if (GetSaveFileName(&ofn))
    {
        // Do something usefull with the filename stored in szFileName 
        wstring ws(szFileName);
        // your new String
        string fileName(ws.begin(), ws.end());

        //defaultFilePath = fileName;
        saveToBinaryFile(fileName);
    }
}