//----------------------------------include section-----------------------------------
#pragma once
#include <SFML/Graphics.hpp>

//----------------------------------enum class definition-----------------------------
enum class Direction
{
    Up,
    Down,
    Right,
    Left,
    Stay,
    Size,
};

enum class EnemyAction
{
	Idle,					
    Walk,                   
	Attack,                  
    Hit                     
};

enum class WeaponAction
{
	Fly,					
	Explode,                
};

enum class rubyAction
{
	Shine,                  
};

//----------------------------------functions definition------------------------------
Direction opposite(Direction dir);
sf::Vector2f toVector(Direction dir);