//----------------------------------include section-----------------------------------
#pragma once
#include "Enemy.h"

//----------------------------------class definition----------------------------------
class MeleeEnemy : public Enemy
{
public:
	MeleeEnemy(sf::Vector2f, const Jazz&, sf::Vector2f);
	virtual void setAction() override;
	virtual void updateAction(const sf::Time& deltaTime) override;

private:
	const float ATTACK_RANGE = 50;

	bool inAttackRange() const;
	bool inEnemyLines()  const;
	void handleInvasion();
};