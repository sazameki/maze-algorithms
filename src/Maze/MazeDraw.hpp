//
//  MazeDraw.hpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#ifndef MazeDraw_hpp
#define MazeDraw_hpp

#include "Maze.hpp"


/**
    迷路を描画します。
    @param  maze        迷路
    @param  usesBatch   描画にバッチ処理を使用するか（デフォルトはtrue）
 */
void    DrawMaze(Maze *maze, bool usesBatch = true);

/**
    迷路上の交点を描画します。
    @param  maze    迷路
    @param  pos     交点の座標
    @param  usesBatch   描画にバッチ処理を使用するか（デフォルトはtrue）
 */
void    DrawCrossPoint(Maze *maze, const CrossPoint& pos, bool usesBatch = true);

/**
    迷路上に探索のための人形を描画します。
    @param  maze    迷路
    @param  pos     セルの座標
    @param  dir     人形の向き
 */
void    DrawMan(Maze *maze, const CellPoint& pos, Direction dir);


#endif /* MazeDraw_hpp */

