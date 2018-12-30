//
//  Maze_RightHand.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "Maze_RightHand.hpp"


void SolveMaze_RightHand(Maze *maze, const CellPoint& start, Direction initDir, const CellPoint& goal)
{
    // 開始時の設定
    CellPoint cell = start;
    Direction dir = initDir;
    maze->SetCellTag(cell, 1);
    StartBatch();
    maze->Draw(false);
    maze->DrawStart(start);
    maze->DrawGoal(goal);
    DrawMan(maze, cell, dir);
    EndBatch();

    // ゴールにたどり着くまで繰り返し
    while (!cell.Equals(goal)) {
        // 正面に壁があったら左回転
        while (maze->CheckWall(cell, dir)) {
            dir = RotateLeft(dir);
            StartBatch();
            maze->Draw(false);
            maze->DrawStart(start);
            maze->DrawGoal(goal);
            DrawMan(maze, cell, dir);
            EndBatch();
        }

        // 正面に壁がなければ前進
        while (!maze->CheckWall(cell, dir)) {
            // 前進の処理
            CellPoint cell2 = cell.Move(dir);
            if (maze->GetCellTag(cell2) != 0) {
                maze->SetCellTag(cell, 2);
            }
            maze->SetCellTag(cell2, 1);
            StartBatch();
            maze->Draw(false);
            maze->DrawStart(start);
            maze->DrawGoal(goal);
            DrawMan(maze, cell, dir);
            EndBatch();
            cell = cell2;

            // 右側の壁が空いていれば右回転
            Direction rightDir = RotateRight(dir);
            if (!maze->CheckWall(cell, rightDir)) {
                dir = rightDir;
                StartBatch();
                maze->Draw(false);
                maze->DrawStart(start);
                maze->DrawGoal(goal);
                DrawMan(maze, cell, dir);
                EndBatch();
                break;
            }
        }
    }

    // 最後の描画
    StartBatch();
    maze->Draw(false);
    maze->DrawStart(start);
    maze->DrawGoal(goal);
    DrawMan(maze, cell, dir);
    EndBatch();
}

