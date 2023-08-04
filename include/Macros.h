#pragma once

#include <SFML/Graphics.hpp>

const int INC = 1,
DEC = -1,
INC_RUBY = 10,
LIVES_DEFAULT = 5,
AMMO_INIT = 10,
BOMB_INIT = 10,
SCORE_INIT = 0,
LVL1 = 1,
LVL2 = 2,
INIT = 0;



const sf::Vector2f  DOWN_VEC = { 0,1 },
UP_VEC = { 0,-1 },
RIGHT_VEC = { 1,0 },
LEFT_VEC = { -1,0 },
STAY_VEC = { 0,0 },
LEFT_SPRITE_TURN = { -1, 1 },
RIGHT_SPRITE_TURN = { 1, 1 };

enum PlayerFeatures
{
	SCORE,
	LIVES,
	AMMO,
	BOMBS,
};

struct PlayerData
{
	int _score,
		_lives,
		_ammo,
		_bombs;
	float _speed;
};