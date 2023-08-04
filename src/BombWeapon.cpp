
//----------------------------------include section-----------------------------------
#include "BombWeapon.h"

//----------------------------------const section-----------------------------------
const int EXPLODE = 1;

//----------------------------------class implementation------------------------------

// constructor - initialize all the members
BombWeapon::BombWeapon(sf::Vector2f pos, Direction dir):
	Weapon(pos, Resources::instance().animationData(Resources::BombWeapon))
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_effects));
	setDirection(dir);
}

void BombWeapon::updateAction(const sf::Time& deltaTime)
{
	setAction();
	DynamicObject::updateDynamicObject(deltaTime, m_action);
}

//----------------------------------functions section---------------------------------

void BombWeapon::createBody(int offset, int height, int gravity,b2World* world, sf::Vector2f position, sf::Vector2f size)
{
	Weapon::createBody(offset, height, gravity, world, position, size);
}

void BombWeapon::setAction()
{
	if (m_dir == Direction::Stay)
	{
		if (m_animation.animationEneded() && m_action == EXPLODE)
			setDisposed();
		m_action = EXPLODE;
	}	
}