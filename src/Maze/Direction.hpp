//
//  Direction.hpp
//
//  Created by Satoshi Numata on 2018/12/02.
//  License: Public domain.
//

#ifndef Direction_hpp
#define Direction_hpp

#ifdef _WINDOWS
#define srandom(seed)   srand((seed))
#define random()        rand()
#endif


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

    /// 方向が設定されていない
    NoDirection,
};


Direction   GetOppositeDirection(Direction dir);
Direction   GetRandomDirection();
Direction   RotateRight(Direction dir);
Direction   RotateLeft(Direction dir);


#endif /* Direction_hpp */

