#include "MyBlock.h"

MyBlock::MyBlock(RECT inRect)
{
	rect = inRect;
	costFromStart = 0;
	costFromEnd = 0;
	costTotal = 0;

	blockType = Default;
}

MyBlock::MyBlock()
{
	rect = { 0,0,60,60 };
	costFromStart = 0;
	costFromEnd = 0;
	costTotal = 0;

	blockType = Default;
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
	HBRUSH hBrush = NULL;
	switch (blockType)
	{
	case Default:	hBrush = CreateSolidBrush(RGB(255, 255, 255)); break;
	case Start:		hBrush = CreateSolidBrush(RGB(0, 255, 0)); break;
	case Dest:		hBrush = CreateSolidBrush(RGB(255, 0, 0)); break;
	case Path:		hBrush = CreateSolidBrush(RGB(0, 255, 255)); break;
	case Wall:		hBrush = CreateSolidBrush(RGB(0, 0, 0)); break;
	case Candidate:	hBrush = CreateSolidBrush(RGB(100, 100, 100)); break;
	}

	if (hBrush != NULL)
	{
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, oldBrush);
		DeleteObject(hBrush);
	}

	if (blockType != Wall)
	{
		int offset = 3;
		TCHAR str[128] = { 0 };
		RECT rt = rect;
		rt.left += offset;
		rt.right -= offset;
		rt.top += offset;
		rt.bottom -= offset;

		SetBkMode(hdc, TRANSPARENT);
		wsprintf(str, _T("%d"), costFromStart);
		DrawText(hdc, str, lstrlen(str), &rt, DT_LEFT);

		memset(str, 0, sizeof(str));
		wsprintf(str, _T("%d"), costFromEnd);
		DrawText(hdc, str, lstrlen(str), &rt, DT_RIGHT);
		
		memset(str, 0, sizeof(str));
		rt.top = rt.bottom - 16;
		wsprintf(str, _T("%d"), costTotal);
		DrawText(hdc, str, lstrlen(str), &rt, DT_LEFT);
	}
}

void MyBlock::ResetCost()
{
	costFromStart = 0;
	costFromEnd = 0;
	costTotal = 0;

	if (blockType == Wall) return;
	if (blockType == Start) return;
	if (blockType == Dest) return;
	
	blockType = Default;
}
