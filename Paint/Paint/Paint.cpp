#include "Paint.h"

#define MAX_LOADSTRING  100
#define MAX_BUFF		255

int fromX = 0;
int fromY = 0;
int toX = 0;
int toY = 0;
bool isDown = false;
bool isPreview = false;
bool selected = false;
bool selectButton = false;
bool isMoving = false;
bool mouseDown = false;
int mouseX = 0;
int mouseY = 0;
int oriFx;
int oriFy;
int olddx;
int olddy;
int indexCutObj = -1;
DWORD oldColor;
int oldStyle;
string currentFile = "";
bool isNewed;
bool isSaved;
bool isLoaded;
bool isDeleted;

bool copyButton = false;

shared_ptr<Object> cloneObjPtr = NULL;
shared_ptr<Object> obj;
vector<shared_ptr<Object>> objects;
vector<shared_ptr<Object>> objBuffer;
Object* selectedPtr = NULL;


PAINTSTRUCT ps;
POINT p;
RECT rc;

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
        HANDLE_MSG(hWnd, WM_CLOSE, OnClose);
        
    case WM_NOTIFY:
    switch (((LPNMHDR)lParam)->code)
    {
        case TTN_GETDISPINFO:
        {
            LPTOOLTIPTEXT lpttt = (LPTOOLTIPTEXT)lParam;

            // Set the instance of the module that contains the resource.
            lpttt->hinst = hInst;

            UINT_PTR idButton = lpttt->hdr.idFrom;
            switch (idButton)
                {
                case ID_DRAW_LINE:
                    lpttt->lpszText = (LPWSTR)L"Draw Line";
                    break;
                case ID_DRAW_RECTANGLE:
                    lpttt->lpszText = (LPWSTR)L"Draw Rectangle";
                    break;
                case ID_DRAW_ELLIPSE:
                    lpttt->lpszText = (LPWSTR)L"Draw Ellipse";
                    break;
                case ID_DRAW_TEXT:
                    lpttt->lpszText = (LPWSTR)L"Draw Text";
                    break;
                case ID_SELECT:
                    lpttt->lpszText = (LPWSTR)L"Select";
                    break;
                case ID_REDO:
                    lpttt->lpszText = (LPWSTR)L"Redo";
                    break;
                case ID_UNDO:
                    lpttt->lpszText = (LPWSTR)L"Undo";
                    break;
                case ID_EDIT_CUT:
                    lpttt->lpszText = (LPWSTR)L"Cut";
                    break;
                case ID_EDIT_COPY:
                    lpttt->lpszText = (LPWSTR)L"Copy";
                    break;
                case ID_EDIT_PASTE:
                    lpttt->lpszText = (LPWSTR)L"Paste";
                    break;
                case ID_EDIT_DELETE:
                    lpttt->lpszText = (LPWSTR)L"Delete";
                    break;
                case ID_FILE_NEW:
                    lpttt->lpszText = (LPWSTR)L"New file";
                    break;
                case ID_FILE_OPEN:
                    lpttt->lpszText = (LPWSTR)L"Open file";
                    break;
                case ID_FILE_SAVE:
                    lpttt->lpszText = (LPWSTR)L"Save file";
                    break;
                }
           break;
       }
    }
    return TRUE;
    case WM_ERASEBKGND:
        return 1;
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
        { 3, ID_DRAW_TEXT,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { 4, ID_SELECT,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { 5, ID_REDO,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
        { 6, ID_UNDO,	TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 }
    };
    TBADDBITMAP	tbBitmap = { hInst, IDB_BITMAP2 };
    
    int idx = SendMessage(hToolBarWnd, TB_ADDBITMAP, (WPARAM)sizeof(tbBitmap) / sizeof(TBADDBITMAP),
        (LPARAM)(LPTBADDBITMAP)&tbBitmap);
    
    userButtons[1].iBitmap += idx;
    userButtons[2].iBitmap += idx;
    userButtons[3].iBitmap += idx;
    userButtons[4].iBitmap += idx;
    userButtons[5].iBitmap += idx;
    userButtons[6].iBitmap += idx;
    userButtons[7].iBitmap += idx;

    //how to define the button ?
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
    //SetWindowPos(hToolBarWnd, NULL, 0, rc.bottom - 27, rc.right, 27, SWP_NOZORDER);

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
    case ID_FILE_OPEN:
        openFileDialog(hwnd);
        break;
    case ID_FILE_NEW:
        newFileDialog(hwnd);
        break;
    case ID_FILE_SAVE:
        saveFileDialog(hwnd);
        break;
    case ID_REDO:
        /*if (objects.size() > 0)
        {
            shared_ptr<Object> temp = objects.back();
            objects.pop_back();
            objBuffer.push_back(temp);
        }*/
        if (objBuffer.size() > 0)
        {
            shared_ptr<Object> temp = objBuffer.back();
            objBuffer.pop_back();
            objects.push_back(temp);
        }
        InvalidateRect(hwnd, &rc, FALSE);
        
        break;
        
    case ID_UNDO:
        if (objBuffer.size() > 0 && objBuffer.back()->getStatus() == true)
        {
            shared_ptr<Object> temp = objBuffer.back();
            objBuffer.pop_back();
            temp->setStatus(false);
            objects.push_back(temp);
        }
        else if (objects.size() > 0 && !objects.back()->getStatus())
        {
            shared_ptr<Object> temp = objects.back();
            objects.pop_back();
            objBuffer.push_back(temp);
        }
        InvalidateRect(hwnd, &rc, FALSE);
   
        break;
    case ID_DRAW_ELLIPSE:
        selected = false;
        selectButton = false;
        id_button = ID_DRAW_ELLIPSE;
        SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)L"Ellipse");      
        break;
    case ID_DRAW_LINE:
        selectButton = false;
        selected = false;
        id_button = ID_DRAW_LINE;
        SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)L"Line");
        break;
    case ID_DRAW_RECTANGLE:
        selectButton = false;
        selected = false;
        id_button = ID_DRAW_RECTANGLE;
        SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)L"Rectangle");
        break;
    case ID_EDIT_DELETE:
        if (selected)
        {
            Delete();
            selected = false;
            selectButton = false;
            InvalidateRect(hwnd, &rc, FALSE);
        }      
        break;
    case ID_SELECT:
        selectButton = true;
        SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)L"Select");
        break;
    case ID_EDIT_CUT:
        selectButton = false;
        Copy();
        Delete();
        selected = false;
        InvalidateRect(hwnd, &rc, FALSE);
        break;
    case ID_EDIT_COPY:
        selectButton = false;
        Copy();
        if(cloneObjPtr != NULL)
            SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 2, (LPARAM)L"Copied");
        selected = false;
        break;
    case ID_EDIT_PASTE: 
        if (cloneObjPtr != NULL)
        {
            int width = abs(cloneObjPtr->getFrom().x - cloneObjPtr->getTo().x);
            int height = abs(cloneObjPtr->getFrom().y - cloneObjPtr->getTo().y);
            cloneObjPtr->setFrom(Point(5, 30));
            cloneObjPtr->setTo(Point(width + 5, height + 30));
            cloneObjPtr->setColor(oldColor);
            cloneObjPtr->setStyle(oldStyle);

            objects.push_back(cloneObjPtr->Clone());
            SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 2, (LPARAM)L"");

            //SMALL DETAIL
            int IDtemp = objects.back()->getID();
            wstring ttext;

            if (IDtemp == ID_DRAW_LINE)
                ttext = L"Line";
            else if (IDtemp == ID_DRAW_RECTANGLE)
                ttext = L"Rectangle";
            else if (IDtemp == ID_DRAW_ELLIPSE)
                ttext = L"Ellipse";
            SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 1, (LPARAM)ttext.c_str());
            //

            cloneObjPtr.reset();
            cloneObjPtr = NULL;

            InvalidateRect(hwnd, &rc, FALSE);
        }
        break;
    } 
}

void OnClose(HWND hwnd)
{
    if (objects.size() == 0)
    {
        DestroyWindow(hwnd);
    }
    int result = MessageBox(hwnd, L"Do you want to save?", L"My Paint", MB_OKCANCEL | MB_ICONQUESTION);
    if (result == IDOK)
    {
        saveFileDialog(hwnd);
    }
    DestroyWindow(hwnd);
}

void OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

void Copy()
{
    if (selectedPtr != NULL && selected)
    {
        if (cloneObjPtr != NULL)
            cloneObjPtr.reset();

        cloneObjPtr = selectedPtr->Clone();
    }
}

void Delete()
{
    if (indexCutObj != -1 && objects.size() > 0)
    {
        objects[indexCutObj]->setStatus(true);
        objBuffer.push_back(objects[indexCutObj]);
        objects.erase(objects.begin() + indexCutObj);
    }
    indexCutObj = -1;
}
//starting point
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    mouseDown = true;

    if (selectButton)
        isPreview = false;
    else isPreview = true;
    
    if (isPreview)
    {
        fromX = x;
        fromY = y;
    }
    else if (selectButton && !selected)
    {
        for (int i = 0; i < objects.size(); i++)
            if (objects[i]->checkCollision(mouseX, mouseY))
            {
                selectedPtr = (objects[i]).get();
                indexCutObj = i;
                if (selectedPtr != NULL)
                {
                    oldColor = selectedPtr->getcolor();
                    oldStyle = selectedPtr->getStyle();

                    selectedPtr->setColor(RGB(0, 0, 0));
                    selectedPtr->setStyle(1);

                    selected = true;
                }  
                break;
            }
    }
}

//release mouse
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    isMoving = false;
    mouseDown = false;
    if (isPreview)
    {
        objects.push_back(obj);
        isPreview = false;
    }
    InvalidateRect(hwnd, &rc, FALSE);
}

//preview
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    mouseX = x;
    mouseY = y;

    WCHAR text[30];
    wsprintf(text, L"%d, %dpx", mouseX, mouseY);
    SendMessage(GetDlgItem(hwnd, IDC_STATUSBAR), SB_SETTEXT, 0, (LPARAM)text);
    RECT* r = obj->getDimens();

    if (isPreview)
    {
        toX = x;
        toY = y;
        
        InvalidateRect(hwnd, r, FALSE);
    }
    else if (isPreview  == false && selected && mouseDown)
    {
        if (!isMoving)//start to move
        { 
            oriFx = mouseX;
            oriFy = mouseY;
            olddx = oriFx - selectedPtr->getFrom().x;
            olddy = oriFy - selectedPtr->getFrom().y;
            isMoving = true;
        }
        if (isMoving)
        {
            selectedPtr->Moving(mouseX, mouseY, oriFx, oriFy, olddx, olddy);
            InvalidateRect(hwnd, &rc, FALSE);
        }
    }
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
    //SelectObject(hdcMem, hPen);

    if (selectedPtr != NULL && !selectButton)
    {
        selectedPtr->setColor(oldColor);
        selectedPtr->setStyle(oldStyle);
        selectedPtr = NULL;
    }

    obj->setColor(rgbCurrent);
    obj->setFrom(Point(fromX, fromY));
    obj->setTo(Point(toX, toY));
    obj->setSize(1); //line width
    obj->setStyle(0); //solid = 0;

    /*for (int i = 0; i < objects.size(); i++)
    {
       HPEN hNewPen = CreatePen(objects[i]->getStyle(), objects[i]->getSize(), objects[i]->getcolor());
       SelectObject(hdcMem, hNewPen);
       objects[i]->draw(hdcMem);
       DeleteObject(hNewPen);
    }*/
    updateScreen(hdcMem);
  
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

void saveToBinaryFile(string filePath) 
{
    std::ofstream out;
    out.open(filePath, std::iostream::out | std::iostream::binary | std::iostream::trunc);

    if (out.is_open()) 
    {
        isSaved = true;
        int size = objects.size();
        out.write(reinterpret_cast<const char*>(&size), sizeof(size));

        for (shared_ptr<Object> shape : objects) 
        {
            int id = shape->getID();
            COLORREF color = shape->getcolor();
            RECT* rect = shape->getDimens();
            int style = shape->getStyle();
            int size = shape->getSize();
            out.write(reinterpret_cast<const char*>(&id), sizeof(id));
            out.write(reinterpret_cast<const char*>(&color), sizeof(COLORREF));
            out.write(reinterpret_cast<const char*>(rect), sizeof(RECT));
            out.write(reinterpret_cast<const char*>(&style), sizeof(style));
            out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        }
    }
    else 
    {
        isSaved = false;
        OutputDebugString(L"Can't open data.bin to write");
    }

    out.close();
}

void loadFromBinaryFile(string filePath) {
    ifstream in;
    in.open(filePath, ios::in | ios::binary);

    if (in.is_open()) 
    {
        isLoaded = true;
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

            int style;
            in.read(item_buff, sizeof(style));
            style = item_buff[0];

            int size;
            in.read(item_buff, sizeof(style));
            size = item_buff[0];

            shape->setStyle(style);
            shape->setSize(size);

            objects.push_back(shape);

            delete[] item_buff;
            item_buff = NULL;
        }

        delete[] buffer;
    }
    else
    {
        isLoaded = false;
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
        currentFile = fileName;
        loadFromBinaryFile(fileName);
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

void saveFileDialog(HWND hwnd)
{
    OPENFILENAME ofn;
    WCHAR szFileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = L"Binary Files (*.bin)\0*.bin\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = L"bin";

    if (GetSaveFileName(&ofn))
    {
        // Create wstring with the file's path
        wstring ws(szFileName);
        // your new String
        string fileName(ws.begin(), ws.end());
        /*if (isLoaded || isSaved || isNewed)
        {
            saveToBinaryFile(currentFile);
        }
        else
        {
            currentFile = fileName;
            saveToBinaryFile(fileName);
        }*/
        currentFile = fileName;
        saveToBinaryFile(fileName);
    }
}

void newFileDialog(HWND hwnd)
{
    if (objects.size() != 0)
    {
        int result = MessageBox(hwnd, L"Do you want to save?", L"My Paint", MB_OKCANCEL | MB_ICONQUESTION);
        if (result == IDOK)
        {
            isNewed = true;
            saveFileDialog(hwnd);
            objects.clear();
            saveFileDialog(hwnd);
            InvalidateRect(hwnd, NULL, FALSE);
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
        }
        if (result == IDCANCEL)
        {
            isNewed = true;
            objects.clear();
            saveFileDialog(hwnd);
            InvalidateRect(hwnd, NULL, FALSE);
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
        }
    }
    else
    {
        isNewed = true;
        saveFileDialog(hwnd);
    }
}

void updateScreen(HDC hdcMem)
{
    for (int i = 0; i < objects.size(); i++)
    {
        HPEN hNewPen = CreatePen(objects[i]->getStyle(), objects[i]->getSize(), objects[i]->getcolor());
        SelectObject(hdcMem, hNewPen);
        objects[i]->draw(hdcMem);
        DeleteObject(hNewPen);
    }
}
