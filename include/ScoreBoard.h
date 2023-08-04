//----------------------------------include section-----------------------------------
#pragma once
#include "macros.h"
#include <SFML/Graphics.hpp>
#include "Resources.h"

//----------------------------------class definition----------------------------------
class ScoreBoard 
{
public:
	ScoreBoard();
	~ScoreBoard() = default;
	void drawScoreBoard(sf::RenderWindow& window) const;
	void updateScoreBoard(sf::View view, const PlayerData& data);
	sf::FloatRect getPlayButtonGloBu() const;

private:
	void configureTextMembers();
	void configureSpriteMembers();
	void updatePosition(sf::View view);
	void updateText(const PlayerData& data);

	sf::Text	m_textScore,    // the text that represents the score on screen
				m_textLife,	// the text that represents the lives on screen
				m_textAmmo,		// the text that represents the num of ammo on screen
				m_textBomb;		// the text that represents the num of bombs on screen
	
	sf::Sprite  m_scoreTxr,
				m_ammoTxr,		// the Weapon sprite to be displayed on screen
				m_bombTxr,
				m_lifeTxr,		// the hearts sprite to be displayed on screen
				m_unmuteTxr,
				m_muteTxr;			// represent the play and mute buttons

};