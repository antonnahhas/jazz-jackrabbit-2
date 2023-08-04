//----------------------------------include section-----------------------------------
#pragma once
#include "GameObject.h"

//----------------------------------class definition----------------------------------
class StaticObject : public GameObject
{
public:
	using GameObject::GameObject;
	virtual ~StaticObject() = default;

	virtual void animateStaticObject(sf::Time delta) = 0;
};