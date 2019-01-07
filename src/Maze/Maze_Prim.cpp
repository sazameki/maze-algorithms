//
//  Maze_Prim.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "Maze_Prim.hpp"


// タグ1: 処理済み
// タグ2: チェック中の壁


/// Primのアルゴリズムによる迷路生成
Maze *CreateMaze_Prim(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->SetTagForAllCells(1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // ランダムに1点選択する
    CellPoint p;
    p.x = random() % maze->GetXSize();
    p.y = random() % maze->GetYSize();
    maze->SetCellTag(p, 0);
    maze->Draw();

    // 上下左右のマスを壁リストに登録する
    vector<Wall> walls;
    Direction dir = Up;
    for (int i = 0; i < 4; i++) {
        CellPoint p2 = p.Move(dir);
        if (maze->IsValidCell(p2)) {
            // 壁であることを示すタグを追加
            walls.push_back(Wall(p, dir));
            maze->SetCellTag(p2, 2);
        }
        dir = RotateRight(dir);
    }
    maze->Draw();

    // 壁リストに登録された壁を取り除いていく
    while (walls.size() > 0) {
        int index = (int)(random() % walls.size());
        Wall wall = walls[index];
        walls.erase(walls.begin() + index);
        CellPoint p2 = wall.pos.Move(wall.dir);
        maze->SetCellTag(wall.pos, 0);

        // すでに処理済みのマスでないことをチェック
        if (maze->GetCellTag(p2) != 0) {
            // 四方の壁に対して同様の処理を繰り返す
            Direction dir = Up;
            for (int i = 0; i < 4; i++) {
                CellPoint p3 = p2.Move(dir);
                if (maze->IsValidCell(p3) && maze->GetCellTag(p3) == 1) {
                    walls.push_back(Wall(p2, dir));
                    maze->SetCellTag(p3, 2);
                }
                dir = RotateRight(dir);
            }
            // 壁を取り除いて、処理済みにする
            maze->RemoveWall(wall);
            maze->SetCellTag(p2, 0);
        }
        maze->Draw();
    }

    // 終了
    return maze;
}

