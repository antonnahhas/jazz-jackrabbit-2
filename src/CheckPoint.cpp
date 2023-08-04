//----------------------------------include section-----------------------------------
#include "CheckPoint.h"

//----------------------------------class implementation------------------------------

CheckPoint::CheckPoint(sf::Vector2f pos) :
	Consumable(pos, Resources::instance().animationData(Resources::CheckPoint))
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_items));
}
