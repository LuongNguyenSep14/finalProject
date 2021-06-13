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

enum TYPE {L = 1, R = 2, E = 3};
const int BUFFER_SIZE = 255;




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
	int ID;
	Point from, to;
	DWORD color;

public:
	Object()
	{
		ID = 0;
		color = RGB(0, 0, 0);
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

	void setColor(DWORD color)
	{
		this->color = color;
	}

	virtual void setFrom(Point from)
	{
		this->from = from;
	}

	virtual void setTo(Point to)
	{
		this->to = to;
	}

	virtual void setDimens(RECT* rect)
	{
		this->from = Point(rect->left, rect->top);
		this->to = Point(rect->right, rect->bottom);
	}

	virtual RECT* getDimens()
	{
		RECT* rect = new RECT();
		rect->left = this->from.x;
		rect->top = this->from.y;
		rect->right = this->to.x;
		rect->bottom = this->to.y;
		return rect;
	}

	virtual void draw(HDC& hdc) = 0;
	virtual shared_ptr<Object> nextObject() = 0;

};

class Line : public Object
{
public:
	Line() : Object()
	{
		ID = ID_DRAW_LINE;

	}

	virtual void draw(HDC& hdc)
	{
		HPEN hPen = CreatePen(PS_DASHDOT, 3, this->color);
		SelectObject(hdc, hPen);
		
		MoveToEx(hdc, getFrom().x, getFrom().y, NULL);
		LineTo(hdc, getTo().x, getTo().y);
	}

	virtual shared_ptr<Object> nextObject()
	{
		shared_ptr<Object> result = make_shared<Line>();

		return result;
	}
};

class Rect : public Object
{
public:
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
};

class Ellipses : public Object
{
public:
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
};

class Texts : public Object
{
private:
	RECT textRect;
	WCHAR buffer[BUFFER_SIZE];
public:

	Texts() : Object()
	{
		ID = ID_DRAW_TEXT;
	}

	void setRect(int fromX, int fromY, int toX, int toY)
	{
		if (toY < fromY) 
		{
			int temp = fromY;
			fromY = toY;
			toY = temp;
		}

		if (toX < fromX) 
		{
			int temp = fromX;
			fromX = toX;
			toX = temp;
		}

		textRect.left = fromX;
		textRect.right = toX;
		textRect.top = fromY;
		textRect.bottom = toY;
	}

	WCHAR getData(HWND textBox)
	{
		//save data from edit box to buffer
		return Edit_GetText(textBox, buffer, BUFFER_SIZE);
	}

	RECT getRect()
	{
		return textRect;
	}
	

	virtual void draw(HDC& hdc)
	{

		HPEN hPen = CreatePen(PS_DASHDOT, 1, RGB(0, 0, 255));
		Rectangle(hdc, textRect.left, textRect.top, textRect.right, textRect.left);
	}

	virtual shared_ptr<Object> nextObject()
	{
		shared_ptr<Object> result = make_shared<Texts>();

		return result;
	}
};

