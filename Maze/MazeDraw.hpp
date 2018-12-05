//
//  MazeDraw.hpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#ifndef MazeDraw_hpp
#define MazeDraw_hpp

#include "Maze.hpp"


void    DrawMaze(Maze *maze, bool usesBatch = true);
void    DrawCrossPoint(Maze *maze, const CrossPoint& pos, bool usesBatch = true);
void    DrawMan(Maze *maze, const CellPoint& pos, Direction dir);


#endif /* MazeDraw_hpp */

