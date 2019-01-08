//
//  MazeDraw.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "MazeDraw.hpp"
#include "Drawing.hpp"


/**
    迷路の通常のセルを描画する色
 */
static const int    kColorFloor = 0xffffff;

/**
    迷路の壁を描画する色
 */
static const int    kColorWall  = 0x404040;

/**
    迷路のタグに対応したセルの描画色の個数
 */
static const int    kTagColorCount = 10;

/**
    迷路のタグに対応したセルの描画色のリスト（タグ1に対応した最初の色は一度のみ、それ以外の色は循環して使用されます）
 */
static const int    kTagColors[] = {
    0xe85a70, 0x73d0c2, 0xffc500, 0xa7c5f6,
    0x9020a0, 0xff008b, 0xffff00, 0x5893d4,
    0xa7e020, 0x00a039,
};

/**
    迷路探索の開始位置の描画色
 */
static const int kColorStart       = 0x40d040;

/**
    迷路探索の終了位置の描画色
 */
static const int kColorGoal        = 0xe04040;

/**
    迷路描画のX方向のパディングサイズ
 */
static const int kPaddingSizeX = 0;

/**
    迷路描画のY方向のパディングサイズ
 */
static const int kPaddingSizeY = 0;

/**
    迷路の壁の描画サイズ
 */
static const int kBorderSize   = 1;

/**
    迷路を描画するたびにスリープする時間
 */
static const float kWaitMaze   = 0.0f;

/**
    迷路探索の人形を描画するたびにスリープする時間
 */
static const float kWaitMan    = 0.0f;

/**
    セルの描画サイズ。実行開始時に、セルの個数と描画設定から、自動的に計算されます。
 */
static int gCellSize = -1;


// セルのX方向の描画座標を計算する
static int GetCellX(int x, int xSize)
{
    int bodyX = (gCellSize + kBorderSize) * xSize + kBorderSize;
    int paddingX = (640 - bodyX) / 2;
    return -320 + paddingX + x * (gCellSize + kBorderSize);
}

// セルのY方向の描画座標を計算する
static int GetCellY(int y, int ySize)
{
    int bodyY = (gCellSize + kBorderSize) * ySize + kBorderSize;
    int paddingY = (480 - bodyY) / 2;
    return -240 + paddingY + (ySize - y - 1) * (gCellSize + kBorderSize);
}

// セルの交点のX方向の描画座標を計算する
static int GetCrossPointX(int x, int xSize)
{
    int cellX = GetCellX(x, xSize);
    return cellX;
}

// セルの交点のY方向の描画座標を計算する
static int GetCrossPointY(int y, int ySize)
{
    int cellY = GetCellY(y-1, ySize);
    return cellY;
}

// セルの描画サイズを計算する
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

// 迷路の描画
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

    if (usesBatch) {
        EndBatch();
        Sleep(kWaitMaze);
    }
}

// 交点の描画
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

// スタート位置の描画
void DrawStart(Maze *maze, const CellPoint& start)
{
    int sx = GetCellX(start.x, maze->GetXSize()) + gCellSize/2 + kBorderSize;
    int sy = GetCellY(start.y, maze->GetYSize()) + gCellSize/2 + kBorderSize;
    int r = gCellSize / 2 - 2;
    if (r < 1) {
        r = 1;
    }
    FillCircle(sx, sy, r, kColorStart);
    if (r >= 2) {
        DrawCircle(sx, sy, r, kColorWhite);
        DrawCircle(sx, sy, r - 1, kColorWhite);
    }
}

// ゴール位置の描画
void DrawGoal(Maze *maze, const CellPoint& goal)
{
    int gx = GetCellX(goal.x, maze->GetXSize()) + gCellSize/2 + kBorderSize;
    int gy = GetCellY(goal.y, maze->GetYSize()) + gCellSize/2 + kBorderSize;
    int r = gCellSize / 2 - 2;
    if (r < 1) {
        r = 1;
    }
    FillCircle(gx, gy, r, kColorGoal);
    if (r >= 2) {
        DrawCircle(gx, gy, r, kColorWhite);
        DrawCircle(gx, gy, r - 1, kColorWhite);
    }
}

// 探索用の人形の描画
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

