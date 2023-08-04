//----------------------------------include section-----------------------------------
#pragma once
#include "Direction.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <vector>

//----------------------------------class section-----------------------------------

struct AnimationData
{
    //each animation has a frame sheet that is represented by a Rect, and there's a vector saving all animations
    //and is reffered by an enum (int)
    using ListType = std::vector<sf::IntRect>;
    using DataType = std::unordered_map<int, ListType>;
    using ListPoint = std::vector<sf::Vector2f>;
    using DataOrigin = std::unordered_map<int, ListPoint>;

    DataType m_animeMap;      //holds the frames sheet of the animation
    DataOrigin m_originPoint; //the origin point of the frames, individual for each frame
};