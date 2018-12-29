//
//  Maze_KabeNobashi.cpp
//
//  Created by Satoshi Numata on 2018/12/06.
//  License: Public domain.
//

#include "Maze_KabeNobashi.hpp"


/// 壁を延ばす最大の長さ。長く延ばしていると単調になるので、このパラメータで調整します。
static const int kMaxExtendCount    = 30;


/**
    迷路の生成が完了しているかどうかをチェックする。
    壁のない交点があれば終了していない。
 */
static bool CheckFinished(Maze *maze)
{
    vector<Direction> dirs = MakeAllDirectionsList();
    for (int cy = 0; cy <= maze->GetYSize(); cy++) {
        for (int cx = 0; cx <= maze->GetXSize(); cx++) {
            CrossPoint cp(cx, cy);
            if (!maze->CheckWallFromCrossPoint(cp)) {
                return false;
            }
        }
    }
    return true;
}

// マスとマスの間の点が壁を作る起点の座標として考えられる。
// そこから上下左右に壁が作れる。
Maze *CreateMaze_KabeNobashi(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize);
    for (int y = 0; y < maze->GetYSize(); y++) {
        maze->MakeWall(0, y, Left);
        maze->MakeWall(maze->GetXSize()-1, y, Right);
    }
    for (int x = 0; x < maze->GetXSize(); x++) {
        maze->MakeWall(x, 0, Up);
        maze->MakeWall(x, maze->GetYSize()-1, Down);
    }
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // スタート
    while (!CheckFinished(maze)) {
        // 壁を延ばし始める点をランダムに選択する（壁がある場所のみ） (0 <= x <= xSize, 0 <= y <= ySize)
        CrossPoint cp;
        do {
            cp.x = random() % (maze->GetXSize() + 1);
            cp.y = random() % (maze->GetYSize() + 1);
        } while (!maze->CheckWallFromCrossPoint(cp));

        // 壁を延ばす
        int extendCount = 0;
        bool isExtending = true;
        while (isExtending && extendCount < kMaxExtendCount) {
            vector<Direction> dirs = MakeAllDirectionsList_shuffled();
            isExtending = false;
            for (int i = 0; i < 4; i++) {
                if (maze->CanMoveFromCrossPoint(cp, dirs[i])) {
                    CrossPoint cp2 = cp.Move(dirs[i]);
                    if (!maze->CheckWallFromCrossPoint(cp2)) {
                        maze->DrawCrossPoint(cp);
                        Sleep(0.01f);

                        maze->MakeWallFromCrossPoint(cp, dirs[i]);
                        maze->Draw();
                        cp = cp2;
                        maze->DrawCrossPoint(cp);
                        extendCount++;
                        isExtending = true;
                        break;
                    }
                }
            }
        }
    }

    // 交点の描画を消すために再描画
    maze->Draw();

    // 終了
    return maze;
}

