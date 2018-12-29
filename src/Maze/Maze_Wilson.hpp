//
//  Maze_Wilson.hpp
//
//  Created by Satoshi Numata on 2018/12/21.
//  License: Public domain.
//

#ifndef Maze_Wilson_hpp
#define Maze_Wilson_hpp

#include "Drawing.hpp"
#include "MazeDraw.hpp"


/**
    Wilsonのアルゴリズムによる迷路生成

    解説：
        1. （準備）最初にセルを1つランダムに選んで「処理済み」にします。
        2. セルを1つランダムに選んで「開始点」にします。
        3. 「開始点」から「処理済み」のセルにたどり着くまで「未処理」のセルだけをランダムウォークします。
           移動する際にはその方向の壁を取り除き、通路を作成します。すでに通ったセルにもう一度来た場合、
           以前にそのセルに来た時までのログをたどって、壁を復元していきます。
        4. 「処理済み」でないセルが残っている間、手順2に戻ります。
 */
Maze    *CreateMaze_Wilson(int xSize, int ySize);


#endif /* Maze_Wilson_hpp */

