//
//  Maze_Dijkstra.hpp
//
//  Created by Satoshi Numata on 2018/12/30.
//  License: Public domain.
//

#ifndef Maze_Dijkstra_hpp
#define Maze_Dijkstra_hpp

#include "Drawing.hpp"
#include "MazeDraw.hpp"


/**
    Dijkstraのアルゴリズムによる迷路探索
    @param  maze    迷路
    @param  start   スタート位置
    @param  goal    ゴール位置
 */
void    SolveMaze_Dijkstra(Maze *maze, const CellPoint& start, const CellPoint& goal);


#endif /* Maze_Dijkstra_hpp */

