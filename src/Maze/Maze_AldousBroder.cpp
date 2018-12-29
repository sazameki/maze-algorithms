//
//  Maze_AldousBroder.cpp
//
//  Created by Satoshi Numata on 2018/12/21.
//  License: Public domain.
//

#include "Maze_AldousBroder.hpp"


// Aldous-Broderのアルゴリズムによる迷路生成の実装
void AldousBroder_Impl(Maze *maze, int cellCountThreshold)
{
    // 処理したセルのカウント
    int cellCount = 0;

    // 最初のセルをランダムに選択する
    CellPoint cell;
    cell.x = random() % maze->GetXSize();
    cell.y = random() % maze->GetYSize();
    cellCount++;
    maze->SetCellTag(cell, 2);
    maze->Draw();

    // すべてのセルを通過するまで繰り返す
    while (cellCount < cellCountThreshold) {
        // ランダムに方向を選ぶ
        vector<Direction> dirs = MakeAllDirectionsList_shuffled();
        for (Direction dir : dirs) {
            // 移動できれば必ず移動する
            CellPoint nextCell = cell.Move(dir);
            if (maze->IsValidCell(nextCell)) {
                // まだ訪れていないセルであれば壁を取り除く
                if (maze->GetCellTag(nextCell) == 1) {
                    cellCount++;
                    maze->RemoveWall(cell, dir);
                }
                // 移動する
                maze->SetCellTag(cell, 0);
                maze->SetCellTag(nextCell, 2);
                cell = nextCell;
                maze->Draw();
                Sleep(0.01f);
                break;
            }
        }
    }

    // クリーンアップ
    maze->SetCellTag(cell, 0);
    maze->Draw();
}


// Aldous-Broderのアルゴリズムによる迷路生成
Maze *CreateMaze_AldousBroder(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->SetTagForAllCells(1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 迷路生成の実装
    AldousBroder_Impl(maze, maze->GetXSize() * maze->GetYSize());

    // 終了
    return maze;
}

