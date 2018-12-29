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

/**
    Wilsonのアルゴリズムによる迷路生成の実装です。
    Houstonのアルゴリズムのために実装部分を分けています。
    @param  maze    迷路
    @param  cellCountThreshold  このアルゴリズムで処理するセルの個数
    @param  opensFirstCell      最初のセルを1個ランダムに選択して処理済みにするかどうか
 */
void Wilson_Impl(Maze *maze, int cellCountThreshold, bool opensFirstCell);


#endif /* Maze_Wilson_hpp */

