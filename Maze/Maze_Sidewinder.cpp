//
//  Maze_Sidewinder.cpp
//
//  Created by Satoshi Numata on 2018/12/08.
//  License: Public domain.
//

#include "Maze_Sidewinder.hpp"


/// Sidewinderのアルゴリズムによる迷路生成
Maze *CreateMaze_Sidewinder(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->SetTagForAllCells(1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 最初の行はすべての右壁を取り除く
    for (int x = 0; x < maze->GetXSize() - 1; x++) {
        maze->SetCellTag(x, 0, 0);
        maze->RemoveWall(x, 0, Right);
        maze->Draw();
    }
    maze->SetCellTag(maze->GetXSize() - 1, 0, 0);
    maze->Draw();

    // 2行目以降の処理（右の壁か上の壁を取り除く）
    for (int y = 1; y < maze->GetYSize(); y++) {
        int runStart = 0;
        for (int x = 0; x < maze->GetXSize(); x++) {
            maze->SetCellTag(x, y, 0);
            // 右端でなければ、1/2の確率で右の壁を取り除く
            if (x < maze->GetXSize() - 1 && random() % 2 == 0) {
                maze->RemoveWall(x, y, Right);
            }
            // そうでなければ、上の壁を取り除く（以前に上の壁を取り除いたセルの右から現在のセルまでの間でランダムに選ぶ）
            else {
                int cellX = runStart + random() % (x - runStart + 1);
                maze->RemoveWall(cellX, y, Up);
                runStart = x + 1;
            }
            maze->Draw();
        }
    }

    // 終了
    return maze;
}

