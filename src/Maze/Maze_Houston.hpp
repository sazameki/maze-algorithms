//
//  Maze_Houston.hpp
//
//  Created by Satoshi Numata on 2018/12/30.
//  License: Public domain.
//

#ifndef Maze_Houston_hpp
#define Maze_Houston_hpp

#include "Drawing.hpp"
#include "MazeDraw.hpp"


/**
    Houstonのアルゴリズムによる迷路生成

    解説：
        1. 一定量のセルを処理し終えるまで、Aldous-Broderのアルゴリズムで迷路を生成します。
        2. 残りのセルを、Wilsonのアルゴリズムで処理します。すでに通路ができているので、最初にセルを1個処理済みにする手順はスキップします。
 */
Maze    *CreateMaze_Houston(int xSize, int ySize);


#endif /* Maze_Houston_hpp */

