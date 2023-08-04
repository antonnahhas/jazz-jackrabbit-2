//----------------------------------include section-----------------------------------
#pragma once

#include "StaticObject.h"

//----------------------------------class definition----------------------------------
class Consumable : public StaticObject
{
public:
	Consumable(sf::Vector2f, const AnimationData& data);

	virtual void createBody(int, int, int, b2World*, sf::Vector2f, sf::Vector2f = { 15,1 }) {};
	virtual void animateStaticObject(sf::Time delta) override;

protected:
	Animation m_animation;  // current animation 
};