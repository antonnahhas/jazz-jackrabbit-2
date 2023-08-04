//----------------------------------include section-----------------------------------
#pragma once
#include "Weapon.h"

//----------------------------------class definition----------------------------------
class SmokeWeapon : public Weapon
{
public:
	SmokeWeapon(sf::Vector2f, Direction);
	virtual void createBody(int, int, int, b2World*, sf::Vector2f, sf::Vector2f = { 1,1 }) override;
};