//---------------------------------- include section------------------------------

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "MainMenu.h"
#include "Resources.h"


const int MAX_LETTERS = 128;

//---------------------------------- functions section------------------------------

MainMenu::MainMenu()
{
	m_feature.updateMenuFeatures();
}

void MainMenu::openMenu()
{
	Resources::instance().playMusic(Resources::s_menuMusic);
	sf::RenderWindow window(sf::VideoMode(1700, 900), "Jazz", sf::Style::Titlebar | sf::Style::Close);

	sf::Cursor cursor;
	if (cursor.loadFromPixels(Resources::instance().getCursorImg().getPixelsPtr(), Resources::instance().getCursorImg().getSize(), sf::Vector2u({ 26,16 })))
		window.setMouseCursor(cursor);

	while (window.isOpen())
	{
		// draw the menus background and its buttons
		drawMenuButtons(window);

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::MouseMoved)
			{
				// convert the current mouse position in the window to world coordinates
				sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				setOutline(m_feature._buttons[NEW_GAME], worldPos);
				setOutline(m_feature._buttons[SCORE_BOARD], worldPos);
				setOutline(m_feature._buttons[HELP], worldPos);
				setOutline(m_feature._buttons[EXIT], worldPos);
			}

			// Window closed or escape key pressed: exit
			if ((event.type == sf::Event::Closed) ||
				((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				window.close();
				exit(EXIT_SUCCESS);
			}
			// pressed on the window
			if (event.type == sf::Event::MouseButtonReleased)
			{
				// save the click location
				auto clickLocation = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
				// pressed on "new game" return true
				if (m_feature._buttons[NEW_GAME].getGlobalBounds().contains(clickLocation))
				{ 
					Resources::instance().playSound(Resources::s_click);
					bool pressedSave = false;
					openReceivePlayerName(window, pressedSave);
					if (pressedSave)
						return;
				}
				// pressed on "exit game" return false
				else if (m_feature._buttons[EXIT].getGlobalBounds().contains(clickLocation))
				{
					Resources::instance().playSound(Resources::s_click);
					window.close();
					exit(EXIT_SUCCESS);
				}
				else if (m_feature._buttons[SCORE_BOARD].getGlobalBounds().contains(clickLocation))
				{
					Resources::instance().playSound(Resources::s_click);
					openHighScoresBoard(window);
				}
				else if (m_feature._buttons[HELP].getGlobalBounds().contains(clickLocation))
				{
					Resources::instance().playSound(Resources::s_click);
					openHelpBoard(window);
				}
			}
		}
		window.display();
	}

}

void MainMenu::addScore(int score)
{
	// add player to high score board 
	m_highScoresBoard.addScore(m_playersName,score);
}

void MainMenu::openHighScoresBoard(sf::RenderWindow& window)
{
	Resources& ins = Resources::instance();
	std::vector < sf::Text > allScores;
	sf::Text nameT, scoreT;
	for (int i = 0; i < m_highScoresBoard.getScores().size(); i++)
	{
		std::string  name = m_highScoresBoard.getScores()[i].playerName;
		std::string score = std::to_string(m_highScoresBoard.getScores()[i].playerScore);

		nameT = ins.setText(40, { 300, (float)(250 + 50 * i) }, sf::Color::Yellow, name, 0 ,Resources::f_font);
		scoreT = ins.setText(40, { 750, (float)(250 + 50 * i) }, sf::Color::Yellow, score,0, Resources::f_font);
		allScores.push_back(nameT);
		allScores.push_back(scoreT);
	}
	sf::Event event;
	sf::RectangleShape rec = Resources::instance().createRec({ 240, 130 }, { 820, 650 });

	while (window.isOpen())
	{
		window.pollEvent(event);
		drawHighScoreButtons(window, rec);
		for (int i = 0; i < allScores.size(); i++)
			window.draw(allScores[i]);

		if (event.type == sf::Event::MouseMoved)
		{
			// convert the current mouse position in the window to world coordinates
			sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			setOutline(m_feature._buttons[BACK], worldPos);
		}
		if ((event.type == sf::Event::Closed) ||
			((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
		{
			window.close();
			exit(EXIT_SUCCESS);
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			auto clickLocation = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
			if (m_feature._buttons[BACK].getGlobalBounds().contains(clickLocation))
			{
				Resources::instance().playSound(Resources::s_click);
				return; 
			}
		}
		window.display();
	}
}

void MainMenu::openHelpBoard(sf::RenderWindow& window)
{
	sf::RectangleShape rec = Resources::instance().createRec({ 120, 100 }, { 1000,700 });	
	sf::Event event;
	while (window.isOpen())
	{
		window.pollEvent(event);
		drawHelpButtons(window, rec);
		if ((event.type == sf::Event::Closed) ||
			((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
		{
			window.close();
			exit(EXIT_SUCCESS);
		}
		if (event.type == sf::Event::MouseMoved)
		{
			// convert the current mouse position in the window to world coordinates
			sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			setOutline(m_feature._buttons[BACK], worldPos);
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			auto clickLocation = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
			if (m_feature._buttons[BACK].getGlobalBounds().contains(clickLocation))
			{
				Resources::instance().playSound(Resources::s_click);
				return;
			}
		}
		window.display();
	}
}

//opens a window to receive the player's name
void MainMenu::openReceivePlayerName(sf::RenderWindow& window, bool& pressedSave)
{
	sf::RectangleShape rec = Resources::instance().createRec({ 250, 300 }, { 930, 210 });

	Resources& ins = Resources::instance();

	sf::Text name;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			drawPlayerNameButtons(window, rec, name);

			if ((event.type == sf::Event::Closed) ||
				((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
			{
				window.close();
				exit(EXIT_SUCCESS);
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				auto clickLocation = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
				if (m_feature._buttons[BACK].getGlobalBounds().contains(clickLocation))
				{
					Resources::instance().playSound(Resources::s_click);
					return;
				}
				if (m_feature._buttons[SAVE].getGlobalBounds().contains(clickLocation))
				{
					Resources::instance().playSound(Resources::s_click);
					pressedSave = true;
					window.close();
					return;
				}
			}
			if (event.type == sf::Event::MouseMoved)
			{
				// convert the current mouse position in the window to world coordinates
				sf::Vector2f worldPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				setOutline(m_feature._buttons[SAVE], worldPos);
				setOutline(m_feature._buttons[BACK], worldPos);
			}
			if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode < MAX_LETTERS)
				{
					if (event.text.unicode == BACKSPACE) // backspace
						m_playersName = m_playersName.substr(0, m_playersName.size() - 1);
					else if (m_playersName.size() <= 10)
							m_playersName += static_cast<char>(event.text.unicode);

					name = ins.setText(65, { 320, 390 }, sf::Color::Yellow, m_playersName, 0, Resources::f_font);
				}
			}
			window.display();
		}
	}
}

void MainMenu::drawMenuButtons(sf::RenderWindow& window)
{
	window.draw(m_feature._bg);
	window.draw(m_feature._buttons[NEW_GAME]);
	window.draw(m_feature._buttons[SCORE_BOARD]);
	window.draw(m_feature._buttons[HELP]);
	window.draw(m_feature._buttons[EXIT]);
	window.draw(m_feature._logo);

}

void MainMenu::drawHighScoreButtons(sf::RenderWindow& window, sf::RectangleShape rec)
{
	window.clear();
	window.draw(m_feature._bg);
	window.draw(rec);
	window.draw(m_feature._buttons[BACK]);
	window.draw(m_feature._buttons[NAME]);
	window.draw(m_feature._buttons[SCOR]);
}

void MainMenu::drawHelpButtons(sf::RenderWindow& window, sf::RectangleShape rec)
{
	window.clear();
	window.draw(m_feature._bg);
	window.draw(rec);
	window.draw(m_feature._buttons[BACK]);
	window.draw(m_feature._help);

}

void MainMenu::drawPlayerNameButtons(sf::RenderWindow& window, sf::RectangleShape rec, sf::Text& name)
{
	window.clear();
	window.draw(m_feature._bg);
	window.draw(rec);
	window.draw(m_feature._logo);
	window.draw(m_feature._nameTag);
	window.draw(name);
	window.draw(m_feature._buttons[ENTER]);
	window.draw(m_feature._buttons[SAVE]);
	window.draw(m_feature._buttons[BACK]);
}

//----------------------------------set functions section------------------------------

// if the cursor is on txt then change its outline
void MainMenu::setOutline(sf::Text& txt, sf::Vector2f worldPos) const
{
	// if the cursor is on txt then change its outline to be thick and colorful
	if (txt.getGlobalBounds().contains(worldPos))
		setOutlineWhenCursorMoved(txt, sf::Color(136, 82, 9));
	else
		setOutlineWhenCursorMoved(txt, sf::Color::Black);
}

void MainMenu::setOutlineWhenCursorMoved(sf::Text& txt, sf::Color color) const
{
	txt.setOutlineColor(color);
}
