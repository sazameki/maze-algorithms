//
//  Maze_Houston.cpp
//
//  Created by Satoshi Numata on 2018/12/30.
//  License: Public domain.
//

#include "Maze_Houston.hpp"
#include "Maze_AldousBroder.hpp"
#include "Maze_Wilson.hpp"


// Houstonの迷路生成アルゴリズム
Maze *CreateMaze_Houston(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->SetTagForAllCells(1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // まだ通過していないセルの数
    int cellCount = maze->GetXSize() * maze->GetYSize();
    int aldousBroderCount = cellCount / 5;
    int wilsonCount = cellCount - aldousBroderCount;

    // Aldous-Broderの迷路生成アルゴリズム
    AldousBroder_Impl(maze, aldousBroderCount);

    // Wilsonの迷路生成アルゴリズム
    Wilson_Impl(maze, wilsonCount, false);

    return maze;
}

