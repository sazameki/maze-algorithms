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


/**
    方向を表す定数の列挙型
 */
enum Direction
{
    /**
        上方向を表す定数
     */
    Up,

    /**
        右方向を表す定数
     */
    Right,

    /**
        下方向を表す定数
     */
    Down,

    /**
        左方向を表す定数
     */
    Left,
};


/**
    与えられた方向と反対の方向をリターンします。
 */
Direction   GetOppositeDirection(Direction dir);

/**
    上下左右いずれかの方向から、ランダムに一方向を選択してリターンします。
 */
Direction   GetRandomDirection();

/**
    与えられた方向を右方向に回転します。
    例. 上->右, 右->下
 */
Direction   RotateRight(Direction dir);

/**
    与えられた方向を左方向に回転します。
    例. 上->左, 左->下
 */
Direction   RotateLeft(Direction dir);


#endif /* Direction_hpp */

