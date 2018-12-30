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
    CellPoint pos = start;
    Direction dir = initDir;
    maze->SetCellTag(pos, 1);
    StartBatch();
    maze->Draw(false);
    maze->DrawStart(start);
    maze->DrawGoal(goal);
    DrawMan(maze, pos, dir);
    EndBatch();

    // ゴールにたどり着くまで繰り返し
    while (pos.x != goal.x || pos.y != goal.y) {
        // 正面に壁があったら左回転
        while (maze->CheckWall(pos, dir)) {
            dir = RotateLeft(dir);
            StartBatch();
            maze->Draw(false);
            maze->DrawStart(start);
            maze->DrawGoal(goal);
            DrawMan(maze, pos, dir);
            EndBatch();
        }

        // 正面に壁がなければ前進
        while (!maze->CheckWall(pos, dir)) {
            // 前進の処理
            pos = pos.Move(dir);
            maze->SetCellTag(pos, 1);
            StartBatch();
            maze->Draw(false);
            maze->DrawStart(start);
            maze->DrawGoal(goal);
            DrawMan(maze, pos, dir);
            EndBatch();

            // 右側の壁が空いていれば右回転
            Direction rightDir = RotateRight(dir);
            if (!maze->CheckWall(pos, rightDir)) {
                dir = rightDir;
                StartBatch();
                maze->Draw(false);
                maze->DrawStart(start);
                maze->DrawGoal(goal);
                DrawMan(maze, pos, dir);
                EndBatch();
                break;
            }
        }
    }
}

