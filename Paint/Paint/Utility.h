#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include <commdlg.h>
#include <windowsx.h>

class Object
{
protected:
	int fromX;
	int fromY;
	int toX;
	int toY;
    bool isPreview;
public:
    Object()
    {
        fromX = 0;
        fromY = 0;
        toX = 0;
        toY = 0;
        isPreview = false;
    }

	virtual void OnPaint(HWND hwnd) = 0;
	virtual void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) = 0;
	virtual void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) = 0;
	virtual void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) = 0;
};


class Line : public Object
{

public:
    Line() : Object() {}
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

        // Tạo pen với nét gạch chấm, độ dày là 3, màu đỏ
        HPEN hPen = CreatePen(PS_DASHDOT, 3, RGB(255, 0, 0));
        SelectObject(hdc, hPen);
        MoveToEx(hdc, fromX, fromY, NULL);
        LineTo(hdc, toX, toY);

        EndPaint(hwnd, &ps);
    }
};

class Rect : public Object
{
public:
    Rect() : Object() {}
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

        Rectangle(hdc, fromX, fromY, toX, toY);

        //Ellipse(hdc, fromX, fromY, toX, toY);

        EndPaint(hwnd, &ps);
    }
};

class Ellipses : public Object
{
public:
    Ellipses() : Object() { }

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

        Ellipse(hdc, fromX, fromY, toX, toY);

        EndPaint(hwnd, &ps);
    }

};