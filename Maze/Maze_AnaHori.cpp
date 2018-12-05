//
//  Maze_AnaHori.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "Maze_AnaHori.hpp"


// マーカー1: まだ通路を掘っていない（←これだけが掘り進められる）
// マーカー2: 掘っている最中の通路
// マーカー3: 掘り終わった通路（←これだけが起点に選択可能）
// マーカー4: 彫り終わった通路で、周囲に空きがない


/// 穴を掘る最大の長さ。長く穴を掘っていると単調になるので、このパラメータで調整します。
static const int kMaxDigCount   = 20;


// 穴掘り法の実装
Maze *CreateMaze_AnaHori(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kBlock_AllBorders | kBlock_CreateMarker1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 最初の点をランダムに選ぶ
    Point p;
    p.x = random() % maze->GetXSize();
    p.y = random() % maze->GetYSize();
    maze->RemoveFlag(p, kBlock_CreateMarker1);
    maze->AddFlag(p, kBlock_CreateMarker2);
    maze->Draw();

    while (true) {
        int digCount = 0;
        while (digCount < kMaxDigCount) {
            // 穴を掘る方向をランダムに選択
            Direction dir = (Direction)(random() % 4);
            // 四方に掘り進める
            bool hasDug = false;
            for (int i = 0; i < 4; i++) {
                Point nextPos = p.Move(dir);
                if (maze->CheckFlag(nextPos, kBlock_CreateMarker1) && maze->CanRemoveWall(p, dir)) {
                    maze->RemoveWall(p, dir);
                    p = nextPos;
                    maze->RemoveFlag(p, kBlock_CreateMarker1);
                    maze->AddFlag(p, kBlock_CreateMarker2);
                    maze->Draw();
                    hasDug = true;
                    break;
                }
                // 移動できなければ回転してみる
                dir = RotateRight(dir);
            }
            if (!hasDug) {
                maze->RemoveFlag(p, kBlock_AllCreateMarkers);
                maze->AddFlag(p, kBlock_CreateMarker4);
                maze->Draw();
                break;
            }
            digCount++;
        }

        // 作成が完了した通路のマーカーを3に
        for (int y = 0; y < maze->GetYSize(); y++) {
            for (int x = 0; x < maze->GetXSize(); x++) {
                if (maze->CheckFlag(x, y, kBlock_CreateMarker2)) {
                    maze->RemoveFlag(x, y, kBlock_CreateMarker2);
                    maze->AddFlag(x, y, kBlock_CreateMarker3);
                }
            }
        }
        maze->Draw();

        // 終了判定
        bool isFinished = true;
        for (int y = 0; y < maze->GetYSize(); y++) {
            for (int x = 0; x < maze->GetXSize(); x++) {
                if (maze->CheckFlag(x, y, kBlock_CreateMarker1 | kBlock_CreateMarker2)) {
                    isFinished = false;
                    break;
                }
            }
        }
        if (isFinished) {
            break;
        }

        // すでに掘った座標をランダムに選ぶ
        vector<Point> nextPoints;
        for (int y = 0; y < maze->GetYSize(); y++) {
            for (int x = 0; x < maze->GetXSize(); x++) {
                if (maze->CheckFlag(x, y, kBlock_CreateMarker3)) {
                    nextPoints.push_back(Point(x, y));
                }
            }
        }
        int r = (int)(random() % nextPoints.size());
        p = nextPoints[r];
    }

    // 生成用のフラグをクリアする
    Sleep(0.8f);
    for (int y = 0; y < maze->GetYSize(); y++) {
        for (int x = 0; x < maze->GetXSize(); x++) {
            maze->RemoveFlag(x, y, kBlock_AllCreateMarkers);
        }
        maze->Draw();
    }

    // 終了
    return maze;
}

