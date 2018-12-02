//
//  Direction.cpp
//
//  Created by numata on 2018/12/02.
//  Copyright © 2018 Satoshi Numata. All rights reserved.
//

#include "Direction.hpp"


Direction GetOppositeDirection(Direction dir)
{
    if (dir == Up) {
        return Down;
    } else if (dir == Right) {
        return Left;
    } else if (dir == Down) {
        return Up;
    } else {
        return Right;
    }
}

Direction RotateRight(Direction dir)
{
    if (dir == Up) {
        return Right;
    } else if (dir == Right) {
        return Down;
    } else if (dir == Down) {
        return Left;
    } else {
        return Up;
    }
}

Direction RotateLeft(Direction dir)
{
    if (dir == Up) {
        return Left;
    } else if (dir == Left) {
        return Down;
    } else if (dir == Down) {
        return Right;
    } else {
        return Up;
    }
}

