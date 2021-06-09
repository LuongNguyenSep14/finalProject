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

//enum TYPE {L = 1, R = 2, E = 3};

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
	//ID Rect, Ellipse, Line ...
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

class Text : public Object
{
private:
	WCHAR buffer[BUFFER_SIZE];
	RECT box;
	
public:
	HWND textBox;

	Text() : Object()
	{
		ID = ID_DRAW_TEXT;
	}

	virtual void draw(HDC& hdc)
	{

		HPEN hpenOld = static_cast<HPEN>(SelectObject(hdc, GetStockObject(DC_PEN)));
		HBRUSH hbrushOld = static_cast<HBRUSH>(SelectObject(hdc, GetStockObject(NULL_BRUSH)));

		HPEN hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 0, 255));
		SelectObject(hdc, hPen);



		RECT rc1;

		rc1.top = getFrom().y < getTo().y ? getFrom().y : getTo().y;
		rc1.bottom = getFrom().y > getTo().y ? getFrom().y : getTo().y;

		rc1.left = getFrom().x < getTo().x ? getFrom().x : getTo().x;
		rc1.right = getFrom().x > getTo().x ? getFrom().x : getTo().x;

		Rectangle(hdc, rc1.left, rc1.top, rc1.right, rc1.bottom);
		setRect(rc1.left, rc1.top, rc1.right, rc1.bottom);


		SetBkMode(hdc, TRANSPARENT);
		SetBkColor(hdc, RGB(0, 0, 0));   // black
		// TODO: Optionally, set a nicer font than the default.
		//DrawText(hdc, TEXT("A!"), -1, &rc1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		SelectObject(hdc, hpenOld);
		SelectObject(hdc, hbrushOld);

	}

	void finish(HDC &hDC)
	{

		HPEN hpenOld = static_cast<HPEN>(SelectObject(hDC, GetStockObject(DC_PEN)));
		HBRUSH hbrushOld = static_cast<HBRUSH>(SelectObject(hDC, GetStockObject(NULL_BRUSH)));

		HPEN hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 0, 255));
		SelectObject(hDC, hPen);


		Rectangle(hDC, box.left, box.top, box.right, box.bottom);


		SetBkMode(hDC, TRANSPARENT);
		SetBkColor(hDC, RGB(0, 0, 0));   // black
		// TODO: Optionally, set a nicer font than the default.
		DrawText(hDC, (LPCWSTR)buffer, -1, &box, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

		SelectObject(hDC, hpenOld);
		SelectObject(hDC, hbrushOld);

	}

	void processing(HWND &hwnd, PAINTSTRUCT &ps, int fromX, int fromY, int toX, int toY)
	{
		HDC hDC = BeginPaint(hwnd, &ps);

		if (toY < fromY) {
			int temp = fromY;
			fromY = toY;
			toY = temp;
		}

		if (toX < fromX) {
			int temp = fromX;
			fromX = toX;
			toX = temp;
		}

		setRect(fromX, fromY, toX, toY);

		int width = abs(fromX - toX);
		int height = abs(fromY - toY);
		textBox = CreateWindowEx(
			NULL, L"EDIT", L"Textbox...",
			WS_CHILD | WS_BORDER | WS_VISIBLE | ES_MULTILINE ,
			fromX, fromY, width, height, hwnd, (HMENU)0, NULL, NULL
			);

	}


	void setRect(int fromX, int fromY, int toX, int toY)
	{
		box.top = fromY < toY ? fromY : toY;
		box.bottom = fromY > toY ? fromY : toY;

		box.left = fromX < toX ? fromX : toX;
		box.right = fromX > toX ? fromX : toX;
	}

	RECT getRect()
	{
		return box;
	}

	WCHAR* getData()
	{
		Edit_GetText(textBox, buffer, BUFFER_SIZE);
		return buffer;
	}

	virtual shared_ptr<Object> nextObject()
	{
		shared_ptr<Object> result = make_shared<Text>();

		return result;
	}
};

