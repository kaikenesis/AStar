#include "MyBlock.h"

MyBlock::MyBlock(RECT inRect)
{
	rect = inRect;
}

MyBlock::MyBlock()
{
	rect = { 0,0,10,10 };
}

MyBlock::~MyBlock()
{
}

void MyBlock::Init(HDC hdc)
{
	Draw(hdc);
}

void MyBlock::Update(HDC hdc)
{
	Draw(hdc);
}

void MyBlock::Draw(HDC hdc)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(100, 100, 100));
	switch (blockState)
	{
	case Default:	hBrush = CreateSolidBrush(RGB(0, 0, 0)); break;
	case Start:		hBrush = CreateSolidBrush(RGB(0, 0, 0)); break;
	case Dest:		hBrush = CreateSolidBrush(RGB(0, 0, 0)); break;
	case Path:		hBrush = CreateSolidBrush(RGB(0, 0, 0)); break;
	case Wall:		hBrush = CreateSolidBrush(RGB(0, 0, 0)); break;
	case Candidate:	hBrush = CreateSolidBrush(RGB(0, 0, 0)); break;
	}
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

	SelectObject(hdc, oldBrush);
	DeleteObject(hBrush);

	TCHAR str[128] = { 0 };
	wsprintf(str, _T("%d"), costFromStart);
	TextOut(hdc, rect.left + 1, rect.bottom - 1, str, lstrlen(str));

	memset(str, 0, sizeof(str));
	wsprintf(str, _T("%d"), costFromEnd);
	TextOut(hdc, rect.right - 1, rect.top + 1, str, lstrlen(str));

	memset(str, 0, sizeof(str));
	wsprintf(str, _T("%d"), costTotal);
	TextOut(hdc, rect.left + 1, rect.top + 1, str, lstrlen(str));
}
