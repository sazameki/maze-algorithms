//
//  Maze_AnaHori.cpp
//
//  Created by numata on 2018/12/02.
//  Copyright © 2018 Satoshi Numata. All rights reserved.
//

#include "Maze_AnaHori.hpp"


Maze *CreateMaze_AnaHori(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kBlock_AllBorders);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 最初の点をランダムに選ぶ
    Point p;
    p.x = random() % maze->GetXSize();
    p.y = random() % maze->GetYSize();
    maze->AddFlag(p, kBlock_CreateMarker1);
    maze->Draw();

    while (true) {
        while (true) {
            // 穴を掘る方向をランダムに選択
            Direction dir = (Direction)(random() % 4);
            // 四方に掘り進める
            bool hasDug = false;
            for (int i = 0; i < 4; i++) {
                Point nextPos = p.Move(dir);
                if (!maze->CheckFlag(nextPos, kBlock_CreateMarker1) && maze->CanRemoveWall(p, dir)) {
                    maze->RemoveWall(p, dir);
                    p = nextPos;
                    maze->AddFlag(p, kBlock_CreateMarker1);
                    maze->Draw();
                    hasDug = true;
                    break;
                }
                // 移動できなければ回転してみる
                dir = RotateRight(dir);
            }
            if (!hasDug) {
                break;
            }
        }

        // 終了判定
        bool isFinished = true;
        for (int y = 0; y < maze->GetYSize(); y++) {
            for (int x = 0; x < maze->GetXSize(); x++) {
                if (!maze->CheckFlag(x, y, kBlock_CreateMarker1)) {
                    isFinished = false;
                    break;
                }
            }
        }
        if (isFinished) {
            break;
        }

        // すでに掘った座標をランダムに選ぶ
        do {
            p.x = random() % maze->GetXSize();
            p.y = random() % maze->GetYSize();
        } while (!maze->CheckFlag(p, kBlock_CreateMarker1));
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

