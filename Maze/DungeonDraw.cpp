//
//  DungeonDraw.cpp
//
//  Created by Satoshi Numata on 2018/12/06.
//  License: Public domain.
//

#include "DungeonDraw.hpp"


static int kBackgroundColor = kColorBlack;
static int kBackWallColor   = 0x202020;
static int kCeilingColor    = 0x505050;
static int kFloorColor      = 0xa0a0a0;
static int kWallFaceColor   = 0xc0c0c0;
static int kWallSideColor   = 0x808080;
static int kBorderColor     = 0x202020;

static int kPaddingX    = -300;
static int kPaddingY    = -180;
static int kBlockSizeX  = 25;
static int kBlockSizeY  = 26;
static int kBorderSize  = 2;

static int kMapSizeX = 10;
static int kMapSizeY = 10;
static int kMapPaddingX = 130;
static int kMapPaddingY = -200;


struct Vector2Int
{
    int x;
    int y;
    
    Vector2Int()
        : x(0), y(0)
    {
        // Do nothing
    }

    Vector2Int(int _x, int _y)
        : x(_x), y(_y)
    {
        // Do nothing
    }
    
    Vector2Int(const Vector2Int& v)
        : x(v.x), y(v.y)
    {
        // Do nothing
    }
};

static Vector2Int ConvertPoint(Maze *maze, const Vector2Int& p)
{
    Vector2Int ret;
    ret.x = p.x * kBlockSizeX + kPaddingX;
    ret.y = (16 - p.y - 1) * kBlockSizeY + kPaddingY;
    return ret;
}

static void FillRect(Maze *maze, const Vector2Int& startPos, const Vector2Int& size, int color)
{
    Vector2Int sp(startPos);
    if (sp.x < 0) {
        sp.x = 0;
    }
    Vector2Int p1 = ConvertPoint(maze, sp);
    Vector2Int p2(startPos);
    p2.x += size.x;
    p2.y += size.y;
    if (p2.x < 0) {
        p2.x = 0;
    } else if (p2.x > 16) {
        p2.x = 16;
    }
    if (p2.y < 0) {
        p2.y = 0;
    }
    p2 = ConvertPoint(maze, p2);
    FillRect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y, color);
}

static void FillQuad(Maze *maze, const Vector2Int& p1, const Vector2Int& p2, const Vector2Int& p3, const Vector2Int& p4, int color)
{
    Vector2Int d1 = ConvertPoint(maze, p1);
    Vector2Int d2 = ConvertPoint(maze, p2);
    Vector2Int d3 = ConvertPoint(maze, p3);
    Vector2Int d4 = ConvertPoint(maze, p4);

    FillTriangle(d1.x, d1.y, d2.x, d2.y, d3.x, d3.y, color);
    FillTriangle(d1.x, d1.y, d3.x, d3.y, d4.x, d4.y, color);
}

static void DrawQuad(Maze *maze, const Vector2Int& p1, const Vector2Int& p2, const Vector2Int& p3, const Vector2Int& p4, int color)
{
    Vector2Int d1 = ConvertPoint(maze, p1);
    Vector2Int d2 = ConvertPoint(maze, p2);
    Vector2Int d3 = ConvertPoint(maze, p3);
    Vector2Int d4 = ConvertPoint(maze, p4);
    
    DrawLine(d1.x, d1.y, d2.x, d2.y, color);
    DrawLine(d2.x, d2.y, d3.x, d3.y, color);
    DrawLine(d3.x, d3.y, d4.x, d4.y, color);
    DrawLine(d4.x, d4.y, d1.x, d1.y, color);
}

static void DrawRect(Maze *maze, const Vector2Int& startPos, const Vector2Int& size, int color)
{
    Vector2Int sp(startPos);
    if (sp.x < 0) {
        sp.x = 0;
    }
    Vector2Int p1 = ConvertPoint(maze, sp);
    Vector2Int p2(startPos);
    p2.x += size.x;
    p2.y += size.y;
    if (p2.x < 0) {
        p2.x = 0;
    } else if (p2.x > 16) {
        p2.x = 16;
    }
    if (p2.y < 0) {
        p2.y = 0;
    }
    p2 = ConvertPoint(maze, p2);
    DrawRect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y, color);
}

static void DrawBackground(Maze *maze)
{
    Clear(kBackgroundColor);
    
    FillRect(maze, Vector2Int(0, 0), Vector2Int(16, 10), kBackWallColor);
    FillRect(maze, Vector2Int(0, 10), Vector2Int(16, 6), kFloorColor);
    FillRect(maze, Vector2Int(0, 0), Vector2Int(16, 6), kCeilingColor);
}

static CellPoint CalcCellPoint(const CellPoint& from, Direction dir, int x, int y)
{
    if (dir == Up) {
        return CellPoint(from.x + x, from.y + y);
    } else if (dir == Right) {
        return CellPoint(from.x - y, from.y + x);
    } else if (dir == Down) {
        return CellPoint(from.x - x, from.y - y);
    } else {
        return CellPoint(from.x + y, from.y - x);
    }
}

int RotateWalls(int data, Direction dir)
{
    // 上方向は変換なし
    if (dir == Up) {
        return data;
    } else {
        bool top = ((data & kBlock_TopBorder) > 0);
        bool bottom = ((data & kBlock_BottomBorder) > 0);
        bool right = ((data & kBlock_RightBorder) > 0);
        bool left = ((data & kBlock_LeftBorder) > 0);

        // 下方向の場合、上と下、左と右が逆
        if (dir == Down) {
            int ret = 0;
            if (top) { ret |= kBlock_BottomBorder; }
            if (bottom) { ret |= kBlock_TopBorder; }
            if (right) { ret |= kBlock_LeftBorder; }
            if (left) { ret |= kBlock_RightBorder; }
            return ret;
        }
        // 右方向の場合、上が左、下が右、右が上、左が下
        else if (dir == Right) {
            int ret = 0;
            if (top) { ret |= kBlock_LeftBorder; }
            if (bottom) { ret |= kBlock_RightBorder; }
            if (right) { ret |= kBlock_TopBorder; }
            if (left) { ret |= kBlock_BottomBorder; }
            return ret;
        }
        // 左方向の場合、上が右、下が左、右が下、左が上
        else {
            int ret = 0;
            if (top) { ret |= kBlock_RightBorder; }
            if (bottom) { ret |= kBlock_LeftBorder; }
            if (right) { ret |= kBlock_BottomBorder; }
            if (left) { ret |= kBlock_TopBorder; }
            return ret;
        }
    }
    return data;
}

void CopyMazeToMiniMaze(Maze *srcMaze, Maze *destMini, const CellPoint& pos, Direction dir)
{
    for (int y = -2; y <= 0; y++) {
        for (int x = -1; x <= 1; x++) {
            CellPoint cell = CalcCellPoint(pos, dir, x, y);
            if (srcMaze->IsValidCell(cell)) {
                int data = srcMaze->GetCellData(cell);
                int data2 = RotateWalls(data, dir);
                destMini->SetCellData(x + 1, y + 2, data2);
            } else {
                destMini->SetCellData(x + 1, y + 2, 0);
            }
        }
    }
}

void DrawMap(Maze *maze, bool map[], const CellPoint& currentPos)
{
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            int color = kColorBlue;
            if (map[y * maze->GetXSize() + x]) {
                color = kColorYellow;
            }
            FillRect(x * kMapSizeX + kMapPaddingX, (maze->GetYSize()-y-1) * kMapSizeY + kMapPaddingY, kMapSizeX, kMapSizeY, color);
        }
    }
    FillRect(currentPos.x * kMapSizeX + kMapPaddingX, (maze->GetYSize()-currentPos.y-1) * kMapSizeY + kMapPaddingY, kMapSizeX, kMapSizeY, kColorRed);
}

/// 人間の入力による迷路探索
static void Move_HumanInput(Maze *maze, bool map[], CellPoint& pos, Direction& dir)
{
    static bool oldLeftKey = false;
    static bool oldRightKey = false;
    static bool oldUpKey = false;
    static bool oldDownKey = false;

    bool leftKey = CheckKey(kKeyLeftArrow);
    bool rightKey = CheckKey(kKeyRightArrow);
    bool upKey = CheckKey(kKeyUpArrow);
    bool downKey = CheckKey(kKeyDownArrow);
    if (leftKey && !oldLeftKey) {
        dir = RotateLeft(dir);
    }
    if (rightKey && !oldRightKey) {
        dir = RotateRight(dir);
    }
    if (upKey && !oldUpKey) {
        CellPoint nextPos = pos.Move(dir);
        if (maze->IsValidCell(nextPos) && !maze->CheckWall(pos, dir)) {
            pos = nextPos;
            map[pos.y * maze->GetXSize() + pos.x] = true;
        }
    }
    if (downKey && !oldDownKey) {
        Direction oppDir = GetOppositeDirection(dir);
        CellPoint nextPos = pos.Move(oppDir);
        if (maze->IsValidCell(nextPos) && !maze->CheckWall(pos, oppDir)) {
            pos = nextPos;
            map[pos.y * maze->GetXSize() + pos.x] = true;
        }
    }
    oldLeftKey = leftKey;
    oldRightKey = rightKey;
    oldUpKey = upKey;
}

/// 右手法による迷路探索
static void Move_RightHand(Maze *maze, bool map[], CellPoint& pos, Direction& dir)
{
    static bool isFinished = false;
    static bool hasMovedForward = false;
    if (isFinished) {
        return;
    }

    // 右側の壁が空いていれば右回転
    if (hasMovedForward) {
        hasMovedForward = false;
        Direction rightDir = RotateRight(dir);
        if (!maze->CheckWall(pos, rightDir)) {
            dir = rightDir;
            Sleep(0.2f);
            return;
        }
    }

    // 正面に壁があったら左回転
    if (maze->CheckWall(pos, dir)) {
        dir = RotateLeft(dir);
    }
    // 正面に壁がなければ前進
    else {
        pos = pos.Move(dir);
        map[pos.y * maze->GetXSize() + pos.x] = true;
        hasMovedForward = true;
    }
    Sleep(0.2f);
}

/// ダンジョン風の迷路表示と探索
void DungeonDraw(Maze *maze)
{
    Maze *mini = new Maze(3, 3);
    CellPoint pos(0, 0);
    Direction dir = Down;
    
    // ミニマップの作成
    bool map[maze->GetXSize() * maze->GetYSize()];
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            map[y * maze->GetXSize() + x] = false;
        }
    }
    map[pos.y * maze->GetXSize() + pos.x] = true;

    // 迷路を解くループ
    while (pos.x != maze->GetXSize() - 1 || pos.y != maze->GetYSize() - 1) {
        //Move_HumanInput(maze, map, pos, dir);
        Move_RightHand(maze, map, pos, dir);

        CopyMazeToMiniMaze(maze, mini, pos, dir);

        StartBatch();
        DrawBackground(maze);
        for (int y = 0; y < 3; y++) {
            for (int x = 0; x < 3; x++) {
                // Up方向の壁
                if (mini->CheckWall(x, y, Up)) {
                    if (y == 0) {
                        FillRect(maze, Vector2Int(2+4*x, 6), Vector2Int(4, 4), kWallFaceColor);
                    } else if (y == 1) {
                        FillRect(maze, Vector2Int(-1+6*x, 5), Vector2Int(6, 6), kWallFaceColor);
                    } else if (y == 2) {
                        FillRect(maze, Vector2Int(-7+10*x, 3), Vector2Int(10, 10), kWallFaceColor);
                    }
                }
            }
            // Left/Right方向の壁
            if (mini->CheckWall(0, y, Left)) {
                if (y == 0) {
                    FillQuad(maze, Vector2Int(0, 5), Vector2Int(0, 11), Vector2Int(2, 10), Vector2Int(2, 6), kWallSideColor);
                }
                // y=1,2の時はこの壁は見えない
            }
            if (mini->CheckWall(1, y, Left)) {
                if (y == 0) {
                    FillQuad(maze, Vector2Int(5, 5), Vector2Int(5, 11), Vector2Int(6, 10), Vector2Int(6, 6), kWallSideColor);
                } else if (y == 1) {
                    FillQuad(maze, Vector2Int(3, 3), Vector2Int(3, 13), Vector2Int(5, 11), Vector2Int(5, 5), kWallSideColor);
                } else if (y == 2) {
                    FillQuad(maze, Vector2Int(0, 0), Vector2Int(0, 16), Vector2Int(3, 13), Vector2Int(3, 3), kWallSideColor);
                }
            }
            if (mini->CheckWall(1, y, Right)) {
                if (y == 0) {
                    FillQuad(maze, Vector2Int(10, 6), Vector2Int(10, 10), Vector2Int(11, 11), Vector2Int(11, 5), kWallSideColor);
                } else if (y == 1) {
                    FillQuad(maze, Vector2Int(11, 5), Vector2Int(11, 11), Vector2Int(13, 13), Vector2Int(13, 3), kWallSideColor);
                } else if (y == 2) {
                    FillQuad(maze, Vector2Int(13, 3), Vector2Int(13, 13), Vector2Int(16, 16), Vector2Int(16, 0), kWallSideColor);
                }
            }
            if (mini->CheckWall(2, y, Right)) {
                if (y == 0) {
                    FillQuad(maze, Vector2Int(14, 6), Vector2Int(14, 10), Vector2Int(16, 11), Vector2Int(16, 5), kWallSideColor);
                }
                // y=1,2の時はこの壁は見えない
            }
        }
        DrawMap(maze, map, pos);
        EndBatch();
    }

    DrawText("SOLVED!! CMD-R TO RESTART.", -12*13, -240, kColorBlue);

    delete mini;
}

