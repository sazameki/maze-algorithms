//
//  MazeDraw.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "MazeDraw.hpp"
#include "Drawing.hpp"


const int kColorFloor           = 0xffffff;
const int kColorWall            = 0x404040;

const int kColorCreateMarker1   = 0xE85A70;
const int kColorCreateMarker2   = 0x73D0C2;
const int kColorCreateMarker3   = 0xFFC530;
const int kColorCreateMarker4   = 0xF6E5D7;

const int kColorSolveMarker1    = 0xf0f060;
const int kColorSolveMarker2    = 0xe0e050;
const int kColorSolveMarker3    = 0xd0d040;

const int kColorStart       = 0x40d040;
const int kColorGoal        = 0xe04040;

const int kBlockSize    = 21;   // 奇数を推奨
const int kBorderSize   = 2;

const float kWaitMaze   = 0.01f;
const float kWaitMan    = 0.02f;


static int GetBlockX(int x, int xSize)
{
    int bodyX = (kBlockSize + kBorderSize) * xSize + kBorderSize;
    return x * (kBlockSize + kBorderSize) - bodyX / 2;
}

static int GetBlockY(int y, int ySize)
{
    int bodyY = (kBlockSize + kBorderSize) * ySize + kBorderSize;
    return (ySize - y - 1) * (kBlockSize + kBorderSize) - bodyY / 2;
}

void DrawMaze(Maze *maze, bool usesBatch)
{
    if (usesBatch) {
        StartBatch();
    }
    Clear(kColorLightGray);

    // 床の描画
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            int bx = GetBlockX(x, maze->GetXSize());
            int by = GetBlockY(y, maze->GetYSize());

            if (maze->CheckFlag(x, y, kBlock_CreateMarker1)) {
                FillRect(bx, by, kBlockSize+kBorderSize*2-1, kBlockSize+kBorderSize*2-1, kColorCreateMarker1);
            } else if (maze->CheckFlag(x, y, kBlock_CreateMarker2)) {
                FillRect(bx, by, kBlockSize+kBorderSize*2-1, kBlockSize+kBorderSize*2-1, kColorCreateMarker2);
            } else if (maze->CheckFlag(x, y, kBlock_CreateMarker3)) {
                FillRect(bx, by, kBlockSize+kBorderSize*2-1, kBlockSize+kBorderSize*2-1, kColorCreateMarker3);
            } else if (maze->CheckFlag(x, y, kBlock_CreateMarker4)) {
                FillRect(bx, by, kBlockSize+kBorderSize*2-1, kBlockSize+kBorderSize*2-1, kColorCreateMarker4);
            } else if (maze->CheckFlag(x, y, kBlock_SolveMarker1)) {
                FillRect(bx, by, kBlockSize+kBorderSize*2-1, kBlockSize+kBorderSize*2-1, kColorSolveMarker1);
            } else if (maze->CheckFlag(x, y, kBlock_SolveMarker2)) {
                FillRect(bx, by, kBlockSize+kBorderSize*2-1, kBlockSize+kBorderSize*2-1, kColorSolveMarker2);
            } else if (maze->CheckFlag(x, y, kBlock_SolveMarker3)) {
                FillRect(bx, by, kBlockSize+kBorderSize*2-1, kBlockSize+kBorderSize*2-1, kColorSolveMarker3);
            } else {
                FillRect(bx, by, kBlockSize+kBorderSize*2-1, kBlockSize+kBorderSize*2-1, kColorFloor);
            }
        }
    }

    // 壁の描画
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            int bx = GetBlockX(x, maze->GetXSize());
            int by = GetBlockY(y, maze->GetYSize());
            if (maze->CheckFlag(x, y, kBlock_TopBorder)) {
                FillRect(bx, by+kBlockSize+kBorderSize, kBlockSize+kBorderSize*2-1, kBorderSize-1, kColorWall);
            }
            if (maze->CheckFlag(x, y, kBlock_BottomBorder)) {
                FillRect(bx, by, kBlockSize+kBorderSize*2-1, kBorderSize-1, kColorWall);
            }
            if (maze->CheckFlag(x, y, kBlock_RightBorder)) {
                FillRect(bx+kBlockSize+kBorderSize, by, kBorderSize-1, kBlockSize+kBorderSize*2-1, kColorWall);
            }
            if (maze->CheckFlag(x, y, kBlock_LeftBorder)) {
                FillRect(bx, by, kBorderSize-1, kBlockSize+kBorderSize*2-1, kColorWall);
            }
        }
    }

    // スタートとゴールの描画
    int sx = GetBlockX(0, maze->GetXSize()) + kBlockSize/2 + kBorderSize;
    int sy = GetBlockY(0, maze->GetYSize()) + kBlockSize/2 + kBorderSize;
    int gx = GetBlockX(maze->GetXSize()-1, maze->GetXSize()) + kBlockSize/2 + kBorderSize;
    int gy = GetBlockY(maze->GetYSize()-1, maze->GetYSize()) + kBlockSize/2 + kBorderSize;
    FillCircle(sx, sy, kBlockSize/2-2, kColorStart);
    DrawCircle(sx, sy, kBlockSize/2-2, kColorWhite);
    DrawCircle(sx, sy, kBlockSize/2-2-1, kColorWhite);
    FillCircle(gx, gy, kBlockSize/2-2, kColorGoal);
    DrawCircle(gx, gy, kBlockSize/2-2, kColorWhite);
    DrawCircle(gx, gy, kBlockSize/2-2-1, kColorWhite);

    if (usesBatch) {
        EndBatch();
        Sleep(kWaitMaze);
    }
}

void DrawMan(Maze *maze, const Point& pos, Direction dir)
{
    int x = GetBlockX(pos.x, maze->GetXSize()) + kBlockSize / 2 + kBorderSize;
    int y = GetBlockY(pos.y, maze->GetYSize()) + kBlockSize / 2 + kBorderSize;
    DrawCircle(x, y, kBlockSize/2-4, kColorBlack);
    int x2 = x;
    int y2 = y;
    if (dir == Up) {
        y2 = y2 + kBlockSize / 2;
    } else if (dir == Right) {
        x2 = x2 + kBlockSize / 2;
    } else if (dir == Down) {
        y2 = y2 - kBlockSize / 2;
    } else {
        x2 = x2 - kBlockSize / 2;
    }
    DrawLine(x, y, x2, y2, kColorBlack);
    Sleep(kWaitMan);
}

