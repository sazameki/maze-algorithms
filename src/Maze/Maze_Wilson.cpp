//
//  Maze_Wilson.cpp
//
//  Created by Satoshi Numata on 2018/12/21.
//  License: Public domain.
//

#include "Maze_Wilson.hpp"


static Direction CheckMoveDirection(const CellPoint& from, const CellPoint& to)
{
    if (to.x > from.x) {
        return Right;
    } else if (to.x < from.x) {
        return Left;
    } else if (to.y > from.y) {
        return Down;
    } else {
        return Up;
    }
}

void Wilson_Impl(Maze *maze, int cellCountThreshold, bool opensFirstCell)
{
    // 処理したセルのカウント
    int cellCount = 0;

    // 最初のセルをランダムに選択して「処理済み」にする。
    CellPoint cell;
    if (opensFirstCell) {
        cell.x = random() % maze->GetXSize();
        cell.y = random() % maze->GetYSize();
        cellCount++;
        maze->SetCellTag(cell, 3);
        maze->Draw();
        Sleep(0.7f);
        maze->SetCellTag(cell, 0);
        maze->Draw();
    }

    // 「処理済み」でないセルが残っている間、処理を続ける
    while (cellCount < cellCountThreshold) {
        // 「処理済み」でないセルから開始点をランダムに選択する
        do {
            cell.x = random() % maze->GetXSize();
            cell.y = random() % maze->GetYSize();
        } while (maze->GetCellTag(cell) != 1);
        maze->SetCellTag(cell, 3);
        maze->Draw();

        // 移動のログ
        vector<CellPoint> visitedCells;
        visitedCells.push_back(cell);

        // 「処理済み」のセルにたどり着くまでランダムウォーク
        bool isWalking = true;
        while (isWalking) {
            vector<Direction> dirs = MakeAllDirectionsList_shuffled();
            CellPoint lastCell(-1, -1);
            if (visitedCells.size() >= 2) {
                lastCell = visitedCells[visitedCells.size() - 2];
            }
            for (Direction dir : dirs) {
                CellPoint cell2 = cell.Move(dir);
                if (maze->IsValidCell(cell2) && (lastCell.x != cell2.x || lastCell.y != cell2.y)) {
                    // 移動したセルのタグを確認する
                    int tag = maze->GetCellTag(cell2);

                    // タグが0なら「処理済み」
                    if (tag == 0) {
                        // ゴールにたどり着いたのでランダムウォーク終了
                        maze->SetCellTag(cell, 2);
                        maze->RemoveWall(cell, dir);
                        maze->Draw();
                        isWalking = false;
                    }
                    // タグが1なら「未処理」
                    else if (tag == 1) {
                        // 移動ログに追加して探索を続ける
                        maze->SetCellTag(cell, 2);
                        maze->RemoveWall(cell, dir);
                        maze->SetCellTag(cell2, 3);
                        maze->Draw();
                        visitedCells.push_back(cell2);
                        cell = cell2;
                    }
                    // タグがそれ以外なら探索中のセルに戻ってきた
                    else {
                        maze->SetCellTag(cell2, 8);
                        maze->Draw();
                        // 削除対象のセルをマーキングしながら壁を戻す
                        int index = (int)(visitedCells.size() - 1);
                        CellPoint lastCell = cell;
                        while (index >= 0) {
                            CellPoint delCell = visitedCells[index];
                            Direction dir2 = CheckMoveDirection(lastCell, delCell);
                            maze->MakeWall(lastCell, dir2);
                            maze->Draw();
                            if (delCell.x == cell2.x && delCell.y == cell2.y) {
                                break;
                            }
                            maze->SetCellTag(delCell, 8);
                            maze->Draw();
                            lastCell = delCell;
                            index--;
                        }
                        // 移動ログを該当位置まで削除しながら壁を戻す
                        lastCell = cell;
                        maze->SetCellTag(lastCell, 1);
                        maze->Draw();
                        while (visitedCells.size() > 0) {
                            CellPoint delCell = *(visitedCells.end() - 1);
                            visitedCells.erase(visitedCells.end() - 1);
                            if (delCell.x == cell2.x && delCell.y == cell2.y) {
                                break;
                            }
                            maze->SetCellTag(delCell, 1);
                            maze->Draw();
                            lastCell = delCell;
                        }

                        // 移動ログに追加して探索を再開する
                        visitedCells.push_back(cell2);
                        maze->SetCellTag(cell2, 2);
                        maze->Draw();
                        cell = cell2;
                    }
                    break;
                }
            }
        }

        // 移動ログをたどってすべて「処理済み」にする
        for (int i = (int)visitedCells.size() - 1; i >= 0; i--) {
            CellPoint cell = visitedCells[i];
            maze->SetCellTag(cell, 0);
            maze->Draw();
            cellCount++;
        }
    }
}

// Wilsonのアルゴリズムによる迷路生成
Maze *CreateMaze_Wilson(int xSize, int ySize)
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
    Wilson_Impl(maze, maze->GetXSize() * maze->GetYSize(), true);

    // 終了
    return maze;
}

