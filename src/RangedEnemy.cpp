//----------------------------------Include Section---------------------------------

#include "RangedEnemy.h"

//----------------------------------const section-------------------------------------
const int IDLE_TIME = 4, WALK_TIME = 9, ATTACK_RNG_TIME = 1;
//----------------------------------Class Implementation------------------------------

RangedEnemy::RangedEnemy(sf::Vector2f pos, const Jazz& jazz, sf::Vector2f limit) :
	Enemy(pos, Resources::instance().animationData(Resources::RangedEnemy), jazz)
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
void RangedEnemy::updateAction(const sf::Time& deltaTime)
{
	m_elapsed += deltaTime;
	setAction();
	m_body->SetLinearVelocity(b2Vec2(toVector(m_dir).x * MOVE_SPEED, toVector(m_dir).y * MOVE_SPEED));
	DynamicObject::updateDynamicObject(deltaTime, enemyActionToInt(m_enemyAction));
	if (m_enemyAction == EnemyAction::Attack && m_elapsed.asSeconds() > SEC_LIM)
	{
		m_elapsed = m_elapsed.Zero;
		m_activateWeapon = true;
	}
}

//set the enemy's action according to the time that has passed
void RangedEnemy::setAction()
{
	if (inEnemySight())
		if (inEnemyLines())	
		{
			lookAtJazz();
			handleInvasion();
			return;
		}

	if (m_elapsed.asSeconds() < IDLE_TIME)
	{
		m_enemyAction = EnemyAction::Idle;
		setDirection(Direction::Stay);
	}
	else if (m_elapsed.asSeconds() < WALK_TIME)
	{
		assumeWalking(); 
		checkBoundaries();
	}
	else
		m_elapsed = m_elapsed.Zero;
}

//true if the player is in the same level as the enemy
bool RangedEnemy::inEnemyLines() const
{
	return (m_jazz.getPosition().x < m_limBottomRight.x + ATTACK_RANGE &&
		m_jazz.getPosition().x > m_limTopLeft.x - ATTACK_RANGE);
}

//change action if player entered a certain range
void RangedEnemy::handleInvasion()
{
	if (inAttackRange())
	{
		if (m_elapsed.asSeconds() > ATTACK_RNG_TIME)
		{
			m_enemyAction = EnemyAction::Attack;
			m_elapsed = m_elapsed.Zero;
			m_activateWeapon = true;
		}

		setDirection(Direction::Stay);
	}
	else
		m_enemyAction = EnemyAction::Walk;
}

//true if jazz's in a certain attack range
bool RangedEnemy::inAttackRange() const
{
	return (m_sprite.getPosition().x + ATTACK_RANGE >= m_jazz.getPosition().x &&
		m_sprite.getPosition().x - ATTACK_RANGE <= m_jazz.getPosition().x);
}