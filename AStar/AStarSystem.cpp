
#include "AStarSystem.h"
#include <queue>
#include <map>
using namespace std;

vector<vector<MyBlock*>> blocks;
int col = 0, row = 0;
int offset = 60;
int dx[8] = { -1,-1,-1,0,0,1,1,1 }; // left -> right
int dy[8] = { -1,0,1,-1,1,-1,0,1 }; // top -> bottom
bool isStart = false;
bool isDest = false;
POINT startBlock;
POINT destBlock;
WallMode curMode = Create;


void ChangeBlockType(double inX, double inY, DrawType newType)
{
    int x = inX / offset;
    int y = inY / offset;

    if (y < col && x < row)
    {
        switch (newType)
        {
        case DrawWall:
        {
            if (curMode == Create)
            {
                if (blocks[y][x]->blockType == Start)
                {
                    blocks[y][x]->blockType = Wall;
                    isStart = false;
                }
                else if (blocks[y][x]->blockType == Dest)
                {
                    blocks[y][x]->blockType = Wall;
                    isDest = false;
                }
                else
                    blocks[y][x]->blockType = Wall;
            }
            else if (curMode == Destroy)
            {
                if (blocks[y][x]->blockType == Wall)
                    blocks[y][x]->blockType = Default;
            }
        }
            break;

        case DrawPoint:
        {
            if (blocks[y][x]->blockType == Default)
            {
                if (isStart == false)
                {
                    blocks[y][x]->blockType = Start;
                    startBlock = { x,y };
                    isStart = true;
                }
                else if (isDest == false)
                {
                    blocks[y][x]->blockType = Dest;
                    destBlock = { x,y };
                    isDest = true;
                }
            }
            else if (blocks[y][x]->blockType == Start)
            {
                blocks[y][x]->blockType = Default;
                isStart = false;
            }
            else if (blocks[y][x]->blockType == Dest)
            {
                blocks[y][x]->blockType = Default;
                isDest = false;
            }
            else
            {
                if (isStart == false)
                {
                    blocks[y][x]->blockType = Start;
                    startBlock = { x,y };
                    isStart = true;
                }
                else if (isDest == false)
                {
                    blocks[y][x]->blockType = Dest;
                    destBlock = { x,y };
                    isDest = true;
                }
            }
        }
            break;

        case DrawPath:
            break;
        }
    }
}

void CalculateCost()
{
    for (int i = 0; i < col; i++)
        for (int j = 0; j < row; j++)
            blocks[i][j]->ResetCost();

    if (isStart == true && isDest == true)
        AStar(destBlock);
}

// inPos -> ��������
BOOL AStar(POINT inPos)
{
    priority_queue<pair<Huristic, Node>, vector< pair<Huristic, Node>>, greater< pair<Huristic, Node>>> openQ;
    map<POINT, POINT> closeQ;
    
    pair<Huristic, Node> start;
    start.first.movingDist = 0;
    start.first.predictionDist = -1;
    start.second.curPos = { startBlock.x,startBlock.y };
    start.second.prevPos = { -1,-1 };
    
    openQ.push(start);

    while (!openQ.empty())
    {
        Huristic curHuristic = openQ.top().first;
        Node curNode = openQ.top().second;
        openQ.pop();

        if (closeQ.find(curNode.curPos) != closeQ.end()) continue;

        closeQ.insert({ curNode.curPos, curNode.prevPos });
        
        int x = curNode.curPos.x;
        int y = curNode.curPos.y;

        if (x == inPos.x && y == inPos.y) break;

        for (int i = 0; i < 8; i++)
        {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < row && ny >= 0 && ny < col)
            {
                if (blocks[ny][nx]->blockType == Wall || closeQ.find({ nx,ny }) != closeQ.end()) continue;
                
                Huristic huristic;
                Node nextNode;
                
                int deltaX = nx - inPos.x;
                int deltaY = ny - inPos.y;
                float directDist = sqrt(deltaX * deltaX + deltaY * deltaY);
                
                deltaX = nx - x;
                deltaY = ny - y;
                float moveDist = sqrt(deltaX * deltaX + deltaY * deltaY);

                huristic.movingDist = curHuristic.movingDist + moveDist;    // ������������� ���� �̵��� ��ġ������ �̵��Ÿ� (g)
                huristic.predictionDist = directDist;                       // �������������� ���� �̵��� ��ġ������ �����Ÿ�, Huristic ����ġ (h)

                nextNode.curPos = { nx,ny };
                nextNode.prevPos = { x,y };

                if (blocks[ny][nx]->blockType != Dest) blocks[ny][nx]->blockType = Candidate;
                if (blocks[ny][nx]->costTotal == 0 || blocks[ny][nx]->costTotal > (huristic.movingDist * 10) + (huristic.predictionDist * 10))
                {
                    blocks[ny][nx]->costFromStart = huristic.movingDist * 10;
                    blocks[ny][nx]->costFromEnd = huristic.predictionDist * 10;
                    blocks[ny][nx]->costTotal = (huristic.movingDist * 10) + (huristic.predictionDist * 10);
                }

                openQ.push({ huristic, nextNode });
            }
        }
    }

    if (closeQ.find(inPos) != closeQ.end())
    {
        POINT location = inPos;
        while (location.x != -1)
        {
            if (blocks[location.y][location.x]->blockType != Start && blocks[location.y][location.x]->blockType != Dest)
                blocks[location.y][location.x]->blockType = Path;
            location = closeQ[location];
        }
    }
    else
        return false;

    return true;
}

bool operator<(const POINT& lhs, const POINT& rhs)
{
    if (lhs.x == rhs.x)
        return lhs.y < rhs.y;

    return lhs.x < rhs.x;
}

bool operator<(const Node& lhs, const Node& rhs)
{
    return false;
}

bool operator<(const Huristic& lhs, const Huristic& rhs)
{
    return(lhs.movingDist + lhs.predictionDist) < (rhs.movingDist + rhs.predictionDist);
}

void CreateBlock(RECT rect)
{
    for (int i = rect.top; i < rect.bottom - offset; i += offset)
    {
        vector<MyBlock*> temp;
        blocks.push_back(temp);
        for (int j = rect.left; j < rect.right - offset; j += offset)
        {
            RECT rt = { j,i,j + offset,i + offset };
            MyBlock* block = new MyBlock(rt);
            blocks[i / offset].push_back(block);
        }
    }

    col = blocks.size();
    row = blocks[0].size();
}

void UpdateBlock(HDC hdc)
{
    for (int i = 0; i < col; i++)
        for (int j = 0; j < row; j++)
            blocks[i][j]->Update(hdc);
}

void DeleteBlock()
{
    for (int i = 0; i < col; i++)
        for (int j = 0; j < row; j++)
        delete blocks[i][j];
    blocks.clear();
}

void SetWallMode(WallMode mode)
{
    curMode = mode;
}
