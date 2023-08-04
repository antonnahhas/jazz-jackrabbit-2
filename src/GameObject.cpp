//----------------------------------include section-------------------------------------
#include "GameObject.h"

//----------------------------------class implementation------------------------------

GameObject::GameObject(sf::Vector2f position)
	: m_position(position)
{
	m_sprite.setPosition(position);
}

//----------------------------------functions section---------------------------------

GameObject::~GameObject()
{
	m_body = nullptr;
}

void GameObject::draw(sf::RenderWindow& window) const
{
	window.draw(m_sprite);
}

bool GameObject::checkCollision(const sf::FloatRect& gloBou) const
{
	return m_sprite.getGlobalBounds().intersects(gloBou);
}

//type conversion funcs
b2Vec2 GameObject::convertVec2fTob2Vec2(sf::Vector2f pos) const { return b2Vec2({ pos.x , pos.y }); }

sf::Vector2f GameObject::convertb2Vec2ToVec2f(b2Vec2 pos) const { return sf::Vector2f({ pos.x , pos.y }); }

//----------------------------------set functions section------------------------------

void GameObject::destroyBody() { m_body = nullptr; }

void GameObject::setPosition(sf::Vector2f pos) { m_sprite.setPosition(pos); }

void GameObject::setDisposed(bool isDisposed) { m_isDisposed = isDisposed; }

//----------------------------------get functions section------------------------------

b2Body* GameObject::getBody() const { return m_body; }

sf::Vector2f GameObject::getPosition() const { return m_sprite.getPosition(); }

sf::FloatRect GameObject::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

bool GameObject::isDisposed() const { return m_isDisposed; }

