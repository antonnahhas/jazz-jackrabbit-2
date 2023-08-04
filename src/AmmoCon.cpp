  //----------------------------------include section-------------------------------------
#include "AmmoCon.h"

//----------------------------------class implementation------------------------------

AmmoCon::AmmoCon(sf::Vector2f pos) :
	Consumable(pos, Resources::instance().animationData(Resources::AmmoCon))
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_items));
}