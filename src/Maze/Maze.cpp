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


// ---- 定数

// 0〜3ビット目は壁
const int   kCell_TopBorder    = 0x01;
const int   kCell_RightBorder  = 0x02;
const int   kCell_BottomBorder = 0x04;
const int   kCell_LeftBorder   = 0x08;
const int   kCell_AllBorders   = 0x0f;


// ---- CellPoint構造体

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


// ---- CrossPoint構造体

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


// ---- Wall構造体

Wall::Wall(const CellPoint& _pos, Direction _dir)
    : pos(_pos), dir(_dir)
{
    // Do nothing
}

Wall::Wall(const Wall& wall)
    : pos(wall.pos), dir(wall.dir)
{
    // Do nothing
}


// ---- Mazeクラスのコンストラクタ・デストラクタ

Maze::Maze(int _xSize, int _ySize)
{
    Create(_xSize, _ySize, 0);
}

Maze::Maze(int _xSize, int _ySize, unsigned _data)
{
    Create(_xSize, _ySize, _data);
}

Maze::~Maze()
{
    delete[] data;
}

void Maze::Create(int _xSize, int _ySize, unsigned _data)
{
    assert(_xSize >= 2);
    assert(_ySize >= 2);

    xSize = _xSize;
    ySize = _ySize;
    data = new unsigned[xSize * ySize];

    for (int i = 0; i < xSize * ySize; i++) {
        data[i] = _data;
    }
}


// ---- MazeクラスのGetter系の関数

int Maze::GetXSize() const
{
    return xSize;
}

int Maze::GetYSize() const
{
    return ySize;
}


// ---- Mazeクラスの迷路生成用の関数（タグ操作）

int Maze::GetCellTag(int x, int y) const
{
    assert(x >= 0 && x < xSize);
    assert(y >= 0 && y < ySize);
    return (data[y * xSize + x] >> 4);
}

int Maze::GetCellTag(const CellPoint& pos) const
{
    assert(pos.x >= 0 && pos.x < xSize);
    assert(pos.y >= 0 && pos.y < ySize);
    return (data[pos.y * xSize + pos.x] >> 4);
}

void Maze::SetCellTag(int x, int y, int tag)
{
    assert(x >= 0 && x < xSize);
    assert(y >= 0 && y < ySize);
    assert(tag >= 0 && tag <= 65535);
    data[y * xSize + x] &= kCell_AllBorders;
    data[y * xSize + x] |= (tag << 4);
}

void Maze::SetCellTag(const CellPoint& pos, int tag)
{
    assert(pos.x >= 0 && pos.x < xSize);
    assert(pos.y >= 0 && pos.y < ySize);
    assert(tag >= 0 && tag <= 65535);
    data[pos.y * xSize + pos.x] &= kCell_AllBorders;
    data[pos.y * xSize + pos.x] |= (tag << 4);
}

void Maze::SetTagForAllCells(int tag)
{
    assert(tag >= 0 && tag <= 65535);
    for (int y = 0; y < ySize; y++) {
        for (int x = 0; x < xSize; x++) {
            SetCellTag(x, y, tag);
        }
    }
}


// ---- Mazeクラスの迷路生成用の関数（セルベース）

bool Maze::CanMove(const CellPoint& pos, Direction dir) const
{
    CellPoint nextPos = pos.Move(dir);
    return IsValidCell(nextPos);
}

bool Maze::CanRemoveWall(int x, int y, Direction dir) const
{
    return (IsValidCell(CellPoint(x, y)) && CheckWall(x, y, dir));
}

bool Maze::CanRemoveWall(const CellPoint& pos, Direction dir) const
{
    return CanRemoveWall(pos.x, pos.y, dir);
}

bool Maze::CanRemoveWall(const Wall& wall) const
{
    return CanRemoveWall(wall.pos.x, wall.pos.y, wall.dir);
}

bool Maze::CheckWall(int x, int y, Direction dir) const
{
    assert(x >= 0 && x < xSize);
    assert(y >= 0 && y < ySize);

    if (dir == Up) {
        return ((data[y * xSize + x] & kCell_TopBorder) > 0);
    } else if (dir == Right) {
        return ((data[y * xSize + x] & kCell_RightBorder) > 0);
    } else if (dir == Down) {
        return ((data[y * xSize + x] & kCell_BottomBorder) > 0);
    } else {
        return ((data[y * xSize + x] & kCell_LeftBorder) > 0);
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

bool Maze::IsValidCell(const CellPoint& pos) const
{
    return (pos.x >= 0 && pos.x < xSize && pos.y >= 0 && pos.y < ySize);
}

vector<Direction> Maze::MakeValidMoveDirectionList(const CellPoint& pos) const
{
    vector<Direction> dirs = MakeAllDirectionsList();
    vector<Direction> ret;
    for (int i = 0; i < 4; i++) {
        if (CanMove(pos, dirs[i])) {
            ret.push_back(dirs[i]);
        }
    }
    return ret;
}

vector<Direction> Maze::MakeValidMoveDirectionList_shuffled(const CellPoint& pos) const
{
    vector<Direction> dirs = MakeValidMoveDirectionList(pos);
    random_shuffle(dirs.begin(), dirs.end());
    return dirs;
}

void Maze::MakeWall(int x, int y, Direction dir)
{
    assert(x >= 0 && x <= xSize - 1);
    assert(y >= 0 && y <= ySize - 1);

    if (dir == Up) {
        AddCellFlag(x, y, kCell_TopBorder);
        int ny = y - 1;
        if (ny >= 0) {
            AddCellFlag(x, ny, kCell_BottomBorder);
        }
    } else if (dir == Right) {
        AddCellFlag(x, y, kCell_RightBorder);
        int nx = x + 1;
        if (nx < xSize) {
            AddCellFlag(nx, y, kCell_LeftBorder);
        }
    } else if (dir == Down) {
        AddCellFlag(x, y, kCell_BottomBorder);
        int ny = y + 1;
        if (ny < ySize) {
            AddCellFlag(x, ny, kCell_TopBorder);
        }
    } else {
        AddCellFlag(x, y, kCell_LeftBorder);
        int nx = x - 1;
        if (nx >= 0) {
            AddCellFlag(nx, y, kCell_RightBorder);
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

void Maze::RemoveWall(int x, int y, Direction dir)
{
    assert(x >= 0 && x <= xSize - 1);
    assert(y >= 0 && y <= ySize - 1);

    if (dir == Up) {
        RemoveCellFlag(x, y, kCell_TopBorder);
        int ny = y - 1;
        if (ny >= 0) {
            RemoveCellFlag(x, ny, kCell_BottomBorder);
        }
    } else if (dir == Right) {
        RemoveCellFlag(x, y, kCell_RightBorder);
        int nx = x + 1;
        if (nx < xSize) {
            RemoveCellFlag(nx, y, kCell_LeftBorder);
        }
    } else if (dir == Down) {
        RemoveCellFlag(x, y, kCell_BottomBorder);
        int ny = y + 1;
        if (ny < ySize) {
            RemoveCellFlag(x, ny, kCell_TopBorder);
        }
    } else {
        RemoveCellFlag(x, y, kCell_LeftBorder);
        int nx = x - 1;
        if (nx >= 0) {
            RemoveCellFlag(nx, y, kCell_RightBorder);
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


// ---- Mazeクラスの迷路生成用の関数（交点ベース）

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


// ---- Mazeクラスの迷路の描画

void Maze::Draw(bool usesBatch)
{
    DrawMaze(this, usesBatch);
}

void Maze::DrawCrossPoint(const CrossPoint& pos, bool usesBatch)
{
    ::DrawCrossPoint(this, pos, usesBatch);
}


// ---- Mazeクラスの迷路生成用の関数（ビットフラグの操作）

void Maze::AddCellFlag(int x, int y, unsigned flag)
{
    assert(x >= 0 && x <= xSize - 1);
    assert(y >= 0 && y <= ySize - 1);
    data[y * xSize + x] |= flag;
}

void Maze::AddCellFlag(const CellPoint& pos, unsigned flag)
{
    assert(pos.x >= 0 && pos.x <= xSize - 1);
    assert(pos.y >= 0 && pos.y <= ySize - 1);
    data[pos.y * xSize + pos.x] |= flag;
}

void Maze::RemoveCellFlag(int x, int y, unsigned flag)
{
    assert(x >= 0 && x <= xSize - 1);
    assert(y >= 0 && y <= ySize - 1);
    data[y * xSize + x] &= ~flag;
}

void Maze::RemoveCellFlag(const CellPoint& pos, unsigned flag)
{
    assert(pos.x >= 0 && pos.x <= xSize - 1);
    assert(pos.y >= 0 && pos.y <= ySize - 1);
    data[pos.y * xSize + pos.x] &= ~flag;
}


// ---- 迷路生成用のヘルパー関数

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

