//
//  MazeDraw.hpp
//
//  Created by numata on 2018/12/02.
//  Copyright © 2018 Satoshi Numata. All rights reserved.
//

#ifndef MazeDraw_hpp
#define MazeDraw_hpp

#include "Maze.hpp"


void    DrawMaze(Maze *maze, bool usesBatch = true);
void    DrawMan(Maze *maze, const Point& pos, Direction dir);


#endif /* MazeDraw_hpp */

