
//----------------------------------include section-----------------------------------
#pragma once
#include "Direction.h"
#include "AnimationData.h"
#include "Resources.h"
#include <SFML/Graphics.hpp>

//----------------------------------class section-------------------------------------

class Animation
{
public:
    Animation(const AnimationData&, sf::Sprite&);
    ~Animation() = default;
    void action(int action, Direction);
    void update(sf::Time delta);
    void speedUpAnimation(bool toSpeed);
    bool animationEneded() const;

private:
    void update();

    int m_frameIndex = 0;           //the current frame out of the animation sheet
    int m_action;                   //objects's action enum
    bool m_end = false;             //true when reaching the end of an animation sheet
    bool m_toSpeed = false;         //if we want to speed up the pace of the frames
    const AnimationData& m_data;    //saves the object's data of animation
    Direction m_dir = Direction::Up;//direction of the current animation
    sf::Sprite& m_sprite;           //the sprite that shows the current frame out of the animation sheet
    sf::Time m_elapsed = {};        //the time that passed since we last checked
};

