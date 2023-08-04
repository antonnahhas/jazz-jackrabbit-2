//----------------------------------include section-----------------------------------
#pragma once

#include <optional>
#include "DynamicObject.h"
#include "BombWeapon.h"
#include "BulletWeapon.h"
#include "box2d/box2d.h"

//----------------------------------class definition----------------------------------
class Jazz : public DynamicObject
{
public:
	Jazz(sf::Vector2f);
	
	enum class PlayerAction
	{
		Idle,
		Shoot,
		Throw,
	};
	
	enum class AnimationAction
	{
		Idle,					
		Walk,					
		Shoot,                  		
		Throw,				    
		ClimbVertical,			
		IdleOnRope,				
		AttackOnRope,			 
		ThrowOnRope,			 
		IdleJump,				 
		RunJump,				
		JumpThrow,				
		JumpShoot,				
		Fall,					
		Crouch,					
		CrouchShoot,			
		CrouchThrow,			
		Win,					
	};

	virtual void updateAction(const sf::Time&) override;
	virtual void setAction() override;

	void hitAwall();
	void updateInfo(int, int);
	void centerOnRope(sf::Vector2f);
	void resetJazzData();
	void reachedEndOfRope();
	int playerActionToInt(AnimationAction action) const;


	sf::Vector2f getJazzPos() const;
	PlayerAction getAction() const;
	const PlayerData& getJazzData() const;
	bool getOnRope() const;

	void setOnGround(bool);
	void setOnRope(bool);
	void setSpeed(float);
	void setIdleAction();
	void setJazzToDefaultPosition();
	void setDefaultPosition(sf::Vector2f);

	void setBombWeapon(bool throwing);
	void setBulletWeapon(bool throwing);
	bool isBombWeapon() const;
	bool isBulletWeapon() const;

private:
	Direction dirFromKey() const;
	Jazz::PlayerAction actionFromKey() const;
	void faceToCorrectDirection(Direction);  
	b2Vec2 handleJump();
	void handleMove();
	void handleAct();
	
	struct JazzStatus
	{
		bool _onRope,   // if the player is currently on a rope
			_wonLevel,  // if the player won the level
			_onGround,  // if the player is on the ground
			_isJumping, // if the player is jumping
			_isFalling;  // if the player is falling
		void resetJazzStatus()
		{
			_onRope = false;
			_wonLevel = false;
			_onGround = false;
			_isJumping = false;
			_isFalling = true;
		}
	};

	bool			m_bombWeapon = false; // if the object is currently throwing a bomb
	bool			m_bulletWeapon = false;	// if the object is currently shooting

	PlayerData		m_playerData;       // the player's score, lives and weapons
	PlayerAction	m_action;	        // the player's current action
	b2Vec2			m_defualtPosition;  // the player's default position
	AnimationAction m_animationAction;  // the player's current animation action
	JazzStatus      m_jazzStatus;	// the player's status with other objects
	sf::Time		m_deltaTime = {};   // current time that passed
};
