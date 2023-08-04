//----------------------------------include section-----------------------------------
#pragma once

#include "World.h"
#include "MainMenu.h"

//----------------------------------class definition----------------------------------
class Manager
{
public:
	Manager() = default;
	~Manager() = default;
	void runGame();

private:

	sf::Time m_time; // the current time of the game
	World m_world;   // attaches the world of the game to the display on the screen
	MainMenu m_menu;	// the menu of the game

};