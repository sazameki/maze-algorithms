//
//  Maze_AldousBroder.hpp
//
//  Created by Satoshi Numata on 2018/12/21.
//  License: Public domain.
//

#ifndef Maze_AldousBroder_hpp
#define Maze_AldousBroder_hpp

#include "Drawing.hpp"
#include "MazeDraw.hpp"


/**
    Aldous-Broderのアルゴリズムによる迷路生成
 */
Maze    *CreateMaze_AldousBroder(int xSize, int ySize);

/**
    Aldous-Broderのアルゴリズムによる迷路生成の実装です。
    Houstonのアルゴリズムのために実装部分を分けています。
    @param  maze    迷路
    @param  cellCountThreshold  このアルゴリズムで処理するセルの個数
 */
void AldousBroder_Impl(Maze *maze, int cellCountThreshold);


#endif /* Maze_AldousBroder_hpp */

