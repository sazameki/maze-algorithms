//
//  Maze_Blobby.hpp
//
//  Created by Satoshi Numata on 2018/12/04.
//  License: Public domain.
//

#ifndef Maze_Blobby_hpp
#define Maze_Blobby_hpp

#include "Drawing.hpp"
#include "MazeDraw.hpp"


/// "Blobby" Recursive Subdivisionのアルゴリズムによる迷路生成
/// cf: https://www.jamisbuck.org/mazes/
Maze    *CreateMaze_Blobby(int xSize, int ySize);


#endif /* Maze_Blobby_hpp */

