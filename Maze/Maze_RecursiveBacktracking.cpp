//
//  Maze_RecursiveBacktracking.cpp
//
//  Created by numata on 2018/12/02.
//  Copyright © 2018 Satoshi Numata. All rights reserved.
//

#include "Maze_RecursiveBacktracking.hpp"

#include <vector>


using namespace std;


/*!
    Recurvsive Backtrackingの再帰関数
 */
static void CarvePassage(Maze *maze, Point pos)
{
    // 方向をランダムに格納した配列を用意する
    vector<Direction> dirList = MakeAllDirectionsList_shuffled();

    // それぞれの方向に掘り進める
    for (auto dir : dirList) {
        Point nextPos = pos.Move(dir);
        if (maze->IsValid(nextPos) && !maze->CheckFlag(nextPos, kBlock_CreateMarker1)) {
            maze->RemoveWall(pos, dir);
            maze->AddFlag(pos, kBlock_CreateMarker1);
            maze->AddFlag(nextPos, kBlock_CreateMarker1);
            maze->Draw();
            CarvePassage(maze, nextPos);
        }
    }
}

Maze *CreateMaze_RecurvsiveBacktracking(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kBlock_AllBorders);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // ランダムな座標から彫っていく
    Point p;
    p.x = random() % maze->GetXSize();
    p.y = random() % maze->GetYSize();
    CarvePassage(maze, p);

    // 生成用のフラグをクリアする
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            maze->RemoveFlag(x, y, kBlock_AllCreateMarkers);
            maze->Draw();
        }
    }

    return maze;
}

