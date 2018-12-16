//
//  Maze_GrowingTree.cpp
//
//  Created by Satoshi Numata on 2018/12/16.
//  License: Public domain.
//

#include "Maze_GrowingTree.hpp"


/// Growing Treeのアルゴリズムによる迷路生成
Maze *CreateMaze_GrowingTree(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->SetTagForAllCells(1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 最初のセルをランダムに選択してヒストリに加える
    vector<CellPoint> history;
    CellPoint cell;
    cell.x = random() % maze->GetXSize();
    cell.y = random() % maze->GetYSize();
    history.push_back(cell);

    // ヒストリが取り出せる間、迷路生成を行う
    while (history.size() > 0) {
        // ヒストリの最後のセルを取り出す
        CellPoint cell = *(history.end() - 1);
        maze->SetCellTag(cell, 2);
        maze->Draw();

        // 周りのセルをランダムに選び、訪れていない場合はそこに向かって壁を取り除いて移動する
        bool hasMoved = false;
        vector<Direction> dirs = MakeAllDirectionsList_shuffled();
        for (int i = 0; i < 4; i++) {
            CellPoint nextCell = cell.Move(dirs[i]);
            if (maze->IsValidCell(nextCell) && maze->GetCellTag(nextCell) == 1) {
                maze->SetCellTag(nextCell, 3);
                maze->RemoveWall(cell, dirs[i]);
                maze->Draw();
                history.push_back(nextCell);
                hasMoved = true;
                break;
            }
        }

        // 周りに進めないセルは完了したものとしてヒストリから取り除く
        if (!hasMoved) {
            maze->SetCellTag(cell, 0);
            maze->Draw();
            history.erase(history.end() - 1);
        }
    }

    // 終了
    return maze;
}

