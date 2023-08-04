//----------------------------------include section-------------------------------------

#include "Consumable.h"

//----------------------------------class implementation------------------------------

Consumable::Consumable(sf::Vector2f pos, const AnimationData& data):
	StaticObject(pos), m_animation(data, m_sprite) {}

//----------------------------------function section-------------------------------------

void Consumable::animateStaticObject(sf::Time delta)
{
	m_animation.update(delta);
}

