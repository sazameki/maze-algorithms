//
//  Maze.cpp
//
//  Created by numata on 2018/12/02.
//  Copyright © 2018 Satoshi Numata. All rights reserved.
//

#include "Maze.hpp"
#include "MazeDraw.hpp"

#include <algorithm>
#include <cassert>


// 0〜3ビット目は壁
const int   kBlock_TopBorder    = 0x01;
const int   kBlock_RightBorder  = 0x02;
const int   kBlock_BottomBorder = 0x04;
const int   kBlock_LeftBorder   = 0x08;
const int   kBlock_AllBorders   = 0x0f;

// 4〜6ビット目は生成用のマーカー
const int   kBlock_CreateMarker1    = 0x10;
const int   kBlock_CreateMarker2    = 0x20;
const int   kBlock_CreateMarker3    = 0x40;
const int   kBlock_AllCreateMarkers = 0x70;

// 7〜9ビット目は迷路を解くためのマーカー
const int   kBlock_SolveMarker1     = 0x80;
const int   kBlock_SolveMarker2     = 0x100;
const int   kBlock_SolveMarker3     = 0x200;
const int   kBlock_AllSolveMarkers  = 0x380;


#pragma mark - Point構造体

Point::Point()
    : x(0), y(0)
{
    // Do nothing
}

Point::Point(int _x, int _y)
    : x(_x), y(_y)
{
    // Do nothing
}

Point::Point(const Point& p)
    : x(p.x), y(p.y)
{
    // Do nothing
}

Point Point::Move(int dir) const
{
    Point ret(*this);
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

Wall::Wall(const Point& _pos, Direction _dir)
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

int Maze::GetData(int x, int y) const
{
    return data[y * xSize + x];
}

int Maze::GetData(const Point& pos) const
{
    return data[pos.y * xSize + pos.x];
}

bool Maze::CheckFlag(int x, int y, int flag) const
{
    return ((data[y * xSize + x] & flag) > 0);
}

bool Maze::CheckFlag(const Point& pos, int flag) const
{
    return ((data[pos.y * xSize + pos.x] & flag) > 0);
}

bool Maze::IsValid(const Point& pos) const
{
    return (pos.x >= 0 && pos.x < xSize && pos.y >= 0 && pos.y < ySize);
}


#pragma mark - Mazeクラスの迷路生成用の関数

void Maze::AddFlag(int x, int y, int flag)
{
    data[y * xSize + x] |= flag;
}

void Maze::AddFlag(const Point& pos, int flag)
{
    data[pos.y * xSize + pos.x] |= flag;
}

void Maze::RemoveFlag(int x, int y, int flag)
{
    data[y * xSize + x] &= ~flag;
}

void Maze::RemoveFlag(const Point& pos, int flag)
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

bool Maze::CheckWall(const Point& pos, Direction dir) const
{
    return CheckWall(pos.x, pos.y, dir);
}

bool Maze::CheckWall(const Wall& wall) const
{
    return CheckWall(wall.pos.x, wall.pos.y, wall.dir);
}

void Maze::MakeWall(int x, int y, Direction dir)
{
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

void Maze::MakeWall(const Point& pos, Direction dir)
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

bool Maze::CanRemoveWall(const Point& pos, Direction dir) const
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
        RemoveFlag(x, y, kBlock_TopBorder);
        int ny = y - 1;
        if (ny >= 0) {
            RemoveFlag(x, ny, kBlock_BottomBorder);
        }
    } else if (dir == Right) {
        RemoveFlag(x, y, kBlock_RightBorder);
        int nx = x + 1;
        if (nx < xSize) {
            RemoveFlag(nx, y, kBlock_LeftBorder);
        }
    } else if (dir == Down) {
        RemoveFlag(x, y, kBlock_BottomBorder);
        int ny = y + 1;
        if (ny < ySize) {
            RemoveFlag(x, ny, kBlock_TopBorder);
        }
    } else {
        RemoveFlag(x, y, kBlock_LeftBorder);
        int nx = x - 1;
        if (nx >= 0) {
            RemoveFlag(nx, y, kBlock_RightBorder);
        }
    }
}

void Maze::RemoveWall(const Point& pos, Direction dir)
{
    RemoveWall(pos.x, pos.y, dir);
}

void Maze::RemoveWall(const Wall& wall)
{
    RemoveWall(wall.pos.x, wall.pos.y, wall.dir);
}


#pragma mark - Mazeクラスの迷路の描画

void Maze::Draw(bool usesBatch)
{
    DrawMaze(this, usesBatch);
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

