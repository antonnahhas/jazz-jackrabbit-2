//----------------------------------include section-----------------------------------
#pragma once
#include <SFML/Graphics.hpp>

//----------------------------------class definition----------------------------------
class CameraView
{
public:
	CameraView();
	~CameraView() = default;
	void updateView(sf::Vector2f, int);
	void setView(sf::RenderWindow&) const;
	sf::View getView()const;

private:
	sf::View m_view; //the current view of the game out of the whole background
};