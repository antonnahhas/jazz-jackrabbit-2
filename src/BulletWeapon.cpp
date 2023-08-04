
//----------------------------------include section-----------------------------------
#include "BulletWeapon.h"

//----------------------------------class implementation------------------------------

// constructor - initialize all the members
BulletWeapon::BulletWeapon(sf::Vector2f pos, Direction dir) :
	Weapon(pos, Resources::instance().animationData(Resources::BulletWeapon))
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_effects));
	setDirection(dir);
}

void BulletWeapon::updateAction(const sf::Time& deltaTime)
{
	setAction();
	DynamicObject::updateDynamicObject(deltaTime, m_action);
}

//----------------------------------functions section---------------------------------

void BulletWeapon::createBody(int offset, int height, int gravity, b2World* world, sf::Vector2f position, sf::Vector2f size)
{
	Weapon::createBody (offset, height, gravity, world, position, size);
}

void BulletWeapon::setAction()
{
	if (m_dir == Direction::Stay)
	{
		if (m_animation.animationEneded())
			setDisposed();
	}
}