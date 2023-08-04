//----------------------------------include section-----------------------------------
#include "Weapon.h"

//----------------------------------const section-----------------------------------

//const int THROW_HEIGHT = -20, THROW_OFFSET = 40;
//----------------------------------class implementation------------------------------

Weapon::Weapon(sf::Vector2f pos, const AnimationData& data) 
	: DynamicObject(pos, data)
{}

//----------------------------------functions section---------------------------------

void Weapon::updateAction(const sf::Time& deltaTime)
{
	DynamicObject::updateDynamicObject(deltaTime, INIT);
}

void Weapon::createBody(int offset, int height, int gravity, b2World* world, sf::Vector2f position , sf::Vector2f size)
{
	DynamicObject::createBody(offset, height, gravity, world, position, DEF_SIZE);

	m_body->SetLinearVelocity(b2Vec2(toVector(m_dir).x * offset, height));
	m_body->SetGravityScale(gravity);
}