//
//  Maze_BinaryTree.cpp
//
//  Created by Satoshi Numata on 2018/12/15.
//  License: Public domain.
//

#include "Maze_BinaryTree.hpp"


// Binary Treeのアルゴリズムによる迷路生成
Maze *CreateMaze_BinaryTree(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->SetTagForAllCells(1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // メインルーチン
    // 左上から始めた場合、左か上の壁をランダムに選んで取り除いていく。取り除ける壁がなければ何もしない。
    CellPoint pos;
    for (pos.y = 0; pos.y < maze->GetYSize(); pos.y++) {
        for (pos.x = 0; pos.x < maze->GetXSize(); pos.x++) {
            vector<Direction> dirs = maze->MakeValidMoveDirectionList_shuffled(pos);
            for (int i = 0; i < dirs.size(); i++) {
                if (dirs[i] == Up || dirs[i] == Left) {
                    maze->RemoveWall(pos, dirs[i]);
                    break;
                }
            }
            maze->SetCellTag(pos, 0);
        }
        maze->Draw();
    }

    // 終了
    return maze;
}

