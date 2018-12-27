//
//  Maze_RightHand.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "Maze_RightHand.hpp"


void SolveMaze_RightHand(Maze *maze)
{
    // (0, 0)の地点をスタートとして繰り返し
    CellPoint pos(0, 0);
    Direction dir = Right;
    maze->SetCellTag(pos, 1);
    StartBatch();
    {
        maze->Draw();
        DrawMan(maze, pos, dir);
    }
    EndBatch();

    // ゴールにたどり着くまで繰り返し
    while (pos.x != maze->GetXSize() - 1 || pos.y != maze->GetYSize() - 1) {
        // 正面に壁があったら左回転
        while (maze->CheckWall(pos, dir)) {
            dir = RotateLeft(dir);
            maze->Draw();
            DrawMan(maze, pos, dir);
        }

        // 正面に壁がなければ前進
        while (!maze->CheckWall(pos, dir)) {
            // 前進の処理
            pos = pos.Move(dir);
            maze->SetCellTag(pos, 1);
            StartBatch();
            {
                maze->Draw(false);
                DrawMan(maze, pos, dir);
            }
            EndBatch();

            // 右側の壁が空いていれば右回転
            Direction rightDir = RotateRight(dir);
            if (!maze->CheckWall(pos, rightDir)) {
                dir = rightDir;
                StartBatch();
                {
                    maze->Draw();
                    DrawMan(maze, pos, dir);
                }
                EndBatch();
                break;
            }
        }
    }

    DrawText("SOLVED!! CMD-R TO RESTART.", -12*13, -240, kColorBlue);
}
