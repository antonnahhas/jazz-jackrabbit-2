//----------------------------------Include Section-----------------------------------

#include "SentryEnemy.h"

//----------------------------------Class Implementation------------------------------


SentryEnemy::SentryEnemy(sf::Vector2f pos, const Jazz& jazz) :
	Enemy(pos, Resources::instance().animationData(Resources::SentryEnemy), jazz)
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_enemies));

	m_limBottomRight.x = m_sprite.getPosition().x;
	m_limTopLeft.x = m_sprite.getPosition().x;
	m_limBottomRight.y -= (jazz.getGlobalBounds().height / 2);
	m_limTopLeft.y += (jazz.getGlobalBounds().height / 2);
	setDirection(Direction::Stay);
}

//update the enemy's action according to the time that passed
void SentryEnemy::updateAction(const sf::Time& deltaTime)
{
	m_elapsed += deltaTime;
	setAction();
	DynamicObject::updateDynamicObject(deltaTime, enemyActionToInt(EnemyAction::Idle));
}

Direction SentryEnemy::getDirection() const
{
	return DynamicObject::getDirection();
}

//true if the player is in the same level as the enemy
bool SentryEnemy::inEnemyLines() const
{
	return (m_jazz.getPosition().x < m_sprite.getPosition().x + ATTACK_RANGE &&
		m_jazz.getPosition().x > m_sprite.getPosition().x - ATTACK_RANGE);
}

//change action if player entered a certain range
void SentryEnemy::handleInvasion()
{
	if (m_elapsed.asSeconds() > SEC_LIM)
	{
		m_elapsed = m_elapsed.Zero;
		m_activateWeapon = true;
	}
}

//set the enemy's action according to the time that has passed
void SentryEnemy::setAction()
{
	if (inEnemySight())	
		if (inEnemyLines())
		{
			lookAtJazz();
			handleInvasion();
		}
}

//change the direction of the sprite to face jazz when jazz gets close to him
void SentryEnemy::lookAtJazz()
{
	if (m_jazz.getPosition().x < m_sprite.getPosition().x)
	{
		m_sprite.setScale(LEFT_SPRITE_TURN);
		setDirection(Direction::Left);
	}
	else
	{
		m_sprite.setScale(RIGHT_SPRITE_TURN);
		setDirection(Direction::Right);
	}
}