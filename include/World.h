//----------------------------------include section-----------------------------------
#pragma once

#include <vector>
#include <iostream>
#include <memory>

#include "DynamicObject.h"
#include "StaticObject.h"
#include "FinishLine.h"		
#include "Jazz.h"		
#include "CameraView.h"		
#include "ScoreBoard.h"
#include "box2d/box2d.h"

//----------------------------------class definition----------------------------------
class World
{
public:
	World();
	~World();

	void printWorld(sf::RenderWindow&) const;
	void updateObject(const sf::Time&);
	void stepBox2dWorld();
	void resetWorld();

	sf::FloatRect getPlayButtonGloBu() const;
	void setCameraView(sf::RenderWindow&) const;
	bool getIsGameFinished() const;
	int getPlayerScore() const;
	sf::Vector2f getJazzPos() const;

private:
	void addWeapon(Resources::Objects, sf::Vector2f, Direction);
	void readLevel();
	void handleCollision();
	void destroyAllBodies();
	void updateLevelBackground();
	void resetObjects();
	void removeBodyIfDisposed(GameObject*);
	void checkGameStatus();

	int  m_level;				  // the current level number
	bool m_gameFinished = false,  // true if the game is over
		m_isNewLevel = false;    // true if we started a new level              	
	Jazz m_jazz;				// holds the main player
	CameraView m_worldView;         // current view out of the entire screen
	ScoreBoard m_scoreBoard;        // the score board of the current data of the game to be shown on screen
	std::ifstream& m_levelFile;     // the file that holds the data of the levels

	b2World m_box2dWorld;			// the box2D world to be attached to the game

	using uniqueDynamicVec = std::vector<std::unique_ptr<DynamicObject>>;
	using uniqueStaticVec = std::vector<std::unique_ptr<StaticObject>>;
	using uniqueWeaponsVec = std::vector<std::unique_ptr<Weapon>>;
	uniqueDynamicVec	m_dynamicObject;
	uniqueStaticVec	m_staticObjects;
	uniqueWeaponsVec	m_weapons;

	FinishLine m_finishLine;

	sf::Sprite m_LevelTxr;
};