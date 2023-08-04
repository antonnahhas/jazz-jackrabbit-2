//----------------------------------include section---------------------------------
#include "Resources.h"
#include "macros.h"
#include <stdexcept>

//----------------------------------const section---------------------------------
const int VOL = 15, PLAY = 2;
const std::string   TXTURE_ERR = "Failed to load texture\nPress any key to continue...",
					WORLD_OBJ_ERR = "File worldObjectsPos.txt could not open\nPress any key to continue...",
					HIGH_SCORE_ERR = "File high_score_table.txt could not open\nPress any key to continue...",
					FONT_ERR = "Failed to load font\nPress any key to continue...";

//----------------------------------namespace section---------------------------------
namespace
{
	AnimationData readAnimation(std::string fileName)
		// get a file name that contain the coordinate of the animmation;
	{
		auto file = std::ifstream(fileName);
		int numAnime,				// num of the animation in the file
			numOfFrame;				// coordinate for single frame 

		file >> numAnime;
		auto animations = AnimationData{};

		for (int animationKey = 0; animationKey < numAnime; animationKey++)
		{
			file >> numOfFrame;
			sf::Vector2i size,			// x for width, y for heigh
				currentStart;			// coordinate in the pictute
			sf::Vector2f originStart;

			for (int frameIndex = 0; frameIndex < numOfFrame; frameIndex++)
			{
				file >> currentStart.x >> currentStart.y >> size.x >> size.y
					>> originStart.x >> originStart.y;

				animations.m_animeMap[animationKey].emplace_back(currentStart, size);
				animations.m_originPoint[animationKey].emplace_back(originStart);
			}
		}
		return animations;
	}
}

//----------------------------------class implementation------------------------------

// constructor that opens all the files needed for the game
Resources::Resources()
{
	loadFont();
	loadTextures();
	loadAudio();
	loadFile();
	loadAnimationData();
}


// destructor that closes all the files 
Resources::~Resources()
{
	m_animationFile.close();
	m_objectsFile.close();
	m_highScoresFile.close();
}
//----------------------------------functions section---------------------------------

// load all the textures for the game and push it into m_textures
void Resources::loadTextures()
{
	m_cursorImg.loadFromFile("cursor.png");
	// push into Texture vector 
	m_textures.resize(t_size);
	if (!m_textures[t_jazz].loadFromFile("jazz.png") ||
		!m_textures[t_items].loadFromFile("items.png") ||
		!m_textures[t_effects].loadFromFile("effects.png") ||
		!m_textures[t_enemies].loadFromFile("enemies_1.png") ||
		!m_textures[t_level_1].loadFromFile("level_1.png") ||
		!m_textures[t_level_2].loadFromFile("level_2.png") ||
		!m_textures[t_menu].loadFromFile("menu.png") ||
		!m_textures[t_logo].loadFromFile("logo.png") ||
		!m_textures[t_nameTag].loadFromFile("name_tag.png") ||
		!m_textures[t_help].loadFromFile("help.png") ||
		!m_textures[t_mute].loadFromFile("mute.png") ||
		!m_textures[t_unmute].loadFromFile("unmute.png") ||
		!m_textures[t_bomb].loadFromFile("bomb.png") ||
		!m_textures[t_ammo].loadFromFile("ammo.png") ||
		!m_textures[t_score].loadFromFile("score.png") ||
		!m_textures[t_life].loadFromFile("lives.png"))
		throw std::runtime_error(TXTURE_ERR);

	// if failed to load any of the following textures, the game can continue without it
	m_textures[t_nameTag].loadFromFile("name_tag.png");
	m_textures[t_unmute].loadFromFile("unmute.png");
	m_textures[t_mute].loadFromFile("mute.png");
	m_textures[t_bomb].loadFromFile("bomb.png");
	m_textures[t_ammo].loadFromFile("ammo.png");
	m_textures[t_score].loadFromFile("score.png");
	m_textures[t_life].loadFromFile("lives.png");
	m_textures[t_jazz].setSmooth(true);
	m_textures[t_items].setSmooth(true);
	m_textures[t_enemies].setSmooth(true);
	m_textures[t_level_1].setSmooth(true);
	m_textures[t_level_2].setSmooth(true);
	m_textures[t_menu].setSmooth(true);
	m_textures[t_help].setSmooth(true);
}

//load all the audio for the game and saves it the the class's members
void Resources::loadAudio()
{
	// if one of the sounds could not open, do not throw an exceptions
	// the game can continue without it
	if (m_ingameMusic.openFromFile("resources/sound-resources/ingame_music.wav"))
	{
		m_ingameMusic.setVolume(VOL);
	}
	if (m_menuMusic.openFromFile("resources/sound-resources/menu.wav"))
	{
		m_menuMusic.setVolume(VOL);
		m_menuMusic.setLoop(true);
		m_menuMusic.play();
	}

	sf::SoundBuffer clickBuf, collectRubyBuf, reachCheckPointBuf, extraHealthBuf,
		shootingBuf, levelClearBuf, collectWeaponBuf, enemyDisBuf,
		throwBombBuf, resetBuf, jumpBuf;
	
	clickBuf.loadFromFile("resources/sound-resources/click.wav");
	collectRubyBuf.loadFromFile("resources/sound-resources/collect_ruby.wav");
	reachCheckPointBuf.loadFromFile("resources/sound-resources/checkpoint.wav");
	extraHealthBuf.loadFromFile("resources/sound-resources/collect_health.wav");
	shootingBuf.loadFromFile("resources/sound-resources/shooting.wav");
	levelClearBuf.loadFromFile("resources/sound-resources/level_clear.wav");
	enemyDisBuf.loadFromFile("resources/sound-resources/enemy_dis.wav");
	collectWeaponBuf.loadFromFile("resources/sound-resources/collect_weapon.wav");
	throwBombBuf.loadFromFile("resources/sound-resources/throw_bomb.wav");
	resetBuf.loadFromFile("resources/sound-resources/reset.wav");
	jumpBuf.loadFromFile("resources/sound-resources/jump.wav");
	
	m_soundBuffer.push_back(clickBuf);
	m_soundBuffer.push_back(collectRubyBuf);
	m_soundBuffer.push_back(reachCheckPointBuf);
	m_soundBuffer.push_back(extraHealthBuf);
	m_soundBuffer.push_back(shootingBuf);
	m_soundBuffer.push_back(levelClearBuf);
	m_soundBuffer.push_back(collectWeaponBuf);
	m_soundBuffer.push_back(enemyDisBuf);
	m_soundBuffer.push_back(throwBombBuf);
	m_soundBuffer.push_back(resetBuf);
	m_soundBuffer.push_back(jumpBuf);

	for (int i = 0; i < m_soundBuffer.size(); i++)
		m_sounds.push_back(sf::Sound(m_soundBuffer[i]));
}

void Resources::loadFile()
{
	std::string path = "worldObjectsPos.txt";
	m_objectsFile.open(path);
	if (!m_objectsFile.is_open())
	{
		throw std::runtime_error(WORLD_OBJ_ERR);
	}
	path = "high_score_table.txt";
	m_highScoresFile.open(path);
	if (!m_highScoresFile.is_open())
	{
		throw std::runtime_error(HIGH_SCORE_ERR);
	}

}

void Resources::loadFont()
{
	m_font.resize(f_size);
	if (!m_font[f_font].loadFromFile("font.ttf"))
		throw std::runtime_error(FONT_ERR);

}

void Resources::loadAnimationData()
{
	m_data.resize(Resources::Size);
	m_data[Resources::Jazz] = readAnimation("jazz.txt");
	m_data[Resources::MeleeEnemy] = readAnimation("melee_enemy.txt");
	m_data[Resources::RangedEnemy] = readAnimation("ranged_enemy.txt");
	m_data[Resources::SentryEnemy] = readAnimation("sentry_enemy.txt");
	m_data[Resources::BombWeapon] = readAnimation("bomb_weapon.txt");
	m_data[Resources::BulletWeapon] = readAnimation("bullet_weapon.txt");
	m_data[Resources::CoffeeWeapon] = readAnimation("coffee_weapon.txt");
	m_data[Resources::SmokeWeapon] = readAnimation("smoke_weapon.txt");
	m_data[Resources::Ruby] = readAnimation("ruby.txt");
	m_data[Resources::LifeCon] = readAnimation("life_con.txt");
	m_data[Resources::CheckPoint] = readAnimation("checkpoint.txt");
	m_data[Resources::AmmoCon] = readAnimation("ammo_con.txt");
	m_data[Resources::BombCon] = readAnimation("bomb_con.txt");
	m_data[Resources::FinishLine] = readAnimation("finish_line.txt");
}

Resources& Resources::instance()
{
	// the object inst is created once and can be reached everywhere in the game
	try
	{
		static Resources inst;
		return inst;
	}
	catch (std::exception& e)
	{
		sf::Font font;
		font.loadFromFile("font.ttf"); // there is no default font in sf::Text

		sf::RenderWindow window(sf::VideoMode(1700, 900), "Exception", sf::Style::Default | sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
		sf::Text exc;
		exc.setFont(font);
		exc.setCharacterSize(50);
		exc.setPosition({ 20,20 });
		exc.setFillColor(sf::Color::White);
		exc.setString(e.what());
		while (window.isOpen())
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
				case sf::Event::Closed: window.close(); exit(EXIT_FAILURE); break;

				case sf::Event::KeyPressed: window.close(); exit(EXIT_FAILURE); break;

				case sf::Event::MouseButtonReleased: window.close(); exit(EXIT_FAILURE); break;
				}
			}
			window.clear();
			window.draw(exc);
			window.display();
		}
	}
}

void Resources::switchBgMusic()
{
	isBgMusicPlaying() ? m_ingameMusic.pause() : m_ingameMusic.play();	
}
void Resources::playMusic(int music)
{
	switch (music)
	{
	case s_ingameMusic: m_ingameMusic.play();
		m_ingameMusic.setLoop(true);
		m_menuMusic.pause();
		break;
	case s_menuMusic: m_menuMusic.play(); 
		m_menuMusic.setLoop(true);
		m_ingameMusic.pause();
		break;
	}
}

bool Resources::isBgMusicPlaying() const
{
	return m_ingameMusic.getStatus() == PLAY;
}

// plays the wanted sound effect 
void Resources::playSound(int index)
{
	m_sounds[index].play();
}

//----------------------------------set functions section---------------------------------

//reveive a text and set its style
void Resources::configureText(sf::Text& txt, sf::Color fillColor, sf::Color outlineColor) const
{
	txt.setFont(Resources::instance().getFont(Resources::f_font));
	txt.setColor(fillColor);
	txt.setStyle(sf::Text::Bold);
	txt.setOutlineThickness(2);
	txt.setOutlineColor(outlineColor);
	txt.setCharacterSize(40);

}

//returns a text, and updates its characters and wanted features
sf::Text Resources::setText(unsigned int size, sf::Vector2f pos, sf::Color fillColor, std::string str, int outlineThickness, int typeFont) const
{
	sf::Text txt;
	txt.setFont(Resources::instance().getFont(typeFont));
	txt.setCharacterSize(size);
	txt.setPosition(pos);
	txt.setFillColor(fillColor);
	txt.setStyle(sf::Text::Bold);
	txt.setOutlineThickness(outlineThickness);
	txt.setString(str);
	return txt;
}

//creating a half transparent rectangle
sf::RectangleShape Resources::createRec(sf::Vector2f pos, sf::Vector2f size)
{
	sf::RectangleShape rec;
	rec.setPosition(pos);
	rec.setSize(size);
	rec.setFillColor(sf::Color{ 255, 255, 255, 70 });
	return rec;
}

//----------------------------------get functions section---------------------------------

// returns the wanted texture
const sf::Texture& Resources::getTexture(int index) { return m_textures[index]; }

// returns the wanted font
sf::Font& Resources::getFont(int index) { return m_font[index]; }

std::ifstream& Resources::getObjectsPosFile() { return m_objectsFile; }

std::fstream& Resources::getHighScoresFile() { return m_highScoresFile; }

sf::Image& Resources::getCursorImg() { return m_cursorImg; }