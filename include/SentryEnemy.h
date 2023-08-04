//----------------------------------include section-----------------------------------
#pragma once
#include "Enemy.h"

//----------------------------------class definition----------------------------------
class SentryEnemy : public Enemy
{
public:
	SentryEnemy(sf::Vector2f, const Jazz& jazz);
	virtual void setAction() override;
	virtual void updateAction(const sf::Time&) override;
	virtual Direction getDirection() const override; // Additional override for sentry because is facing same as Jazz

private:
	const float ATTACK_RANGE = 150;

	bool inEnemyLines() const;
	void handleInvasion();
	void lookAtJazz();
};