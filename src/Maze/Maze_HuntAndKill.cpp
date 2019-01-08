//
//  Maze_HuntAndKill.cpp
//
//  Created by Satoshi Numata on 2018/12/16.
//  License: Public domain.
//

#include "Maze_HuntAndKill.hpp"


// Hunt and Killのアルゴリズムによる迷路生成
Maze *CreateMaze_HuntAndKill(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, kCell_AllBorders);
    maze->SetTagForAllCells(1);
    maze->Draw();

    // 開始のためのキー入力待ち
    while (!CheckKey(kKeySpace)) {
        DrawText("SPACE KEY TO CREATE!!", -12*10.5, -240, kColorRed);
    }

    // 最初のセルをランダムに選択する（最初の狩猟。必ず成功する）
    bool huntSucceeded = true;
    CellPoint cell;
    cell.x = random() % maze->GetXSize();
    cell.y = random() % maze->GetYSize();
    maze->SetCellTag(cell, 0);
    maze->Draw();

    // 狩猟が成功する間、移動を続ける
    while (huntSucceeded) {
        // 動けなくなるまでランダムに移動する
        bool hasMoved = false;
        vector<Direction> dirs = MakeAllDirectionsList_shuffled();
        for (int i = 0; i < 4; i++) {
            CellPoint nextCell = cell.Move(dirs[i]);
            if (maze->IsValidCell(nextCell) && maze->GetCellTag(nextCell) == 1) {
                maze->SetCellTag(nextCell, 0);
                maze->RemoveWall(cell, dirs[i]);
                maze->Draw();
                cell = nextCell;
                hasMoved = true;
                break;
            }
        }

        // 動けなくなったら狩猟モードに移行する
        if (!hasMoved) {
            huntSucceeded = false;
            CellPoint huntCell;
            vector<Direction> dirs = MakeAllDirectionsList();

            // 狩猟は左上から右下に向けて行う
            for (huntCell.y = 0; huntCell.y < maze->GetYSize(); huntCell.y++) {
                for (huntCell.x = 0; huntCell.x < maze->GetXSize(); huntCell.x++) {
                    int tag = maze->GetCellTag(huntCell);
                    if (tag == 0) {
                        continue;
                    }
                    maze->SetCellTag(huntCell, 2);
                    // 近接セルを探索
                    for (int i = 0; i < 4; i++) {
                        // 探索済みの近接セルが見つかったら、そのセルとの間に通路を空けて、狩猟を終了
                        CellPoint neighbor = huntCell.Move(dirs[i]);
                        if (maze->IsValidCell(neighbor) && maze->GetCellTag(neighbor) == 0) {
                            // 次の探索セルは狩猟で見つかったセルとなる
                            maze->RemoveWall(huntCell, dirs[i]);
                            maze->SetCellTag(huntCell, 0);
                            cell = huntCell;
                            huntSucceeded = true;
                            break;
                        }
                    }
                    if (huntSucceeded) {
                        break;
                    }
                    maze->SetCellTag(huntCell, tag);
                }
                if (huntSucceeded) {
                    break;
                }
            }
            maze->Draw();
        }
    }

    // 終了
    return maze;
}

