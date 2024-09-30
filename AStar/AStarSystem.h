#pragma once

#include "framework.h"
#include "MyBlock.h"
#include <vector>

enum WallMode
{
	Create,
	Destroy,
};

enum DrawType
{
	DrawWall,
	DrawPoint,
	DrawPath,
};

struct Node
{
	POINT curPos;
	POINT prevPos;
};

struct Huristic
{
	int movingDist;			// 현재까지 이동한 거리
	float predictionDist;	// 휴리스틱 가중치
};

bool operator < (const POINT& lhs, const POINT& rhs);
bool operator < (const Node& lhs, const Node& rhs);
bool operator < (const Huristic& lhs, const Huristic& rhs);

void CreateBlock(RECT rect);
void UpdateBlock(HDC hdc);
void DeleteBlock();

void SetWallMode(WallMode mode);

void ChangeBlockType(double inX, double inY, DrawType type);
void CalculateCost();
std::vector<POINT> AStar(POINT inPos);
//void DfsDestToStart();