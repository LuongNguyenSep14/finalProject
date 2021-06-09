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

<<<<<<< Updated upstream
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
=======
enum TYPE {L = 1, R = 2, E = 3};


class Point
{
public:
	int x;
	int y;

	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	Point()
	{
		x = 0;
		y = 0;
	}


};

class Object
{
protected:
	int ID, style;
	Point from, to;
	DWORD color;
	int size; //line width = độ dày nét vẽ

public:
	Object()
	{
		ID = 0;
		color = RGB(0, 0, 0);
		style = 0;
		size = 1;
	}

	Point getFrom()
	{
		return from;
	}

	Point getTo()
	{
		return to;
	}

	int getID()
	{
		return ID;
	}

	DWORD getcolor()
	{
		return color;
	}

	int getSize()
	{
		return size;
	}

	int getStyle()
	{
		return style;
	}

	void setColor(DWORD color)
	{
		this->color = color;
	}

	void setSize(int size)
	{
		this->size = size;
	}

	void setStyle(int style)
	{
		this->style = style;
	}

	virtual void setFrom(Point from)
	{
		this->from = from;
	}

	virtual void setTo(Point to)
	{
		this->to = to;
	}

	virtual void draw(HDC& hdc) = 0;
	virtual shared_ptr<Object> nextObject() = 0;
	virtual bool checkCollision(int mouseX, int mouseY) = 0;
	virtual shared_ptr<Object> Clone() = 0;

	void Moving(int mouseX, int mouseY, int oriFx, int oriFy)
	{
		int dx, dy, fx, fy, tx, ty;

		fx = this->getFrom().x;
		fy = this->getFrom().y;
		tx = this->getTo().x;
		ty = this->getTo().y;

		dx = mouseX - oriFx;
		dy = mouseY - oriFy;

		fx += dx;
		fy += dy;
		tx += dx;
		ty += dy;
>>>>>>> Stashed changes

		this->setFrom(Point(fx, fy));
		this->setTo(Point(tx, ty));
	}
};


class Line : public Object
{
private:
    int id_button;
public:
<<<<<<< Updated upstream
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
=======
	Line() : Object()
	{
		ID = ID_DRAW_LINE;

	}

	virtual void draw(HDC& hdc)
	{	
		MoveToEx(hdc, getFrom().x, getFrom().y, NULL);
		LineTo(hdc, getTo().x, getTo().y);
	}

	virtual shared_ptr<Object> nextObject()
	{
		shared_ptr<Object> result = make_shared<Line>();

		return result;
	}

	bool checkCollision(int mouseX, int mouseY)
	{
		int fx = getFrom().x;
		int fy = getFrom().y;
		int tx = getTo().x;
		int ty = getTo().y;

		int dx, dy;
		int delta = 3;

		int width = abs(fx - tx);
		int height = abs(fy - ty);

		dx = mouseX - fx;
		dy = mouseY - fy;
		float d1 = sqrt(dx * dx + dy * dy);

		dx = mouseX - tx;
		dy = mouseY - ty;
		float d2 = sqrt(dx * dx + dy * dy);

		dx = fx - tx;
		dy = fy - ty;
		float dist = sqrt(dx * dx + dy * dy);

		if (d1 + d2 >= dist - delta && d1 + d2 <= dist + delta)
			return true;

		return false;
	}

	shared_ptr<Object> Clone()
	{
		return make_shared<Line>(*this);
	}
>>>>>>> Stashed changes
};

class Rect : public Object
{
private:
    int id_button;
public:
<<<<<<< Updated upstream
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
=======
	Rect() : Object()
	{
		ID = ID_DRAW_RECTANGLE;
	}

	virtual void draw(HDC& hdc)
	{
		Rectangle(hdc, getFrom().x, getFrom().y, getTo().x, getTo().y);
	}

	virtual shared_ptr<Object> nextObject()
	{
		shared_ptr<Object> result = make_shared<Rect>();

		return result;
	}

	bool checkCollision(int mouseX, int mouseY)
	{
		int fx = getFrom().x;
		int fy = getFrom().y;
		int tx = getTo().x;
		int ty = getTo().y;

		int width = abs(fx - tx);
		int height = abs(fy - ty);
		int delta = 3;

		if (fx < tx && fy > ty)
		{
			fy -= height;
			ty += height;
		}
		else if (fx > tx && fy < ty)
		{
			fx -= width;
			tx += width;
		}
		else if (fx > tx && fy > ty)
		{
			swap(fx, tx);
			swap(fy, ty);
		}

		if (((mouseX >= fx - delta && mouseX <= fx + delta) || (mouseX <= tx + delta && mouseX >= tx - delta)) && mouseY >= fy && mouseY <= ty)
			return true;
		if (mouseX >= fx && mouseX <= tx && ((mouseY >= fy - delta && mouseY <= fy + delta) || (mouseY <= ty + delta && mouseY >= ty - delta)))
			return true;
		return false;
	}

	shared_ptr<Object> Clone()
	{
		return make_shared<Rect>(*this);
	}
>>>>>>> Stashed changes
};

class Ellipses : public Object
{
private:
    int id_button;
public:
<<<<<<< Updated upstream
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

=======
	Ellipses() : Object()
	{
		ID = ID_DRAW_ELLIPSE;
	}

	virtual void draw(HDC& hdc)
	{
		Ellipse(hdc, getFrom().x, getFrom().y, getTo().x, getTo().y);
	}

	virtual shared_ptr<Object> nextObject()
	{
		shared_ptr<Object> result = make_shared<Ellipses>();

		return result;
	}

	bool checkCollision(int mouseX, int mouseY)
	{
		float fx = getFrom().x;
		float fy = getFrom().y;
		float tx = getTo().x;
		float ty = getTo().y;

		int width = abs(fx - tx);
		int height = abs(fy - ty);

		if (fx < tx && fy > ty)
		{
			fy -= height;
			ty += height;
		}
		else if (fx > tx && fy < ty)
		{
			fx -= width;
			tx += width;
		}
		else if (fx > tx && fy > ty)
		{
			swap(fx, tx);
			swap(fy, ty);
		}

		float a = (fx - tx) / 2.0;
		float b = (fy - ty) / 2.0;

		float h = (fx + tx) / 2.0;
		float k = (fy + ty) / 2.0;

		float result = pow(mouseX - h, 2) / pow(a, 2) + pow(mouseY - k, 2) / pow(b, 2);
		float delta = 0.1;

		if (result <= 1 + delta && result >= 1 - delta)
		{
			return true;
		}

		return false;
	}

	shared_ptr<Object> Clone()
	{
		//return new Ellipses(*this);
		return make_shared<Ellipses>(*this);
	}
>>>>>>> Stashed changes
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