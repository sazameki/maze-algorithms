//
//  Maze_BlobbyRecursiveDivision.hpp
//
//  Created by Satoshi Numata on 2018/12/04.
//  License: Public domain.
//

#ifndef Maze_BlobbyRecursiveDivision_hpp
#define Maze_BlobbyRecursiveDivision_hpp

#include "Drawing.hpp"
#include "MazeDraw.hpp"


/// "Blobby" Recursive Subdivisionのアルゴリズムによる迷路生成
/// cf: https://www.jamisbuck.org/mazes/
/// 解説：「Blobby」というのは「不定形の」という意味です。領域が直線でなく凸凹に分割されるということ以外、やっていることは「Recursive Division」と同じです。
/// 領域を2つに分割して、1マスだけ除いて境界線を書いていくことで、迷路を生成します。
Maze    *CreateMaze_BlobbyRecursiveDivision(int xSize, int ySize);


#endif /* Maze_BlobbyRecursiveDivision_hpp */

