//----------------------------------include section-----------------------------------

#include "LifeCon.h"

//----------------------------------class implementation------------------------------

LifeCon::LifeCon(sf::Vector2f pos) : Consumable(pos, Resources::instance().animationData(Resources::LifeCon))

{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_items));
}
