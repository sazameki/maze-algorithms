//
//  Maze_Blobby.cpp
//
//  Created by numata on 2018/12/04.
//  Copyright © 2018 Satoshi Numata. All rights reserved.
//

#include "Maze_Blobby.hpp"
#include <cstdio>
#include <stack>
#include <vector>

using namespace std;


enum BlobbyState
{
    BlobbyState_Start,
    BlobbyState_Plant,
    BlobbyState_Grow,
    BlobbyState_Wall,
};

struct BlobbyRegion
{
    vector<Point>   cells;

    void AddCell(const Point& cell)
    {
        cells.push_back(cell);
    }

    void Clear()
    {
        cells.clear();
    }
};


static const int    kThreshold  = 4;
static const int    kGrowSpeed  = 5;
static const int    kWallSpeed  = 2;


static BlobbyState              state;
static stack<BlobbyRegion *>    regionStack;
static BlobbyRegion             *currentRegion;
static vector<Point>            frontierCells;
static BlobbyRegion             subregionA;
static BlobbyRegion             subregionB;


bool StartRegion(Maze *maze)
{
    //delete @boundary
    if (regionStack.size() == 0) {
        return false;
    }

    currentRegion = regionStack.top();
    regionStack.pop();
    //delete cell.state for cell in @region.cells
    state = BlobbyState_Plant;
    return true;
}

void PlantSeed(Maze *maze)
{
    subregionA.Clear();
    subregionB.Clear();

    int aIndex, bIndex;
    do {
        aIndex = (int)(random() % currentRegion->cells.size());
        bIndex = (int)(random() % currentRegion->cells.size());
    } while (aIndex == bIndex);

    Point cellA = currentRegion->cells[aIndex];
    Point cellB = currentRegion->cells[bIndex];
    maze->RemoveFlag(cellA, kBlock_AllCreateMarkers);
    maze->RemoveFlag(cellB, kBlock_AllCreateMarkers);
    maze->AddFlag(cellA, kBlock_CreateMarker2);
    maze->AddFlag(cellB, kBlock_CreateMarker3);
    maze->Draw();

    subregionA.AddCell(cellA);
    subregionB.AddCell(cellB);

    frontierCells.clear();
    frontierCells.push_back(cellA);
    frontierCells.push_back(cellB);

    state = BlobbyState_Grow;
}

void GrowSeeds(Maze *maze)
{
    int growCount = 0;
    while (frontierCells.size() > 0 && growCount < kGrowSpeed) {
        int index = (int)(random() % frontierCells.size());
        Point cell = frontierCells[index];

        Point up = cell.Move(Up);
        Point right = cell.Move(Right);
        Point down = cell.Move(Down);
        Point left = cell.Move(Left);

        vector<Point> neighborList;
        if (maze->IsValid(up) && !maze->CheckFlag(up, kBlock_CreateMarker2 | kBlock_CreateMarker3)) {
            neighborList.push_back(up);
        }
        if (maze->IsValid(right) && !maze->CheckFlag(right, kBlock_CreateMarker2 | kBlock_CreateMarker3)) {
            neighborList.push_back(right);
        }
        if (maze->IsValid(down) && !maze->CheckFlag(down, kBlock_CreateMarker2 | kBlock_CreateMarker3)) {
            neighborList.push_back(down);
        }
        if (maze->IsValid(left) && !maze->CheckFlag(left, kBlock_CreateMarker2 | kBlock_CreateMarker3)) {
            neighborList.push_back(left);
        }

        if (neighborList.size() > 0) {
            int neighborIndex = (int)(random() % neighborList.size());
            Point neighbor = neighborList[neighborIndex];
            int cellFlag = maze->GetData(cell) & kBlock_AllCreateMarkers;
            maze->RemoveFlag(neighbor, kBlock_AllCreateMarkers);
            maze->AddFlag(neighbor, cellFlag);
            maze->Draw();
            if (cellFlag & kBlock_CreateMarker2) {
                subregionA.AddCell(neighbor);
            } else {
                subregionB.AddCell(neighbor);
            }
            frontierCells.push_back(neighbor);
            growCount++;
        } else {
            frontierCells.erase(frontierCells.begin() + index);
        }
    }

    if (frontierCells.size() == 0) {
        state = BlobbyState_Wall;
    }
}

void DrawWall()
{
    state = BlobbyState_Start;
}

Maze *CreateMaze_Blobby(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize);
    for (int y = 0; y < maze->GetYSize(); y++) {
        maze->MakeWall(0, y, Left);
        maze->MakeWall(maze->GetXSize()-1, y, Right);
    }
    for (int x = 0; x < maze->GetXSize(); x++) {
        maze->MakeWall(x, 0, Up);
        maze->MakeWall(x, maze->GetYSize()-1, Down);
    }
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            maze->AddFlag(x, y, kBlock_CreateMarker1);
        }
    }
    maze->Draw();

    // 開始のためのキー入力待ち
    /*while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }*/

    // 最初のリージョンの追加
    BlobbyRegion *region = new BlobbyRegion();
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            region->AddCell(Point(x, y));
        }
    }
    regionStack.push(region);

    // メインのループ
    state = BlobbyState_Start;
    bool isRunning = true;
    while (isRunning) {
        switch (state) {
            case BlobbyState_Start:
                isRunning = StartRegion(maze);
                break;
            case BlobbyState_Plant:
                PlantSeed(maze);
                break;
            case BlobbyState_Grow:
                GrowSeeds(maze);
                break;
            case BlobbyState_Wall:
                DrawWall();
                break;
            default:
                printf("Unknown Blobby State: %d\n", state);
                exit(1);
        }
    }

    delete region;

    // 生成用のフラグをクリアする
    /*for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            maze->RemoveFlag(x, y, kBlock_AllCreateMarkers);
            maze->Draw();
        }
    }*/

    return maze;
}

