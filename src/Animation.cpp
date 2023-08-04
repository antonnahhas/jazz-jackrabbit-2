//----------------------------------include section-----------------------------------
#include "Animation.h"
#include "Resources.h"
#include "macros.h"

//----------------------------------const section-------------------------------------
const auto AnimationTime = sf::seconds(0.2f); // the speed of the animation's frames
const float ANI_TIME = 0.1;

//----------------------------------class implementation------------------------------

// constructor - initialize all the members
Animation::Animation(const AnimationData& data, sf::Sprite& sprite) :
    m_data(data),
    m_sprite(sprite),
    m_action(INIT)
{
    update();
}

//show the action out of the frames sheet
void Animation::action(int action, Direction dir)
{
    if (m_action == action && m_dir == dir)
    {
        return;
    }
    m_action = action;
    m_dir = dir;
    if (m_dir == Direction::Down)
        m_frameIndex = m_data.m_animeMap.find(m_action)->second.size() - 1;
    else
    {
        m_frameIndex = INIT;
        m_end = false;
    }
    update();
}

//update the current animation according to the time that passed
void Animation::update(sf::Time delta)
{
    m_elapsed += delta;
    auto AnimTime = AnimationTime;

    if (m_toSpeed)
    {
        AnimTime = sf::seconds(ANI_TIME);
        m_toSpeed = false;
    }

    if (m_elapsed >= AnimTime)
    {
        m_elapsed -= AnimTime;
        if (m_dir == Direction::Down)
            --m_frameIndex;

        else
            ++m_frameIndex;
        m_frameIndex %= m_data.m_animeMap.find(m_action)->second.size();

        if (m_frameIndex == m_data.m_animeMap.find(m_action)->second.size() - 1)
            m_end = true;

        update();
    }
}

//change the speed to the anumation frames switching
void Animation::speedUpAnimation(bool toSpeed) {  m_toSpeed = toSpeed; }

//returns true when thr frames sheet reached its end
bool Animation::animationEneded() const { return m_end; }

//update sprite according to the current frame out of the frames sheet
void Animation::update()
{
    m_sprite.setTextureRect(m_data.m_animeMap.find(m_action)->second[m_frameIndex]);
    m_sprite.setOrigin(m_data.m_originPoint.find(m_action)->second[m_frameIndex]);
}
