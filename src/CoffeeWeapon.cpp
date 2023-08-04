//----------------------------------Include Section---------------------------------

#include "CoffeeWeapon.h"

//----------------------------------Class Implementation------------------------------

CoffeeWeapon::CoffeeWeapon(sf::Vector2f pos, Direction dir) :
	Weapon(pos, Resources::instance().animationData(Resources::CoffeeWeapon))
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_enemies));
	setDirection(dir);
}

void CoffeeWeapon::createBody(int offset, int height, int gravity, b2World* world, sf::Vector2f position, sf::Vector2f size)
{
	Weapon::createBody(offset, height, gravity, world, position, size);
}

