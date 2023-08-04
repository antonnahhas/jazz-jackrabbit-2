//----------------------------------Include Section-----------------------------------

#include "MeleeEnemy.h"
#include <iostream> 

//----------------------------------Class Implementation------------------------------

MeleeEnemy::MeleeEnemy(sf::Vector2f pos, const Jazz& jazz, sf::Vector2f limit) :
	Enemy(pos, Resources::instance().animationData(Resources::MeleeEnemy), jazz)
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_enemies));

	m_limTopLeft.x = limit.x;
	m_limBottomRight.x = limit.y;
	m_limBottomRight.y -= (jazz.getGlobalBounds().height / 2);
	m_limTopLeft.y += (jazz.getGlobalBounds().height / 2);
	setDirection(Direction::Right);
	setFaceDirection(false);
	m_enemyAction = EnemyAction::Walk;

}

//----------------------------------Functions Section---------------------------------

//update the enemy's action according to the time that passed
void MeleeEnemy::updateAction(const sf::Time& deltaTime)
{
	setAction();
	m_body->SetLinearVelocity(b2Vec2(toVector(m_dir).x * MOVE_SPEED, toVector(m_dir).y * MOVE_SPEED));
	DynamicObject::updateDynamicObject(deltaTime, enemyActionToInt(m_enemyAction));
}

//set the enemy's action according to the time that has passed
void MeleeEnemy::setAction()
{
	if (inEnemySight())
	{
		lookAtJazz();

		if (inEnemyLines())
		{
			handleInvasion();
			return;
		}
		setDirection(Direction::Stay);
		m_enemyAction = EnemyAction::Idle;
	}

	else
	{
		assumeWalking();
		checkBoundaries();
	}
}

//true if jazz's in a certain attack range
bool MeleeEnemy::inAttackRange() const
{
	return (m_sprite.getPosition().x + ATTACK_RANGE >= m_jazz.getPosition().x &&
		m_sprite.getPosition().x - ATTACK_RANGE <= m_jazz.getPosition().x);
}

//true if the player is in the same level as the enemy
bool MeleeEnemy::inEnemyLines() const
{
	return (m_jazz.getPosition().x < m_limBottomRight.x&&
		m_jazz.getPosition().x > m_limTopLeft.x);
}

//change action if player entered a certain range
void MeleeEnemy::handleInvasion()
{
	if (inAttackRange())
	{
		m_enemyAction = EnemyAction::Attack;
		setDirection(Direction::Stay);
	}
	else
		m_enemyAction = EnemyAction::Walk;
}



