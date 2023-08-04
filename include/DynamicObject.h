#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "box2d/box2d.h"
#include "GameObject.h"
#include "Direction.h"

using std::unique_ptr;
using std::shared_ptr;
using std::make_unique;
using std::make_shared;

class DynamicObject : public GameObject
{
public:
	DynamicObject(sf::Vector2f, const AnimationData&);

	virtual void setAction() = 0;
	virtual void updateAction(const sf::Time&) = 0;
	virtual void createBody(int , int , int , b2World*, sf::Vector2f, sf::Vector2f = { 15,1 }) override;

	bool isThrowingWeapon() const;
	virtual Direction getDirection() const;
	void setDirection(Direction);
	void setThrowingWeapon(bool throwing);

protected:
	virtual void updateDynamicObject(sf::Time, int);

	Animation       m_animation;  // current animation of the object
	Direction		m_dir = Direction::Stay;  // current direction of the object
	bool			m_activateWeapon = false; // if the object is currently throwing a weapon
	sf::Time		m_elapsed = {};     // the time that passed since the last time we checked
};