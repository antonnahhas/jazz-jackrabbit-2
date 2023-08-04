//----------------------------------include section-----------------------------------
#include "Direction.h"
#include <iostream>
#include <stdexcept>
#include "macros.h"

//----------------------------------functions section---------------------------------

//oppose a given direction
Direction opposite(Direction dir)
{
    switch (dir)
    {
        case Direction::Up:
            return Direction::Down;
        case Direction::Down:
            return Direction::Up;
        case Direction::Right:
            return Direction::Left;
        case Direction::Left:
            return Direction::Right;
        case Direction::Stay:
            return Direction::Stay;
    }
    //return Direction::Stay; // added for warning
}

//return a vector according to a given direction
sf::Vector2f toVector(Direction dir)
{
    switch (dir)
    {
        case Direction::Up:
            return UP_VEC;
        case Direction::Down:
            return DOWN_VEC;
        case Direction::Right:
            return RIGHT_VEC;
        case Direction::Left:
            return LEFT_VEC;
        case Direction::Stay:
            return STAY_VEC;
    }
    //return STAY_VEC; // added for warning
}

