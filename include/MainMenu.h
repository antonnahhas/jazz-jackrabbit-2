//----------------------------------include section---------------------------------

#pragma once
#include <SFML/Graphics.hpp>

#include "HighScoreBoard.h"
#include "Resources.h"

//----------------------------------include section---------------------------------
const std::string NEW_GAME = "New Game", SCORE_BOARD = "Score Board", HELP = "Help", EXIT = "Exit Game",
BACK = "back", SAVE = "Save", NAME = "Name", SCOR = "Score", ENTER = "Enter Name", ENTER_Q = "Please enter your name:";
const int BACKSPACE = 8;
				
//----------------------------------struct definition---------------------------------

// Struct to represent the menu's features
struct MenuFeature
{
	sf::Sprite _bg = sf::Sprite(Resources::instance().getTexture(Resources::t_menu));
	sf::Sprite _logo = sf::Sprite(Resources::instance().getTexture(Resources::t_logo));
	sf::Sprite _help = sf::Sprite(Resources::instance().getTexture(Resources::t_help));
	sf::Sprite _nameTag = sf::Sprite(Resources::instance().getTexture(Resources::t_nameTag));
	sf::Cursor _cursor;
	std::map<std::string, sf::Text> _buttons; // Map to represent possible buttons in the game

	void updateMenuFeatures()
	{
		Resources& ins = Resources::instance();

		_bg.setPosition(0.f, 0.f);
		_bg.setScale(1700.f / _bg.getLocalBounds().width, 900.f / _bg.getLocalBounds().height);
		_logo.setPosition(0.f, 0.f);
		_help.setScale({ 1.5f, 1.5f });
		_help.setPosition(160.f, 150.f);
		_nameTag.setPosition(290.f, 390.f);
		_nameTag.setScale({1.5, 1});

		auto yellow = sf::Color(245, 220, 5);
		_buttons[NEW_GAME] = ins.setText(75, { 450, 300 }, yellow, NEW_GAME, 4, Resources::f_font);
		_buttons[SCORE_BOARD] = ins.setText(75, { 430, 400 }, yellow, SCORE_BOARD, 4, Resources::f_font);
		_buttons[HELP] = ins.setText(75, { 570, 500 }, yellow, HELP, 4, Resources::f_font);
		_buttons[EXIT] = ins.setText(75, { 470, 600 }, yellow, EXIT, 4, Resources::f_font);
		_buttons[BACK] = ins.setText(50, { 50, 700 }, yellow, BACK, 3, Resources::f_font);
		_buttons[SAVE] = ins.setText(50, { 880, 400 }, yellow, SAVE, 3, Resources::f_font);
		_buttons[NAME] = ins.setText(70, { 300, 150 }, sf::Color::Yellow, NAME, 0, Resources::f_font);
		_buttons[SCOR] = ins.setText(70, { 750, 150 }, sf::Color::Yellow, SCOR, 0, Resources::f_font);
		_buttons[ENTER] = ins.setText(50, { 300, 320 }, sf::Color::Yellow, ENTER_Q, 0, Resources::f_font);
	}
};

//----------------------------------class definition---------------------------------
class MainMenu
{
public:
	MainMenu();
	~MainMenu() = default;

	void openMenu();
	void addScore(int);
	
private:
	void openHighScoresBoard(sf::RenderWindow&);
	void openHelpBoard(sf::RenderWindow&);
	void openReceivePlayerName(sf::RenderWindow& ,bool&);
	void drawMenuButtons(sf::RenderWindow&);
	void drawHighScoreButtons(sf::RenderWindow&, sf::RectangleShape);
	void drawHelpButtons(sf::RenderWindow&, sf::RectangleShape);
	void drawPlayerNameButtons(sf::RenderWindow&, sf::RectangleShape, sf::Text&);
	void setOutlineWhenCursorMoved(sf::Text&, sf::Color) const;
	void setOutline(sf::Text&, sf::Vector2f) const;
	
	MenuFeature m_feature;
	HighScoreBoard m_highScoresBoard; // the high score table to be shown on screen
	std::string m_playersName = "";	  // the current player's name to be shown in the high score table
};