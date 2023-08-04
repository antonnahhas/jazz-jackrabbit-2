//----------------------------------include section-------------------------------------

#include <string.h>
#include "ScoreBoard.h"

//----------------------------------include section-------------------------------------

const float MUTE_SCALE = 0.3;
const float UNMUTE_SCALE = 0.3;
const sf::Vector2f AMMO_WE_SCALE = { 2,2 };
const sf::Vector2f BOMB_WE_SCALE = { 2,2 };
const std::string COLON = ":", SPACE = " ", P_SCORE = "SCORE: ";
//----------------------------------class implementation------------------------------

ScoreBoard::ScoreBoard()
{
	configureTextMembers();
	configureSpriteMembers();
}

//----------------------------------functions section---------------------------------

void ScoreBoard::drawScoreBoard(sf::RenderWindow& window) const
{
	window.draw(m_textScore);
	window.draw(m_textLife);
	window.draw(m_textAmmo);
	window.draw(m_textBomb);
	window.draw(m_scoreTxr);
	window.draw(m_lifeTxr);
	window.draw(m_ammoTxr);
	window.draw(m_bombTxr);
	window.draw(m_unmuteTxr);
	if(!Resources::instance().isBgMusicPlaying())
		window.draw(m_muteTxr);
}

void ScoreBoard::configureTextMembers()
{
	Resources& ins = Resources::instance();

	ins.configureText(m_textScore, sf::Color::Yellow, sf::Color::Black);
	ins.configureText(m_textLife, sf::Color::Yellow, sf::Color::Black);
	ins.configureText(m_textAmmo, sf::Color::Yellow, sf::Color::Black);
	ins.configureText(m_textBomb, sf::Color::Yellow, sf::Color::Black);
}

void ScoreBoard::configureSpriteMembers()
{

	//m_lifeTxr.setTextureRect(sf::IntRect(343, 171, 19, 24));
	m_scoreTxr.setTexture(Resources::instance().getTexture(Resources::t_score));
	//m_lifeTxr.setTextureRect(sf::IntRect(343, 171, 19, 24));
	m_lifeTxr.setTexture(Resources::instance().getTexture(Resources::t_life));
	//m_lifeTxr.setTextureRect(sf::IntRect(343, 171, 19, 24));
	m_ammoTxr.setTexture(Resources::instance().getTexture(Resources::t_ammo));
	//m_ammoTxr.setTextureRect(sf::IntRect(341, 17, 11, 12));
	//m_ammoTxr.setScale(AMMO_WE_SCALE);
	m_bombTxr.setTexture(Resources::instance().getTexture(Resources::t_bomb));
	//m_bombTxr.setTextureRect(sf::IntRect(341, 17, 11, 12));
	//m_bombTxr.setScale(BOMB_WE_SCALE);

	m_unmuteTxr.setTexture(Resources::instance().getTexture(Resources::t_unmute));
	m_muteTxr.setTexture(Resources::instance().getTexture(Resources::t_mute));
	m_muteTxr.setScale(MUTE_SCALE, MUTE_SCALE);

}

void ScoreBoard::updatePosition(sf::View view)
{
	sf::Vector2f size = view.getSize();
	sf::Vector2f center = view.getCenter();
	
	//specific calculation according to the camera view
	m_scoreTxr.setPosition(center.x - (size.x / 2) + 10.f, center.y - (size.y / 2) + 25.f);
	m_textScore.setPosition(center.x - (size.x / 2) + 40.f, center.y - (size.y / 2) + 7.f);
	
	m_lifeTxr.setPosition(center.x - (size.x / 2) + 190.f, center.y - (size.y / 2) + 25.f);
	m_textLife.setPosition(center.x - (size.x / 2) + 220.f, center.y - (size.y / 2) + 7.f);

	m_ammoTxr.setPosition(center.x - (size.x / 2) + 320.f, center.y - (size.y / 2) + 25.f);
	m_textAmmo.setPosition(center.x - (size.x / 2) + 340.f, center.y - (size.y / 2) + 7.f);
	
	m_bombTxr.setPosition(center.x - (size.x / 2) + 460.f, center.y - (size.y / 2) + 25.f);
	m_textBomb.setPosition(center.x - (size.x / 2) + 490.f, center.y - (size.y / 2) + 7.f);
	
	m_unmuteTxr.setPosition(center.x - (size.x / 2) + 630.f, center.y - (size.y / 2) + 20.f);
	m_muteTxr.setPosition(center.x - (size.x / 2) + 630.f, center.y - (size.y / 2) + 25.f);
}

void ScoreBoard::updateText(const PlayerData& data)
{
	m_textScore.setString(COLON + std::to_string(data._score));
	m_textLife.setString(COLON + std::to_string(data._lives));
	m_textAmmo.setString(COLON + std::to_string(data._ammo));
	m_textBomb.setString(COLON + std::to_string(data._bombs));
}

void ScoreBoard::updateScoreBoard(sf::View view, const PlayerData& data)
{
	updatePosition(view);
	updateText(data);
}

sf::FloatRect ScoreBoard::getPlayButtonGloBu() const
{
	return m_unmuteTxr.getGlobalBounds();
}
