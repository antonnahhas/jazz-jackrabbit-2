//----------------------------------include section-----------------------------------
#pragma once
#include "Weapon.h"

//----------------------------------class definition----------------------------------
class BombWeapon : public Weapon
{
public:
	BombWeapon(sf::Vector2f, Direction);
	virtual void updateAction(const sf::Time&) override;

	virtual void setAction() override;
	virtual void createBody(int, int, int, b2World*, sf::Vector2f, sf::Vector2f = { 1,1 }) override;
private:
	int m_action = INIT;
};