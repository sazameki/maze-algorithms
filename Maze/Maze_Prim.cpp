//
//  Maze_Prim.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "Maze_Prim.hpp"

/// Primのアルゴリズムによる迷路生成
Maze *CreateMaze_Prim(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kBlock_AllBorders);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // ランダムに1点選択する
    Point p;
    p.x = random() % maze->GetXSize();
    p.y = random() % maze->GetYSize();
    maze->AddFlag(p, kBlock_CreateMarker1);
    maze->Draw();

    // 上下左右のマスを壁リストに登録する
    vector<Wall> walls;
    Direction dir = Up;
    for (int i = 0; i < 4; i++) {
        Point p2 = p.Move(dir);
        if (maze->IsValid(p2)) {
            // 壁であることを示すマーカーを追加
            maze->AddFlag(p2, kBlock_CreateMarker2);
            walls.push_back(Wall(p, dir));
            maze->Draw();
        }
        dir = RotateRight(dir);
    }
    maze->Draw();

    // 壁リストに登録された壁を取り除いていく
    while (walls.size() > 0) {
        int index = (int)(random() % walls.size());
        Wall wall = walls[index];
        walls.erase(walls.begin() + index);
        Point p2 = wall.pos.Move(wall.dir);

        // すでに処理済みのマスでないことをチェック
        if (!maze->CheckFlag(p2, kBlock_CreateMarker1)) {
            // 壁であることを示すマーカーを取り除く
            maze->RemoveFlag(p2, kBlock_CreateMarker2);
            // 処理済みであることを示すマーカー
            maze->AddFlag(p2, kBlock_CreateMarker1);
            maze->Draw();

            // 四方の壁に対して同様の処理を繰り返す
            Direction dir = Up;
            for (int i = 0; i < 4; i++) {
                Point p3 = p2.Move(dir);
                if (maze->IsValid(p3)) {
                    maze->AddFlag(p3, kBlock_CreateMarker2);
                    walls.push_back(Wall(p2, dir));
                    maze->Draw();
                }
                dir = RotateRight(dir);
            }

            // 壁を取り除く処理
            maze->RemoveWall(wall);
            maze->Draw();
        }
    }

    // 生成用のフラグをクリアする
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            maze->RemoveFlag(x, y, kBlock_AllCreateMarkers);
            maze->Draw();
        }
    }

    return maze;
}

