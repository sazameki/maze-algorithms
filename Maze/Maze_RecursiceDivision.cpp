//
//  Maze_RecursiceDivision.cpp
//
//  Created by numata on 2018/12/05.
//  Copyright © 2018 Satoshi Numata. All rights reserved.
//

#include "Maze_RecursiceDivision.hpp"


// Recursive Divisionのアルゴリズムによる迷路生成
Maze *CreateMaze_RecursiveDivision(int xSize, int ySize)
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
    
    
    return maze;
}

