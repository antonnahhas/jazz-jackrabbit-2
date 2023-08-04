//----------------------------------include section---------------------------------
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "AnimationData.h"

//---------------------------------class implementation-----------------------------
//Resources is a Singelton pattern
class Resources
{
public:
	enum Objects
	{
		Jazz,
		AmmoCon,
		LifeCon,
		Ruby,
		MeleeEnemy,
		RangedEnemy,
		SentryEnemy,
		Wall,
		Floor,
		Rope,
		Spikes,
		CheckPoint,
		FinishLine,
		BombCon,
		BulletWeapon,
		BombWeapon,
		CoffeeWeapon,
		SmokeWeapon,
		Size
	};

	enum Textures
	{
		t_jazz,
		t_items,
		t_enemies,
		t_effects,
		t_level_1,
		t_level_2,
		t_menu,
		t_logo,
		t_nameTag,
		t_help,
		t_unmute,
		t_mute,
		t_bomb,
		t_ammo,
		t_score,
		t_life,
		t_size
	};

	enum Fonts
	{
		f_font,
		f_size
	};

	enum Sounds
	{
		s_click,
		s_collectRuby,
		s_reachedCheckpoint,
		s_extraLife,
		s_shooting,
		s_levelComplete,
		s_collectWeapon,
		s_enemyDis,
		s_throwBomb,
		s_resetToCheckpoint,
		s_jump,
		s_ingameMusic,
		s_menuMusic,
		s_end
	};

	~Resources();
	static Resources& instance();

	void playMusic(int);
	bool isBgMusicPlaying() const;
	void switchBgMusic();
	void playSound(int);
	const AnimationData& animationData(int object) { return m_data[object]; }

	sf::Image& getCursorImg();
	sf::Font& getFont(int);
	std::ifstream& getObjectsPosFile();
	std::fstream& getHighScoresFile();
	const sf::Texture& getTexture(int);
	void configureText(sf::Text&, sf::Color, sf::Color) const;

	sf::Text setText(unsigned int, sf::Vector2f, sf::Color, std::string, int, int) const;
	sf::RectangleShape createRec(sf::Vector2f, sf::Vector2f);
private:
	Resources(); 
	void loadTextures();
	void loadAudio();
	void loadFile();
	void loadFont();
	void loadAnimationData();

	std::ifstream					m_animationFile, // file that holds all the animations
		m_objectsFile;	 // file that holds all the objects locations
	std::fstream					m_highScoresFile;// file that holds all high scores
	std::vector <sf::Font>			m_font;			 // saves the font that will be shown in the game
	std::vector <sf::Texture>		m_textures;		 // vector for all textures
	std::vector<AnimationData>		m_data;			 // the animation data for each object
	sf::Image m_cursorImg;							 // saves the image to be shown as the cursor in the game

	sf::Music						m_ingameMusic,		 // saves the background music
									m_menuMusic;
	std::vector <sf::Sound>			m_sounds;		 // saves all sounds
	std::vector <sf::SoundBuffer>	m_soundBuffer;	 // saves all sound buffers
};