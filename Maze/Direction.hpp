//
//  Direction.hpp
//
//  Created by numata on 2018/12/02.
//  Copyright © 2018 Satoshi Numata. All rights reserved.
//

#ifndef Direction_hpp
#define Direction_hpp


/// 方向を表す定数
enum Direction
{
    /// 上方向
    Up,

    /// 右方向
    Right,

    /// 下方向
    Down,

    /// 左方向
    Left,
};


Direction   GetOppositeDirection(Direction dir);
Direction   RotateRight(Direction dir);
Direction   RotateLeft(Direction dir);


#endif /* Direction_hpp */

