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
    ���H��`�悵�܂��B
    @param  maze        ���H
    @param  usesBatch   �`��Ƀo�b�`�������g�p���邩�i�f�t�H���g��true�j
 */
void    DrawMaze(Maze *maze, bool usesBatch = true);

/**
    ���H��̌�_��`�悵�܂��B
    @param  maze    ���H
    @param  pos     ��_�̍��W
    @param  usesBatch   �`��Ƀo�b�`�������g�p���邩�i�f�t�H���g��true�j
 */
void    DrawCrossPoint(Maze *maze, const CrossPoint& pos, bool usesBatch = true);

/**
    ���H��ɒT���̂��߂̐l�`��`�悵�܂��B
    @param  maze    ���H
    @param  pos     �Z���̍��W
    @param  dir     �l�`�̌���
 */
void    DrawMan(Maze *maze, const CellPoint& pos, Direction dir);


#endif /* MazeDraw_hpp */

