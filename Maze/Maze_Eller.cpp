//
//  Maze_Eller.cpp
//
//  Created by Satoshi Numata on 2018/12/08.
//  License: Public domain.
//

#include "Maze_Eller.hpp"


// Ellerのアルゴリズムによる迷路生成
Maze *CreateMaze_Eller(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 終了
    return maze;
}

