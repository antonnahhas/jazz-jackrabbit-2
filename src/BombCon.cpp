//----------------------------------include section-------------------------------------
#include "BombCon.h"

//----------------------------------class implementation------------------------------

BombCon::BombCon(sf::Vector2f pos) :
	Consumable(pos, Resources::instance().animationData(Resources::BombCon))
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_items));
}