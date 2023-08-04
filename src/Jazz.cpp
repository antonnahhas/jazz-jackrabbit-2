//----------------------------------include section-----------------------------------
#include "Jazz.h"
#include "Resources.h"

//----------------------------------const section-------------------------------------

const float OFFSET = 0.5;
constexpr auto RUNNIG_SPEED = 30.f,
				CLIMING_SPEED = 10.f,
				JUMP_IMPULSE = 100.f,
				JUMP_IMPULSE_ATTACK = 2.f ,
				JUMP_IMPULSE_IDLE = 3.f,
				JUMP_IMPULSE_RUN = 1.5f,
				JUMP_IMPULSE_THROW = 2.f ;
const int ATTACK_TIME = 1;

//----------------------------------class implementation------------------------------

// constructor - initialize all the members
Jazz::Jazz(sf::Vector2f pos) : DynamicObject(pos, Resources::instance().animationData(Resources::Jazz)),
									m_action(PlayerAction::Idle), m_animationAction(AnimationAction::Idle)
{
	m_jazzStatus.resetJazzStatus();
	m_playerData._score = SCORE_INIT;
	m_playerData._lives = LIVES_DEFAULT;
	m_playerData._ammo = AMMO_INIT;
	m_playerData._bombs = BOMB_INIT;
	m_sprite.setTexture(Resources::instance().getTexture(Resources::t_jazz));
}

//----------------------------------functions section---------------------------------

Direction Jazz::dirFromKey() const
{
	using KeyMapping = std::initializer_list<std::pair<sf::Keyboard::Key, Direction>>;
	static const KeyMapping keyToDirectionMapping =
	{
		{ sf::Keyboard::Right, Direction::Right},
		{ sf::Keyboard::Left , Direction::Left },
		{ sf::Keyboard::Up   , Direction::Up   },
		{ sf::Keyboard::Down , Direction::Down },
	};

	for (const auto& pair : keyToDirectionMapping)
		if (sf::Keyboard::isKeyPressed(pair.first))
			return pair.second;

	return Direction::Stay;
}

//update Jazz's cur action
void Jazz::updateAction(const sf::Time& deltaTime)
{
	m_elapsed += deltaTime;
	if ((m_action == PlayerAction::Idle) ||
		(m_action != PlayerAction::Idle &&
			m_animation.animationEneded()))
	{
		setAction();
		m_jazzStatus._onRope = false;
		m_jazzStatus._onGround = false;
		m_jazzStatus._isFalling = false;
	}
	m_sprite.setPosition(m_body->GetPosition().x, m_body->GetPosition().y);
	if (m_action != PlayerAction::Idle)
		m_animation.speedUpAnimation(true);
	DynamicObject::updateDynamicObject(deltaTime, playerActionToInt(m_animationAction));
}

// update Jazz's data
void Jazz::updateInfo(int feature, int amountToAdd)
{
	switch (feature)
	{
	case SCORE:
		m_playerData._score += amountToAdd;
		break;
	case LIVES:
		m_playerData._lives += amountToAdd;
		break;
	case AMMO:
		m_playerData._ammo += amountToAdd;
		break;
	case BOMBS:
		m_playerData._bombs += amountToAdd;
		break;
	}
}

//all of Jazz's possible actions
int Jazz::playerActionToInt(AnimationAction action) const
{
	switch (action)
	{
	case AnimationAction::Idle:				return 0;
	case AnimationAction::Walk:				return 1;
	case AnimationAction::Shoot:			return 2;
	case AnimationAction::Throw:			return 3;
	case AnimationAction::ClimbVertical:	return 4;
	case AnimationAction::IdleOnRope:		return 5;
	case AnimationAction::AttackOnRope:		return 6;
	case AnimationAction::ThrowOnRope:		return 7;
	case AnimationAction::IdleJump:			return 8;
	case AnimationAction::RunJump:			return 9;
	case AnimationAction::JumpThrow:		return 10;
	case AnimationAction::JumpShoot:		return 11;
	case AnimationAction::Fall:				return 12;
	case AnimationAction::Crouch:			return 13;
	case AnimationAction::CrouchShoot:		return 14;
	case AnimationAction::CrouchThrow:		return 15;
	case AnimationAction::Win:				return 16;
	default:								break;
	}
	return 0;
}

//being called in each restart of the game
void Jazz::resetJazzData()
{
	m_playerData._lives = LIVES_DEFAULT;
	m_playerData._score = SCORE_INIT;
	m_playerData._ammo = AMMO_INIT;
	m_playerData._bombs = BOMB_INIT;
	m_defualtPosition = convertVec2fTob2Vec2(STAY_VEC);
	m_jazzStatus._onRope = false;	
	m_jazzStatus._onGround = false;
	m_jazzStatus._isJumping = false;
	m_jazzStatus._isFalling = true;  
}

//move Jazz a little backwards when hitting a wall
void Jazz::hitAwall()
{
	sf::Vector2f mv; 
	if (getDirection() == Direction::Right)
		mv.x = -OFFSET + m_body->GetPosition().x;
	else if (getDirection() == Direction::Left)
		mv.x = OFFSET + m_body->GetPosition().x;

	mv.y = m_body->GetPosition().y;
	m_body->SetTransform(convertVec2fTob2Vec2(mv), m_body->GetAngle());
	m_sprite.setPosition(m_body->GetPosition().x, m_body->GetPosition().y);
}

//center Jazz when climbing on a rope
void Jazz::centerOnRope(sf::Vector2f pos)
{
	if (!m_jazzStatus._isJumping && !m_jazzStatus._onGround)
		m_body->SetTransform({ convertVec2fTob2Vec2(pos).x, m_sprite.getPosition().y }, m_body->GetAngle());
	b2Vec2 position = convertVec2fTob2Vec2(pos); 
}

//attach a key to an action of Jazz
Jazz::PlayerAction Jazz::actionFromKey() const
{
	using KeyMapping = std::initializer_list<std::pair<sf::Keyboard::Key, PlayerAction>>;
	static const KeyMapping keyToactionMapping =
	{
		{ sf::Keyboard::LControl, PlayerAction::Shoot},
		{ sf::Keyboard::LAlt    , PlayerAction::Throw },
		{ sf::Keyboard::X		, PlayerAction::Shoot},
		{ sf::Keyboard::Z		, PlayerAction::Throw },
	};

	for (const auto& pair : keyToactionMapping)
		if (sf::Keyboard::isKeyPressed(pair.first))
			return pair.second;

	return PlayerAction::Idle;
}

// set scale according to the pressed direction
void Jazz::faceToCorrectDirection(Direction dir)
{
	if (m_dir != dir && dir == Direction::Left)
		m_sprite.setScale(LEFT_SPRITE_TURN);
	else if (m_dir != dir && dir == Direction::Right)
		m_sprite.setScale(RIGHT_SPRITE_TURN);
	m_dir = dir;
}

//update Jazz's AnimationAction while in jump mode
b2Vec2 Jazz::handleJump()
{
	b2Vec2 jumpForce(0, -1);
	if (!m_jazzStatus._isJumping && (m_jazzStatus._onRope || m_jazzStatus._onGround))
	{
		m_jazzStatus._isJumping = true;
		jumpForce.y = -1;
		switch (m_animationAction)
		{
		case Jazz::AnimationAction::Idle:
			Resources::instance().playSound(Resources::s_jump);
			m_animationAction = AnimationAction::IdleJump;
			jumpForce *= JUMP_IMPULSE / JUMP_IMPULSE_IDLE;
			break;

		case Jazz::AnimationAction::Walk:
			Resources::instance().playSound(Resources::s_jump);
			m_animationAction = AnimationAction::RunJump;
			m_body->SetLinearVelocity(b2Vec2(m_body->GetLinearVelocity().x / 2, m_body->GetLinearVelocity().y));
			jumpForce *= JUMP_IMPULSE * JUMP_IMPULSE_RUN;
			break;

		case Jazz::AnimationAction::Shoot:
			Resources::instance().playSound(Resources::s_jump);
			m_animationAction = AnimationAction::JumpShoot;
			jumpForce *= JUMP_IMPULSE / JUMP_IMPULSE_ATTACK;
			break;

		case Jazz::AnimationAction::Throw:
			Resources::instance().playSound(Resources::s_throwBomb);
			m_animationAction = AnimationAction::JumpThrow;
			jumpForce *= JUMP_IMPULSE / JUMP_IMPULSE_THROW;
			break;

		case Jazz::AnimationAction::IdleOnRope:
			Resources::instance().playSound(Resources::s_jump);
			m_animationAction = AnimationAction::Fall;
			jumpForce *= JUMP_IMPULSE;
			m_body->SetLinearVelocity(b2Vec2(toVector(dirFromKey()).x * CLIMING_SPEED, 0));
			break;
		default:
			break;
		}
	}
	else
	{
		m_action = actionFromKey();
		switch (m_action)
		{
		case Jazz::PlayerAction::Idle:
			break;
		case Jazz::PlayerAction::Shoot:
			m_animationAction = AnimationAction::JumpShoot;
			break;
		case Jazz::PlayerAction::Throw:
			m_animationAction = AnimationAction::JumpThrow;
			break;
		default:
			break;
		}
	}
	return jumpForce;
}

//update Jazz's AnimationAction while in walking mode
void Jazz::handleMove()
{
	b2Vec2 moveBodyB2ve2(0, 1); 

	if (m_jazzStatus._onGround)
	{
		m_jazzStatus._isJumping = false;
		if (m_dir == Direction::Right || m_dir == Direction::Left)
		{
			m_animationAction = AnimationAction::Walk;
			moveBodyB2ve2 = convertVec2fTob2Vec2(toVector(m_dir) * RUNNIG_SPEED);
			m_body->SetLinearVelocity(moveBodyB2ve2);
		}
		else if (m_dir == Direction::Down)
			m_animationAction = AnimationAction::Crouch;
		else
			m_animationAction = AnimationAction::Idle;
	}
	else if (m_jazzStatus._onRope)
	{
		m_jazzStatus._isJumping = false;
		if (m_dir == Direction::Up || m_dir == Direction::Down)
		{
			m_animationAction = AnimationAction::ClimbVertical;
			moveBodyB2ve2 = convertVec2fTob2Vec2(toVector(m_dir) * CLIMING_SPEED);
			m_body->SetLinearVelocity(moveBodyB2ve2);
		}
		else
			m_animationAction = AnimationAction::IdleOnRope;
	}
	else if (m_jazzStatus._isJumping && m_animation.animationEneded() && m_action == PlayerAction::Idle)
	{
		m_jazzStatus._isJumping = false;
		m_animationAction = AnimationAction::Fall;
	}
}

//update Jazz's current action according to a key received or his last status
void Jazz::handleAct()
{
	m_action = actionFromKey();
	if (m_action == PlayerAction::Shoot && m_playerData._ammo > 0)
	{
		if (m_action == PlayerAction::Shoot && m_elapsed.asSeconds() > ATTACK_TIME)
		{
			m_elapsed = m_elapsed.Zero;
			m_bulletWeapon = true;

			updateInfo(AMMO, DEC);
			switch (m_animationAction)
			{
			case Jazz::AnimationAction::Idle:
				m_animationAction = AnimationAction::Shoot;
				Resources::instance().playSound(Resources::s_shooting);
				break;
			case Jazz::AnimationAction::IdleOnRope:
				m_animationAction = AnimationAction::AttackOnRope;
				Resources::instance().playSound(Resources::s_shooting);
				break;
			case Jazz::AnimationAction::Crouch:
				Resources::instance().playSound(Resources::s_shooting);
				m_animationAction = AnimationAction::CrouchShoot;
				m_dir = Direction::Stay;
				break;
			default:
				break;
			}
		}
	}
	else if (m_action == PlayerAction::Throw && m_playerData._bombs > 0)
	{
		if (m_action == PlayerAction::Throw && m_elapsed.asSeconds() > ATTACK_TIME)
		{
			m_elapsed = m_elapsed.Zero;
			m_bombWeapon = true;

			updateInfo(BOMBS, DEC);
			switch (m_animationAction)
			{
			case Jazz::AnimationAction::Idle:
				Resources::instance().playSound(Resources::s_throwBomb);
				m_animationAction = AnimationAction::Throw;
				break;
			case Jazz::AnimationAction::IdleOnRope:
				Resources::instance().playSound(Resources::s_throwBomb);
				m_animationAction = AnimationAction::ThrowOnRope;
				break;
			case Jazz::AnimationAction::Crouch:
				Resources::instance().playSound(Resources::s_throwBomb);
				m_animationAction = AnimationAction::CrouchThrow;
				m_dir = Direction::Stay;
				break;
			}
		}
	}
}

//limit Jazz while reaching to the top of a rope
void Jazz::reachedEndOfRope()
{
	m_body->SetTransform({ m_body->GetPosition().x, m_body->GetPosition().y + OFFSET }, m_body->GetAngle());
}

//----------------------------------set functions section---------------------------------

void Jazz::setAction()
{
	//Get player direction from keyboard
	auto dir = dirFromKey();
	// set scale according to the pressed direction
	faceToCorrectDirection(dir);

	handleMove();
	handleAct();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		m_body->ApplyLinearImpulse(handleJump(), m_body->GetPosition(), true);

	if (m_animationAction == AnimationAction::IdleOnRope)
		m_body->SetAwake(false);
	if (m_jazzStatus._isJumping)
		m_body->SetAwake(true);
}

//if Jazz lost life, he returns to his default position and his status is reset
void Jazz::setJazzToDefaultPosition()
{
	m_body->SetTransform(m_defualtPosition, m_body->GetAngle());
	m_jazzStatus.resetJazzStatus();
	m_body->SetAwake(true);
	b2Vec2 position = m_body->GetPosition();
	m_sprite.setPosition(position.x, position.y);
}

void Jazz::setOnGround(bool onGround) { m_jazzStatus._onGround = onGround; }

void Jazz::setOnRope(bool onRope) { m_jazzStatus._onRope = onRope; }

void Jazz::setIdleAction() { m_action = PlayerAction::Idle; }

void Jazz::setDefaultPosition(sf::Vector2f pos) { m_defualtPosition = convertVec2fTob2Vec2(pos); }

void Jazz::setSpeed(float speed) { m_playerData._speed = speed; }

void Jazz::setBombWeapon(bool throwing) { m_bombWeapon = throwing; }

void Jazz::setBulletWeapon(bool shooting) { m_bulletWeapon = shooting; }

//----------------------------------get functions section---------------------------------

const PlayerData& Jazz::getJazzData() const {	return m_playerData; }

bool Jazz::getOnRope() const { return m_jazzStatus._onRope; }

sf::Vector2f Jazz::getJazzPos() const { return m_sprite.getPosition(); }

Jazz::PlayerAction Jazz::getAction() const { return m_action; }

bool Jazz::isBombWeapon() const { return m_bombWeapon; }

bool Jazz::isBulletWeapon() const { return m_bulletWeapon; }