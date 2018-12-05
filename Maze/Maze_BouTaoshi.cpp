//
//  Maze_BouTaoshi.cpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#include "Maze_BouTaoshi.hpp"


Maze *CreateMaze_BouTaoshi(int xSize, int ySize)
{
    // 迷路の生成
    Maze *maze = new Maze(xSize, ySize, 0);
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
    for (int x = 0; x < maze->GetXSize()-1; x++) {
        // (x,0),(x+1,0),(x+1,1),(x,1)のマスの中間にある枠線（上下左右）を書く
        int r;
        do {
            r = random() % 4;
        } while ((r == 0 && maze->CheckWall(x, 0, Right)) ||
                 (r == 1 && maze->CheckWall(x, 1, Right)) ||
                 (r == 2 && maze->CheckWall(x+1, 0, Down)) ||
                 (r == 3 && maze->CheckWall(x, 0, Down)));
        if (r == 0) {
            maze->MakeWall(x, 0, Right);
        } else if (r == 1) {
            maze->MakeWall(x, 1, Right);
        } else if (r == 2) {
            maze->MakeWall(x + 1, 0, Down);
        } else {
            maze->MakeWall(x, 0, Down);
        }
        maze->Draw();
    }

    // 2行目以降
    for (int y = 1; y < maze->GetYSize()-1; y++) {
        for (int x = 0; x < maze->GetXSize()-1; x++) {
            // (x,0),(x+1,0),(x+1,1),(x,1)のマスの中間にある枠線（上を除く）を書く
            int r;
            do {
                r = random() % 3;
            } while ((r == 0 && maze->CheckWall(x, y+1, Right)) ||
                     (r == 1 && maze->CheckWall(x+1, y, Down)) ||
                     (r == 2 && maze->CheckWall(x, y, Down)));
            if (r == 0) {
                maze->MakeWall(x, y + 1, Right);
            } else if (r == 1) {
                maze->MakeWall(x + 1, y, Down);
            } else {
                maze->MakeWall(x, y, Down);
            }
            maze->Draw();
        }
    }

    return maze;
}

