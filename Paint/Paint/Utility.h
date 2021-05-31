#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <commctrl.h>
#pragma comment(lib, "Comctl32.lib")
#include <commdlg.h>
#include <windowsx.h>
#include <memory>
#include <vector>

using namespace std;

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

    virtual void setToX(int toX)
    {
        this->toX = toX;
    }

    virtual void setToY(int toY)
    {
        this->toY = toY;
    }

    virtual void setFromX(int fromX)
    {
        this->fromX = fromX;
    }
    virtual void setFromY(int fromY)
    {
        this->fromY = fromY;
    }

    virtual void setPreview(bool preview)
    {
        this->isPreview = preview;
    }

	virtual void OnPaint(HWND hwnd) = 0;
	virtual void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) = 0;
	virtual void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags) = 0;
	virtual void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags) = 0;
    virtual int getID() = 0;
    virtual shared_ptr<Object> nextObject() = 0;

};


class Line : public Object
{
private:
    int id_button;
public:
    Line() : Object() 
    {
        id_button = ID_DRAW_LINE;
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

        // Tạo pen với nét gạch chấm, độ dày là 3, màu đỏ
        HPEN hPen = CreatePen(PS_DASHDOT, 3, RGB(255, 0, 0));
        SelectObject(hdc, hPen);
        MoveToEx(hdc, fromX, fromY, NULL);
        LineTo(hdc, toX, toY);

        EndPaint(hwnd, &ps);
    }

    int getID()
    {
        return id_button;
    }

    shared_ptr<Object> nextObject()
    {
        shared_ptr<Object> result = make_shared<Line>();

        return result;
    }
};

class Rect : public Object
{
private:
    int id_button;
public:
    Rect() : Object() 
    {
        id_button = ID_DRAW_RECTANGLE;
    }
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

    int getID()
    {
        return id_button;
    }

    shared_ptr<Object> nextObject()
    {
        shared_ptr<Object> result = make_shared<Rect>();

        return result;
    }
};

class Ellipses : public Object
{
private:
    int id_button;
public:
    Ellipses() : Object() 
    { 
        id_button = ID_DRAW_ELLIPSE;
    }

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

    int getID()
    {
        return id_button;
    }

    shared_ptr<Object> nextObject()
    {
        shared_ptr<Object> result = make_shared<Ellipses>();

        return result;
    }

};

class ObjectFactory
{
private:
    static ObjectFactory* _instance;
    vector<shared_ptr<Object>> _prototype;

    ObjectFactory()
    {
        _prototype.push_back(make_shared<Rect>());
        _prototype.push_back(make_shared<Line>());
        _prototype.push_back(make_shared<Ellipses>());
    }
public:

    static ObjectFactory* instance()
    {
        if (_instance == nullptr)
            _instance = new ObjectFactory();
        return _instance;
    }

    int size()
    {
        return _prototype.size();
    }

    shared_ptr<Object> create(int id_button)
    {
        for (int i = 0; i < _prototype.size(); i++)
        {
            if (_prototype[i]->getID() == id_button)
            {
                shared_ptr<Object> result = _prototype[i]->nextObject();
                return result;
            }
        }
        return nullptr;

    }
};
ObjectFactory* ObjectFactory::_instance = nullptr;