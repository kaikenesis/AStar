#pragma once

#include "framework.h"

enum BlockType
{
	Default,
	Start,
	Dest,
	Path,
	Wall,
	Candidate
};

class MyBlock
{
private:
	RECT rect;

public:
	int costFromStart = 0;
	int costFromEnd = 0;
	int costTotal = 0;

	BlockType blockType;

public:
	MyBlock(RECT inRect);
	MyBlock();
	~MyBlock();

	double GetWidth() { return rect.right - rect.left; }
	double GetHeight() { return rect.bottom - rect.top; }

	void Init(HDC hdc);
	void Update(HDC hdc);

	void Draw(HDC hdc);
	void ResetCost();
};

