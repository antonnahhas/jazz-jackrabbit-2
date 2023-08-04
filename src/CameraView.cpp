//----------------------------------include section-----------------------------------
#include "CameraView.h"
#include "macros.h"
#include <iostream>
#include <Jazz.h>

//----------------------------------const section-----------------------------------
const float CAMERA_LEFT = 0.f,
CAMERA_TOP = 300.f,
CAMERA_WIDTH = 700.f,
CAMERA_HEIGHT = 380.f;

//----------------------------------class implementation------------------------------

// constructor - initialize all the members
CameraView::CameraView() : 
	m_view(sf::FloatRect(CAMERA_LEFT, CAMERA_TOP, CAMERA_WIDTH, CAMERA_HEIGHT)) {}

//----------------------------------functions section---------------------------------

// update the view according to the player's position, make sure not to leave the map
void CameraView::updateView(sf::Vector2f jazzPos, int level)
{
	//specific camera views of both levels
	switch (level)
	{
	case LVL1:
	{
		if (jazzPos.x <= 356.696)
			jazzPos.x = 356.696;
		else if (jazzPos.x >= 4421.59)
			jazzPos.x = 4421.59;
		if (jazzPos.y <= 278.421)
			jazzPos.y = 278.421;
		else if (jazzPos.y >= 491.611)
			jazzPos.y = 491.611;
	}
	break;
	case LVL2:
	{
		if (jazzPos.x <= 356.106)
			jazzPos.x = 356.106;
		else if (jazzPos.x >= 1911.03)
			jazzPos.x = 1911.03;
		if (jazzPos.y <= 205.944)
			jazzPos.y = 205.944;
		else if (jazzPos.y >= 947.325)
			jazzPos.y = 947.325;
	}
	break;
	}


	m_view.setCenter(jazzPos);
}

void CameraView::setView(sf::RenderWindow& window) const { window.setView(m_view); }

sf::View CameraView::getView() const { return m_view; }