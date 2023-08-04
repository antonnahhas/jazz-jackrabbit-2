//----------------------------------include section-----------------------------------
#include "FinishLine.h"

//----------------------------------functions section---------------------------------

FinishLine::FinishLine(sf::Vector2f pos) :
	Consumable(pos, Resources::instance().animationData(Resources::FinishLine))
{
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_items));
}
