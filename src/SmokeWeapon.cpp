//----------------------------------Include Section---------------------------------

#include "SmokeWeapon.h"

//----------------------------------Class Implementation------------------------------

SmokeWeapon::SmokeWeapon(sf::Vector2f pos, Direction dir) :
	Weapon(pos, Resources::instance().animationData(Resources::SmokeWeapon))
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_effects));
	setDirection(dir);
}

void SmokeWeapon::createBody(int offset, int height, int gravity, b2World* world, sf::Vector2f position, sf::Vector2f size)
{
	Weapon::createBody(offset, height, gravity, world, position, size);
}

