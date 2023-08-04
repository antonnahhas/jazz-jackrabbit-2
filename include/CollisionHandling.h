//----------------------------------include section-----------------------------------
#pragma once
#include "GameObject.h"
#include <iostream>
#include <typeinfo>

//----------------------------------struct definition---------------------------------

// struct for exceptions
struct UnknownCollision : public std::runtime_error
{
    UnknownCollision(GameObject& a, GameObject& b)
        : std::runtime_error(std::string("Unknown collision of ") + typeid(a).name() + " and " + typeid(b).name())
    {}
};

void processCollision(GameObject& object1, GameObject& object2);
