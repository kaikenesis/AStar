#pragma once

#include "framework.h"

enum BlockState
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
	int costFromStart;
	int costFromEnd;
	int costTotal;

	BlockState blockState;

public:
	MyBlock(RECT inRect);
	MyBlock();
	~MyBlock();

	void Init(HDC hdc);
	void Update(HDC hdc);

	void Draw(HDC hdc);
};

