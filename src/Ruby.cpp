//----------------------------------include section-------------------------------------
#include "Ruby.h"

//----------------------------------class implementation------------------------------

Ruby::Ruby(sf::Vector2f pos) : 
	Consumable(pos, Resources::instance().animationData(Resources::Ruby))
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_items));
}