//----------------------------------include section-------------------------------------

#include "DynamicObject.h"
#include "Weapon.h"

//----------------------------------class implementation------------------------------

DynamicObject::DynamicObject(sf::Vector2f pos, const AnimationData& data) :
	GameObject(pos),
	m_animation(data, m_sprite)
{}

//----------------------------------functions section---------------------------------

//create a box2d body for the movable
void DynamicObject::createBody(int offset, int height, int gravity, b2World* world, sf::Vector2f position, sf::Vector2f size) // create box2d dynamic body
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	m_body = world->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(size.x, size.y);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	fixtureDef.density = DENS;  //check 10, 100
	fixtureDef.friction = FRIC;
	m_body->CreateFixture(&fixtureDef);
	m_body->SetFixedRotation(true);
	b2MassData massa;
	massa.mass = MASS;
	massa.center = m_body->GetLocalCenter();

	m_body->SetMassData(&massa);
	m_sprite.setPosition(convertb2Vec2ToVec2f(m_body->GetPosition()));
}

//update the movable's action, animation and position
void DynamicObject::updateDynamicObject(sf::Time deltaTime, int action)
{
	m_sprite.setPosition(convertb2Vec2ToVec2f(m_body->GetPosition()));
	m_animation.action(action, m_dir);
	m_animation.update(deltaTime);
}

//----------------------------------set functions section---------------------------------

void DynamicObject::setThrowingWeapon(bool throwing) { m_activateWeapon = throwing; }

void DynamicObject::setDirection(Direction dir) { m_dir = dir; }

//----------------------------------get functions section---------------------------------

//get the player's direction according to the scale of its sprite
Direction DynamicObject::getDirection() const
{
	if (m_sprite.getScale() == LEFT_SPRITE_TURN)
	{
		return Direction::Left;
	}
	else if (m_sprite.getScale() == RIGHT_SPRITE_TURN)
	{
		return Direction::Right;
	}
	return Direction::Stay;
}

bool DynamicObject::isThrowingWeapon() const { return m_activateWeapon; }