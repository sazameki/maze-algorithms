//
//  Maze_RightHand.hpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#ifndef Maze_RightHand_hpp
#define Maze_RightHand_hpp

#include "Drawing.hpp"
#include "MazeDraw.hpp"


/**
    右手法による迷路探索
    @param  maze    迷路
    @param  start   スタート位置
    @param  initDir 探索開始時の方向
    @param  goal    ゴール位置
 */
void    SolveMaze_RightHand(Maze *maze, const CellPoint& start, Direction initDir, const CellPoint& goal);


#endif /* Maze_RightHand_hpp */

