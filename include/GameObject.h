//----------------------------------include section-----------------------------------
#pragma once
#include <SFML/Graphics.hpp>
#include "box2d/box2d.h"
#include "Resources.h"
#include "macros.h"
#include "Animation.h"
#include "Direction.h"

//----------------------------------const section-----------------------------------
const float DENS = 10.f, FRIC = 100.f, MASS = 5.f;

//----------------------------------class definition----------------------------------
class GameObject
{
public:
	GameObject() = default;
	GameObject(sf::Vector2f);

	virtual ~GameObject();
	virtual void draw(sf::RenderWindow&) const;
	virtual sf::Vector2f getPosition() const;
	virtual sf::FloatRect getGlobalBounds() const;
	virtual b2Body* getBody() const;
	virtual void createBody(int, int, int, b2World*, sf::Vector2f, sf::Vector2f = { 15,1 }) = 0;

	bool checkCollision(const sf::FloatRect&) const;
	void destroyBody();

	bool isDisposed() const;
	void setPosition(sf::Vector2f pos);
	void setDisposed(bool = true);


protected:
	b2Vec2 convertVec2fTob2Vec2(sf::Vector2f) const;
	sf::Vector2f convertb2Vec2ToVec2f(b2Vec2) const;

	b2Body* m_body = nullptr;   // the box2D body of the object
	sf::Vector2f m_position;    // position of the game object
	sf::Sprite m_sprite;        // sprite of the object

private:
	bool m_isDisposed = false;  // if the object should be removed from the world
};