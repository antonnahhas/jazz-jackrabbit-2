//----------------------------------include section-----------------------------------
#pragma once
#include "StaticObject.h"

//----------------------------------class definition----------------------------------
class Obstacle : public StaticObject
{
public:
	Obstacle(sf::Vector2f, sf::Vector2f);

	virtual void draw(sf::RenderWindow&) const override {};
	virtual void animateStaticObject(sf::Time) override {};
	virtual void createBody(int, int, int, b2World*, sf::Vector2f, sf::Vector2f = {15,1}) override;

	virtual sf::FloatRect getGlobalBounds() const override;
	virtual sf::Vector2f getPosition() const override;
	
protected:
	sf::RectangleShape m_structureRect; //the structure's shape
};