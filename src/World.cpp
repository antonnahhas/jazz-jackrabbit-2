//----------------------------------include section-----------------------------------
#include <experimental/vector>
#include <iostream>
#include <typeinfo>

#include "World.h"										
#include "Resources.h"					
#include "Jazz.h"				
#include "AmmoCon.h"					
#include "BombCon.h"	
#include "Ruby.h"					
#include "CheckPoint.h"					
#include "LifeCon.h"				
#include "Floor.h"					
#include "BombWeapon.h"					
#include "BulletWeapon.h"
#include "CoffeeWeapon.h"
#include "SmokeWeapon.h"
#include "Wall.h"					
#include "Rope.h"					
#include "Enemy.h"					
#include "CollisionHandling.h"		
#include "MeleeEnemy.h"				
#include "RangedEnemy.h"			
#include "SentryEnemy.h"			

//----------------------------------const section-------------------------------------
const int32 m_velocityIterations = 6, // velocity of the world
			m_positionIterations = 2; // position iterations of the world
const float m_timeStep = 1.0f / 60.0,// timeStep of the world
			s_lvl1 = 35.f, s_lvl2 = 30.f;
const b2Vec2 gravity = b2Vec2(0.0f, 10.0f);
const sf::Vector2f initVec = sf::Vector2f(0.f, 0.f);
const int OFFSET = 37;

//----------------------------------class implementation------------------------------

World::World() : m_level(INIT), m_box2dWorld(gravity), m_jazz(initVec), m_finishLine(initVec),
	m_levelFile(Resources::instance().getObjectsPosFile()) {}

World::~World()
{
	m_finishLine.destroyBody();
	m_jazz.destroyBody();
	destroyAllBodies();
}

//----------------------------------functions section---------------------------------

void World::readLevel()
{	
	if (m_levelFile.eof()) //read the game until reaching the end of the file
	{
		m_gameFinished = true;
		return;
	}
	m_level++;
	updateLevelBackground();
	resetObjects();
	int numOfObjects;
	m_levelFile >> numOfObjects; //read how many objects there are in the game

	int objectId;
	sf::Vector2f pos,
				limit;
	for (int i = 0; i < numOfObjects; i++)
	{
		m_levelFile >> objectId >> pos.x >> pos.y;
		switch (objectId)
		{
		case Resources::Jazz:
		{
			if (m_jazz.getBody() == nullptr)
				m_jazz.createBody(0, 0, 0,&m_box2dWorld, pos);
			m_jazz.setDefaultPosition(pos);
			m_jazz.setJazzToDefaultPosition();
			break;
		}
		case Resources::MeleeEnemy:
		{
			m_levelFile >> limit.x >> limit.y;
			m_dynamicObject.emplace_back(std::make_unique<MeleeEnemy>(pos, m_jazz, limit));
			m_dynamicObject[m_dynamicObject.size() - 1]->createBody(0, 0, 0, &m_box2dWorld, pos);
			break;
		}
		case Resources::RangedEnemy:
		{
			m_levelFile >> limit.x >> limit.y;
			m_dynamicObject.emplace_back(std::make_unique<RangedEnemy>(pos, m_jazz, limit));
			m_dynamicObject[m_dynamicObject.size() - 1]->createBody(0, 0, 0, &m_box2dWorld, pos);
			break;
		}
		case Resources::SentryEnemy:
		{
			m_dynamicObject.emplace_back(std::make_unique<SentryEnemy>(pos, m_jazz));
			m_dynamicObject[m_dynamicObject.size() - 1]->createBody(0, 0, 0, &m_box2dWorld, pos);
			break;
		}

		case Resources::AmmoCon:
			m_staticObjects.emplace_back(std::make_unique<AmmoCon>(pos)); break;
		case Resources::BombCon:
			m_staticObjects.emplace_back(std::make_unique<BombCon>(pos)); break;
		case Resources::Ruby:
			m_staticObjects.emplace_back(std::make_unique<Ruby>(pos)); break;
		case Resources::CheckPoint:
			m_staticObjects.emplace_back(std::make_unique<CheckPoint>(pos)); break;
		case Resources::LifeCon:
			m_staticObjects.emplace_back(std::make_unique<LifeCon>(pos)); break;
		case Resources::FinishLine:
			m_finishLine.setPosition(pos); break;

		case Resources::Floor: case Resources::Wall: case Resources::Rope:
		{
			sf::Vector2f size;
			m_levelFile >> size.x >> size.y; //read the object's size as well

			if (objectId == Resources::Rope)
			{
				m_staticObjects.emplace_back(std::make_unique<Rope>(pos, size));
				break;
			}

			switch (objectId)
			{
			case Resources::Floor:
				m_staticObjects.emplace_back(std::make_unique<Floor>(pos, size));
				m_staticObjects[m_staticObjects.size() - 1]->createBody(0, 0, 0, &m_box2dWorld, pos, size);
				break;
			case Resources::Wall:
				m_staticObjects.emplace_back(std::make_unique<Wall>(pos, size));
				m_staticObjects[m_staticObjects.size() - 1]->createBody(0, 0, 0, &m_box2dWorld, pos, size);
				break;
			}
			break;
		}
		}
	}
}

//change background images according to the level, and set jazzs speed
void World::updateLevelBackground()
{
	switch (m_level)
	{
	case LVL1:
		m_LevelTxr = sf::Sprite(Resources::instance().getTexture(Resources::t_level_1));
		m_jazz.setSpeed(s_lvl1);
		break;
	case LVL2:
		m_LevelTxr = sf::Sprite(Resources::instance().getTexture(Resources::t_level_2));
		m_jazz.setSpeed(s_lvl2);
		break;
	}
}

void World::stepBox2dWorld()
{
	m_box2dWorld.Step(m_timeStep, m_velocityIterations, m_positionIterations);
}

//print all the game objects and background to the screen
void World::printWorld(sf::RenderWindow& window) const
{
	window.draw(m_LevelTxr);
	for (auto& staticObjects : m_staticObjects)
		staticObjects->draw(window);

	for (auto& npc : m_dynamicObject)
		npc->draw(window);

	for (auto& weapon : m_weapons)
		weapon->draw(window);

	m_jazz.draw(window);
	m_finishLine.draw(window);
	m_scoreBoard.drawScoreBoard(window);
}

//update all objects' actions and locations
void World::updateObject(const sf::Time& deltaTime)
{
	m_jazz.updateAction(deltaTime);
	m_worldView.updateView(m_jazz.getPosition(), m_level);
	checkGameStatus();

	if (m_jazz.isBombWeapon())
	{
		addWeapon(Resources::Objects::BombWeapon, m_jazz.getPosition(), m_jazz.getDirection());
		m_jazz.setBombWeapon(false);
	}
	else if (m_jazz.isBulletWeapon())
	{
		addWeapon(Resources::Objects::BulletWeapon, m_jazz.getPosition(), m_jazz.getDirection());
		m_jazz.setBulletWeapon(false);
	}

	for (auto& dynamicObject : m_dynamicObject)
	{
		dynamicObject->updateAction(deltaTime);
		if (dynamicObject->isThrowingWeapon())
		{
			auto& p = typeid(*dynamicObject);

			if (p == typeid(RangedEnemy))
				addWeapon(Resources::Objects::CoffeeWeapon, dynamicObject->getPosition(), dynamicObject->getDirection());
			else if (p == typeid(SentryEnemy))
				addWeapon(Resources::Objects::SmokeWeapon, dynamicObject->getPosition(), dynamicObject->getDirection());
			
			dynamicObject->setThrowingWeapon(false);
		}
	}

	for (auto& weapon : m_weapons)
		weapon->updateAction(deltaTime);

	for (auto& staticObjects : m_staticObjects)
		staticObjects->animateStaticObject(deltaTime);

	m_scoreBoard.updateScoreBoard(m_worldView.getView(), m_jazz.getJazzData());
	handleCollision();
	stepBox2dWorld();
}

//check all possible collisions
void World::handleCollision()
{
	for (auto& staticObjects : m_staticObjects)
	{
		if (m_jazz.checkCollision(staticObjects->getGlobalBounds()))
			processCollision(m_jazz, *staticObjects);
		for (auto& dynamicObject : m_dynamicObject)
			if (dynamicObject->checkCollision(staticObjects->getGlobalBounds()))
				processCollision(*dynamicObject, *staticObjects);

		for (auto& weapon : m_weapons)
		{
			if (weapon->checkCollision(staticObjects->getGlobalBounds()))
			{
				processCollision(*weapon, *staticObjects);
				removeBodyIfDisposed(weapon.get());
			}
		}
	}
	for (auto& dynamicObject : m_dynamicObject)
	{
		if (m_jazz.checkCollision(dynamicObject->getGlobalBounds()))
		{
			processCollision(m_jazz, *dynamicObject);
			removeBodyIfDisposed(dynamicObject.get());
		}
	}
	for (auto& dynamicObject : m_dynamicObject)
	{
		for (auto& weapon : m_weapons)
		{
			if (weapon->checkCollision(dynamicObject->getGlobalBounds()))
			{
				processCollision(*weapon, *dynamicObject);
				removeBodyIfDisposed(dynamicObject.get());
				removeBodyIfDisposed(weapon.get());
			}
		}
	}
	for (auto& weapon : m_weapons)
	{
		for (auto& weapon2 : m_weapons)
		{
			if (weapon->checkCollision(weapon2->getGlobalBounds()))
			{
				processCollision(*weapon, *weapon2);
				removeBodyIfDisposed(weapon.get());
				removeBodyIfDisposed(weapon2.get());
			}
		}
		if (m_jazz.checkCollision(weapon->getGlobalBounds()))
		{
			processCollision(m_jazz, *weapon);
			removeBodyIfDisposed(weapon.get());
		}
	}
	//erase from the objects vectors the objects that are disposed
	std::experimental::erase_if(m_dynamicObject, [](const auto& item) { return item->isDisposed(); });
	std::experimental::erase_if(m_staticObjects, [](const auto& item) { return item->isDisposed(); });
	std::experimental::erase_if(m_weapons, [](const auto& item) { return item->isDisposed(); });

}

//delete all box2d bodies when ending a level
void World::destroyAllBodies()
{
	if (m_box2dWorld.GetBodyCount() > INIT)
	{
		for (auto& dynamicObject : m_dynamicObject)
			if (dynamicObject->getBody() != nullptr)
				m_box2dWorld.DestroyBody(dynamicObject->getBody());

		for (auto& staticObjects : m_staticObjects)
			if (staticObjects->getBody() != nullptr)
				m_box2dWorld.DestroyBody(staticObjects->getBody());

		for (auto& weapon : m_weapons)
			if (weapon->getBody() != nullptr)
				m_box2dWorld.DestroyBody(weapon->getBody());
	}
}

//reset all objects vectors and bodies
void World::resetObjects()
{
	destroyAllBodies(); // delete all m_box2d bodies
	m_staticObjects.clear();
	m_dynamicObject.clear();
	m_weapons.clear();
}

//if an object was disposed, remove his box2d body
void World::removeBodyIfDisposed(GameObject* gameObject)
{
	if (gameObject->getBody() && gameObject->isDisposed())
	{
		m_box2dWorld.DestroyBody(gameObject->getBody());
		gameObject->destroyBody();
	}
}

//check the current game data to know how to continue the game
void World::checkGameStatus()
{
	if (m_jazz.getJazzData()._lives == INIT)
		m_gameFinished = true;

	if (m_jazz.checkCollision(m_finishLine.getGlobalBounds()))
		processCollision(m_jazz, m_finishLine);

	if (m_finishLine.isDisposed())
	{
		m_finishLine.setDisposed(false);
		readLevel();
		return;
	}
}

void World::addWeapon(Resources::Objects obj, sf::Vector2f pos, Direction dir)
{
	pos.y -= OFFSET;
	switch (obj)
	{
	case Resources::SmokeWeapon:
		m_weapons.emplace_back(std::make_unique<SmokeWeapon>(pos, dir));
		m_weapons[m_weapons.size() - 1]->createBody(10, 1, 0, &m_box2dWorld, pos);
		break;
	case Resources::CoffeeWeapon:
		m_weapons.emplace_back(std::make_unique<CoffeeWeapon>(pos, dir));
		m_weapons[m_weapons.size() - 1]->createBody(20, 1, 0, &m_box2dWorld, pos);
		break;
	case Resources::BombWeapon:
		m_weapons.emplace_back(std::make_unique<BombWeapon>(pos, dir));
		m_weapons[m_weapons.size() - 1]->createBody(40, -20, 1, &m_box2dWorld, pos);
		break;
	case Resources::BulletWeapon:
		m_weapons.emplace_back(std::make_unique<BulletWeapon>(pos, dir));
		Resources::instance().playSound(Resources::s_shooting);
		m_weapons[m_weapons.size() - 1]->createBody(50, 1, 0, &m_box2dWorld, pos);
		break;
	default:
		return;
	}
}


//reset all the game data bedore starting a new level/game
void World::resetWorld()
{
	if (m_gameFinished)
	{
		m_jazz.resetJazzData();
		m_levelFile.clear();
		m_levelFile.seekg(INIT);
		m_level = INIT;
		m_finishLine.setPosition(initVec);
		m_gameFinished = false;
	}
	resetObjects();
	readLevel();
}

//----------------------------------set functions section---------------------------------

void World::setCameraView(sf::RenderWindow& window) const { m_worldView.setView(window); }

//----------------------------------get functions section---------------------------------

sf::FloatRect World::getPlayButtonGloBu() const { return m_scoreBoard.getPlayButtonGloBu(); }

sf::Vector2f World::getJazzPos() const { return m_jazz.getJazzPos(); }

bool World::getIsGameFinished() const { return m_gameFinished; }

int World::getPlayerScore() const { return m_jazz.getJazzData()._score; }