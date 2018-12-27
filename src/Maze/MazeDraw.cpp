//
//  MazeDraw.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "MazeDraw.hpp"
#include "Drawing.hpp"


static const int    kColorFloor = 0xffffff;
static const int    kColorWall  = 0x404040;

static const int    kTagColorCount = 10;
static const int    kTagColors[] = {
    0xE85A70, 0x73D0C2, 0xFFC530, 0xF6E5D7,
    0x9020A0, 0xFF008B, 0xFEE4A6, 0x5893D4,
    0xA7D129, 0x616F39,
};

static const int kColorStart       = 0x40d040;
static const int kColorGoal        = 0xe04040;

static const int kPaddingSizeX = 20;
static const int kPaddingSizeY = 25;
static const int kBorderSize   = 2;
static int gCellSize    = -1;

static const float kWaitMaze   = 0.01f;
static const float kWaitMan    = 0.02f;


static int GetCellX(int x, int xSize)
{
    int bodyX = (gCellSize + kBorderSize) * xSize + kBorderSize;
    int paddingX = (640 - bodyX) / 2;
    return -320 + paddingX + x * (gCellSize + kBorderSize);
}

static int GetCellY(int y, int ySize)
{
    int bodyY = (gCellSize + kBorderSize) * ySize + kBorderSize;
    int paddingY = (480 - bodyY) / 2;
    return -240 + paddingY + (ySize - y - 1) * (gCellSize + kBorderSize);
}

static int GetCrossPointX(int x, int xSize)
{
    int cellX = GetCellX(x, xSize);
    return cellX;
}

static int GetCrossPointY(int y, int ySize)
{
    int cellY = GetCellY(y-1, ySize);
    return cellY;
}

static int CalcCellSize(Maze *maze)
{
    int sizeX = (640 - kPaddingSizeX * 2 - kBorderSize) / maze->GetXSize() - kBorderSize;
    int sizeY = (480 - kPaddingSizeY * 2 - kBorderSize) / maze->GetYSize() - kBorderSize;
    int cellSize = (sizeX < sizeY)? sizeX: sizeY;
    if (cellSize < 1) {
        cellSize = 1;
    }
    return cellSize;
}

void DrawMaze(Maze *maze, bool usesBatch)
{
    if (gCellSize <= 0) {
        gCellSize = CalcCellSize(maze);
    }

    if (usesBatch) {
        StartBatch();
    }
    Clear(kColorLightGray);

    // 床の描画
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            int bx = GetCellX(x, maze->GetXSize());
            int by = GetCellY(y, maze->GetYSize());

            int color = kColorFloor;
            int tag = maze->GetCellTag(x, y);
            if (tag == 1) {
                color = kTagColors[0];
            } else if (tag > 1) {
                color = kTagColors[(tag - 2) % (kTagColorCount - 1) + 1];
            }

            FillRect(bx, by, gCellSize+kBorderSize*2-1, gCellSize+kBorderSize*2-1, color);
        }
    }

    // 壁の描画
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            int bx = GetCellX(x, maze->GetXSize());
            int by = GetCellY(y, maze->GetYSize());
            if (maze->CheckWall(x, y, Up)) {
                FillRect(bx, by+gCellSize+kBorderSize, gCellSize+kBorderSize*2-1, kBorderSize-1, kColorWall);
            }
            if (maze->CheckWall(x, y, Down)) {
                FillRect(bx, by, gCellSize+kBorderSize*2-1, kBorderSize-1, kColorWall);
            }
            if (maze->CheckWall(x, y, Right)) {
                FillRect(bx+gCellSize+kBorderSize, by, kBorderSize-1, gCellSize+kBorderSize*2-1, kColorWall);
            }
            if (maze->CheckWall(x, y, Left)) {
                FillRect(bx, by, kBorderSize-1, gCellSize+kBorderSize*2-1, kColorWall);
            }
        }
    }

    // スタートとゴールの描画
    int sx = GetCellX(0, maze->GetXSize()) + gCellSize/2 + kBorderSize;
    int sy = GetCellY(0, maze->GetYSize()) + gCellSize/2 + kBorderSize;
    int gx = GetCellX(maze->GetXSize()-1, maze->GetXSize()) + gCellSize/2 + kBorderSize;
    int gy = GetCellY(maze->GetYSize()-1, maze->GetYSize()) + gCellSize/2 + kBorderSize;

    int r = gCellSize / 2 - 2;
    if (r < 1) {
        r = 1;
    }
    FillCircle(sx, sy, r, kColorStart);
    if (r >= 2) {
        DrawCircle(sx, sy, r, kColorWhite);
        DrawCircle(sx, sy, r - 1, kColorWhite);
    }
    FillCircle(gx, gy, r, kColorGoal);
    if (r >= 2) {
        DrawCircle(gx, gy, r, kColorWhite);
        DrawCircle(gx, gy, r - 1, kColorWhite);
    }

    if (usesBatch) {
        EndBatch();
        Sleep(kWaitMaze);
    }
}

void DrawCrossPoint(Maze *maze, const CrossPoint& pos, bool usesBatch)
{
    if (usesBatch) {
        StartBatch();
    }

    int x = GetCrossPointX(pos.x, maze->GetXSize());
    int y = GetCrossPointY(pos.y, maze->GetYSize());
    FillCircle(x, y, kBorderSize+1, kColorWall);

    if (usesBatch) {
        EndBatch();
    }
}

void DrawMan(Maze *maze, const CellPoint& pos, Direction dir)
{
    int x = GetCellX(pos.x, maze->GetXSize()) + gCellSize / 2 + kBorderSize;
    int y = GetCellY(pos.y, maze->GetYSize()) + gCellSize / 2 + kBorderSize;
    DrawCircle(x, y, gCellSize/2-4, kColorBlack);
    int x2 = x;
    int y2 = y;
    if (dir == Up) {
        y2 = y2 + gCellSize / 2;
    } else if (dir == Right) {
        x2 = x2 + gCellSize / 2;
    } else if (dir == Down) {
        y2 = y2 - gCellSize / 2;
    } else {
        x2 = x2 - gCellSize / 2;
    }
    DrawLine(x, y, x2, y2, kColorBlack);
    Sleep(kWaitMan);
}

