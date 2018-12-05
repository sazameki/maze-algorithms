//
//  Maze.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "Maze.hpp"
#include "MazeDraw.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>


// 0〜3ビット目は壁
const int   kBlock_TopBorder    = 0x01;
const int   kBlock_RightBorder  = 0x02;
const int   kBlock_BottomBorder = 0x04;
const int   kBlock_LeftBorder   = 0x08;
const int   kBlock_AllBorders   = 0x0f;

// 4〜7ビット目は生成用のマーカー
const int   kBlock_CreateMarker1    = 0x10;
const int   kBlock_CreateMarker2    = 0x20;
const int   kBlock_CreateMarker3    = 0x40;
const int   kBlock_CreateMarker4    = 0x80;
const int   kBlock_AllCreateMarkers = 0xf0;

// 8〜10ビット目は迷路を解くためのマーカー
const int   kBlock_SolveMarker1     = 0x100;
const int   kBlock_SolveMarker2     = 0x200;
const int   kBlock_SolveMarker3     = 0x400;
const int   kBlock_AllSolveMarkers  = 0x800;


#pragma mark - CellPoint構造体

CellPoint::CellPoint()
    : x(0), y(0)
{
    // Do nothing
}

CellPoint::CellPoint(int _x, int _y)
    : x(_x), y(_y)
{
    // Do nothing
}

CellPoint::CellPoint(const CellPoint& p)
    : x(p.x), y(p.y)
{
    // Do nothing
}

CellPoint CellPoint::Move(Direction dir) const
{
    CellPoint ret(*this);
    if (dir == Up) {
        ret.y -= 1;
    } else if (dir == Right) {
        ret.x += 1;
    } else if (dir == Down) {
        ret.y += 1;
    } else {
        ret.x -= 1;
    }
    return ret;
}


#pragma mark - CrossPoint構造体

CrossPoint::CrossPoint()
    : x(0), y(0)
{
    // Do nothing
}

CrossPoint::CrossPoint(int _x, int _y)
    : x(_x), y(_y)
{
    // Do nothing
}

CrossPoint::CrossPoint(const CrossPoint& pos)
    : x(pos.x), y(pos.y)
{
    // Do nothing
}

CrossPoint CrossPoint::Move(Direction dir) const
{
    CrossPoint ret(*this);
    if (dir == Up) {
        ret.y -= 1;
    } else if (dir == Right) {
        ret.x += 1;
    } else if (dir == Down) {
        ret.y += 1;
    } else {
        ret.x -= 1;
    }
    return ret;
}


#pragma mark - Wall構造体

Wall::Wall(const CellPoint& _pos, Direction _dir)
    : pos(_pos), dir(_dir)
{
    // Do nothing
}


#pragma mark - Mazeクラスのコンストラクタ・デストラクタ

Maze::Maze(int _xSize, int _ySize)
{
    Create(_xSize, _ySize, 0);
}

Maze::Maze(int _xSize, int _ySize, int _data)
{
    Create(_xSize, _ySize, _data);
}

Maze::~Maze()
{
    delete[] data;
}

void Maze::Create(int _xSize, int _ySize, int _data)
{
    assert(_xSize >= 2);
    assert(_ySize >= 2);

    xSize = _xSize;
    ySize = _ySize;
    data = new int[xSize * ySize];

    for (int i = 0; i < xSize * ySize; i++) {
        data[i] = _data;
    }
}


#pragma mark - MazeクラスのGetter系の関数

int Maze::GetXSize() const
{
    return xSize;
}

int Maze::GetYSize() const
{
    return ySize;
}

int Maze::GetCellData(int x, int y) const
{
    return data[y * xSize + x];
}

int Maze::GetCellData(const CellPoint& pos) const
{
    return data[pos.y * xSize + pos.x];
}

bool Maze::CheckCellFlag(int x, int y, int flag) const
{
    return ((data[y * xSize + x] & flag) > 0);
}

bool Maze::CheckCellFlag(const CellPoint& pos, int flag) const
{
    return ((data[pos.y * xSize + pos.x] & flag) > 0);
}

bool Maze::IsValidCell(const CellPoint& pos) const
{
    return (pos.x >= 0 && pos.x < xSize && pos.y >= 0 && pos.y < ySize);
}


#pragma mark - Mazeクラスの迷路生成用の関数

void Maze::AddCellFlag(int x, int y, int flag)
{
    data[y * xSize + x] |= flag;
}

void Maze::AddCellFlag(const CellPoint& pos, int flag)
{
    data[pos.y * xSize + pos.x] |= flag;
}

void Maze::RemoveCellFlag(int x, int y, int flag)
{
    data[y * xSize + x] &= ~flag;
}

void Maze::RemoveCellFlag(const CellPoint& pos, int flag)
{
    data[pos.y * xSize + pos.x] &= ~flag;
}

bool Maze::CheckWall(int x, int y, Direction dir) const
{
    if (dir == Up) {
        return ((data[y * xSize + x] & kBlock_TopBorder) > 0);
    } else if (dir == Right) {
        return ((data[y * xSize + x] & kBlock_RightBorder) > 0);
    } else if (dir == Down) {
        return ((data[y * xSize + x] & kBlock_BottomBorder) > 0);
    } else {
        return ((data[y * xSize + x] & kBlock_LeftBorder) > 0);
    }
}

bool Maze::CheckWall(const CellPoint& pos, Direction dir) const
{
    return CheckWall(pos.x, pos.y, dir);
}

bool Maze::CheckWall(const Wall& wall) const
{
    return CheckWall(wall.pos.x, wall.pos.y, wall.dir);
}

void Maze::MakeWall(int x, int y, Direction dir)
{
    assert(x >= 0 && x <= xSize - 1);
    assert(y >= 0 && y <= ySize - 1);

    if (dir == Up) {
        data[y * xSize + x] |= kBlock_TopBorder;
        int ny = y - 1;
        if (ny >= 0) {
            data[ny * xSize + x] |= kBlock_BottomBorder;
        }
    } else if (dir == Right) {
        data[y * xSize + x] |= kBlock_RightBorder;
        int nx = x + 1;
        if (nx < xSize) {
            data[y * xSize + nx] |= kBlock_LeftBorder;
        }
    } else if (dir == Down) {
        data[y * xSize + x] |= kBlock_BottomBorder;
        int ny = y + 1;
        if (ny < ySize) {
            data[ny * xSize + x] |= kBlock_TopBorder;
        }
    } else {
        data[y * xSize + x] |= kBlock_LeftBorder;
        int nx = x - 1;
        if (nx >= 0) {
            data[y * xSize + nx] |= kBlock_RightBorder;
        }
    }
}

void Maze::MakeWall(const CellPoint& pos, Direction dir)
{
    return MakeWall(pos.x, pos.y, dir);
}

void Maze::MakeWall(const Wall& wall)
{
    return MakeWall(wall.pos.x, wall.pos.y, wall.dir);
}

bool Maze::CanRemoveWall(int x, int y, Direction dir) const
{
    if (x == 0 && dir == Left) {
        return false;
    }
    if (y == 0 && dir == Up) {
        return false;
    }
    if (x == xSize - 1 && dir == Right) {
        return false;
    }
    if (y == ySize - 1 && dir == Down) {
        return false;
    }
    return CheckWall(x, y, dir);
}

bool Maze::CanRemoveWall(const CellPoint& pos, Direction dir) const
{
    return CanRemoveWall(pos.x, pos.y, dir);
}

bool Maze::CanRemoveWall(const Wall& wall) const
{
    return CanRemoveWall(wall.pos.x, wall.pos.y, wall.dir);
}

void Maze::RemoveWall(int x, int y, Direction dir)
{
    if (dir == Up) {
        RemoveCellFlag(x, y, kBlock_TopBorder);
        int ny = y - 1;
        if (ny >= 0) {
            RemoveCellFlag(x, ny, kBlock_BottomBorder);
        }
    } else if (dir == Right) {
        RemoveCellFlag(x, y, kBlock_RightBorder);
        int nx = x + 1;
        if (nx < xSize) {
            RemoveCellFlag(nx, y, kBlock_LeftBorder);
        }
    } else if (dir == Down) {
        RemoveCellFlag(x, y, kBlock_BottomBorder);
        int ny = y + 1;
        if (ny < ySize) {
            RemoveCellFlag(x, ny, kBlock_TopBorder);
        }
    } else {
        RemoveCellFlag(x, y, kBlock_LeftBorder);
        int nx = x - 1;
        if (nx >= 0) {
            RemoveCellFlag(nx, y, kBlock_RightBorder);
        }
    }
}

void Maze::RemoveWall(const CellPoint& pos, Direction dir)
{
    RemoveWall(pos.x, pos.y, dir);
}

void Maze::RemoveWall(const Wall& wall)
{
    RemoveWall(wall.pos.x, wall.pos.y, wall.dir);
}


#pragma mark - Mazeクラスの迷路生成用の関数（交点ベース）

bool Maze::IsValidCrossPoint(const CrossPoint& cp) const
{
    return (cp.x >= 0 && cp.x <= xSize && cp.y >= 0 && cp.y <= ySize);
}

bool Maze::CanMoveFromCrossPoint(const CrossPoint& cp, Direction dir) const
{
    CrossPoint cp2 = cp.Move(dir);
    return IsValidCrossPoint(cp2);
}

bool Maze::CheckWallFromCrossPoint(const CrossPoint& cp) const
{
    CellPoint leftUpCell(cp.x - 1, cp.y - 1);
    if (IsValidCell(leftUpCell) && (CheckWall(leftUpCell, Down) || CheckWall(leftUpCell, Right))) {
        return true;
    }

    CellPoint rightUpCell(cp.x, cp.y - 1);
    if (IsValidCell(rightUpCell) && (CheckWall(rightUpCell, Left) || CheckWall(rightUpCell, Down))) {
        return true;
    }

    CellPoint leftDownCell(cp.x - 1, cp.y);
    if (IsValidCell(leftDownCell) && (CheckWall(leftDownCell, Up) || CheckWall(leftDownCell, Right))) {
        return true;
    }

    CellPoint rightDownCell(cp.x, cp.y);
    if (IsValidCell(rightDownCell) && (CheckWall(rightDownCell, Left) || CheckWall(rightDownCell, Up))) {
        return true;
    }

    return false;
}

bool Maze::CheckWallFromCrossPoint(const CrossPoint& cp, Direction dir) const
{
    CellPoint leftUpCell(cp.x - 1, cp.y - 1);
    CellPoint rightUpCell(cp.x, cp.y - 1);
    CellPoint leftDownCell(cp.x - 1, cp.y);
    CellPoint rightDownCell(cp.x, cp.y);

    if (dir == Up) {
        if ((IsValidCell(leftUpCell) && CheckWall(leftUpCell, Right)) ||
            (IsValidCell(rightUpCell) && CheckWall(leftUpCell, Left)))
        {
            return true;
        }
    } else if (dir == Right) {
        if ((IsValidCell(rightUpCell) && CheckWall(rightUpCell, Down)) ||
            (IsValidCell(rightDownCell) && CheckWall(rightDownCell, Up)))
        {
            return true;
        }
    } else if (dir == Down) {
        if ((IsValidCell(leftDownCell) && CheckWall(leftDownCell, Right)) ||
            (IsValidCell(rightDownCell) && CheckWall(rightDownCell, Left)))
        {
            return true;
        }
    } else {
        if ((IsValidCell(leftUpCell) && CheckWall(leftUpCell, Down)) ||
            (IsValidCell(leftDownCell) && CheckWall(leftDownCell, Up)))
        {
            return true;
        }
    }
    return false;
}

void Maze::MakeWallFromCrossPoint(const CrossPoint& cp, Direction dir)
{
    CellPoint leftUpCell(cp.x - 1, cp.y - 1);
    CellPoint rightUpCell(cp.x, cp.y - 1);
    CellPoint leftDownCell(cp.x - 1, cp.y);
    CellPoint rightDownCell(cp.x, cp.y);

    if (dir == Up) {
        if (IsValidCell(leftUpCell)) {
            MakeWall(leftUpCell, Right);
            return;
        } else if (IsValidCell(rightUpCell)) {
            MakeWall(rightUpCell, Left);
            return;
        }
    } else if (dir == Right) {
        if (IsValidCell(rightUpCell)) {
            MakeWall(rightUpCell, Down);
            return;
        } else if (IsValidCell(rightDownCell)) {
            MakeWall(rightDownCell, Up);
            return;
        }
    } else if (dir == Down) {
        if (IsValidCell(leftDownCell)) {
            MakeWall(leftDownCell, Right);
            return;
        } else if (IsValidCell(rightDownCell)) {
            MakeWall(rightDownCell, Left);
            return;
        }
    } else {
        if (IsValidCell(leftUpCell)) {
            MakeWall(leftUpCell, Down);
            return;
        } else if (IsValidCell(leftDownCell)) {
            MakeWall(leftDownCell, Up);
            return;
        }
    }

    throw runtime_error("Invalid wall making from cross point.");
}


#pragma mark - Mazeクラスの迷路の描画

void Maze::Draw(bool usesBatch)
{
    DrawMaze(this, usesBatch);
}

void Maze::DrawCrossPoint(const CrossPoint& pos, bool usesBatch)
{
    ::DrawCrossPoint(this, pos, usesBatch);
}


#pragma mark - 迷路生成用のヘルパー関数

vector<Direction>   MakeAllDirectionsList()
{
    return { Up, Right, Down, Left };
}

vector<Direction>   MakeAllDirectionsList_shuffled()
{
    vector<Direction> dirList = { Up, Right, Down, Left };
    random_shuffle(dirList.begin(), dirList.end());
    return dirList;
}

