//
//  Maze_BouTaoshi.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "Maze_BouTaoshi.hpp"


// 棒倒し法の実装
Maze *CreateMaze_BouTaoshi(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize);
    for (int x = 0; x < maze->GetXSize(); x++) {
        maze->MakeWall(x, 0, Up);
        maze->MakeWall(x, maze->GetYSize() - 1, Down);
    }
    for (int y = 0; y < maze->GetYSize(); y++) {
        maze->MakeWall(0, y, Left);
        maze->MakeWall(maze->GetXSize() - 1, y, Right);
    }
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 最初の行
    for (int cx = 1; cx <= maze->GetXSize()-1; cx++) {
        // 各交点から上下左右いずれかの方向に壁を作る
        CrossPoint cp(cx, 1);
        vector<Direction> dirs = MakeAllDirectionsList_shuffled();
        for (int i = 0; i < 4; i++) {
            if (!maze->CheckWallFromCrossPoint(cp, dirs[i])) {
                maze->DrawCrossPoint(cp);
                Sleep(0.02f);
                maze->MakeWallFromCrossPoint(cp, dirs[i]);
                maze->Draw();
                maze->DrawCrossPoint(cp.Move(dirs[i]));
                Sleep(0.02f);
                break;
            }
        }
    }
    maze->Draw();

    // 2行目以降
    for (int cy = 2; cy <= maze->GetYSize()-1; cy++) {
        for (int cx = 1; cx <= maze->GetXSize()-1; cx++) {
            // 各交点から下左右いずれかの方向に壁を作る
            CrossPoint cp(cx, cy);
            vector<Direction> dirs = MakeAllDirectionsList_shuffled();
            for (int i = 0; i < 4; i++) {
                if (dirs[i] == Up) {
                    continue;
                }
                if (!maze->CheckWallFromCrossPoint(cp, dirs[i])) {
                    maze->DrawCrossPoint(cp);
                    Sleep(0.02f);
                    maze->MakeWallFromCrossPoint(cp, dirs[i]);
                    maze->Draw();
                    maze->DrawCrossPoint(cp.Move(dirs[i]));
                    Sleep(0.02f);
                    break;
                }
            }
        }
    }

    // 交点の表示を消すために再描画
    maze->Draw();

    // 終了
    return maze;
}

