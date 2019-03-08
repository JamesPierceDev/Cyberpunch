#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(Component* sprite) :
	m_current(nullptr),
	m_spriteToAnimate(sprite)
{

}

void AnimationComponent::addAnimation(std::string name, SDL_Texture* texture, std::vector<SDL_Rect> frames, float duration)
{
	//Add the animation to our map
	m_animations[name] = Animation(texture, name, frames, frames.size(), duration);
}

void AnimationComponent::playAnimation(std::string name, bool loop)
{
	//If there is an animation playing then stop it, and play the ne wone
	if (nullptr != m_current)
	{
		if(m_current->getName() != name)
			m_current->resetAnimation();
	}

	m_current = &m_animations[name];
	m_current->setLoop(loop);
	//Set texture of the sprite based on the animation that is being played
	getSprite()->setTexture(m_current->getTexture());
	//Set the destination width and height
	auto sDst = getSprite()->getDestRect();
	auto aDst = m_current->getCurrentTextureRect();
	sDst.w = aDst.w;
	sDst.h = aDst.h;
	getSprite()->setDestRect(sDst);
	getSprite()->setSourceRect(aDst);
}

AnimationComponent::Animation::Animation(SDL_Texture* texture, std::string name, std::vector<SDL_Rect> frames, int maxFrames, float duration) :
	m_texture(texture),
	m_name(name),
	m_loop(false),
	m_frames(frames),
	m_duration(duration),
	m_maxFrames(maxFrames),
	m_completed(false),
	m_currentFrame(0),
	m_timeGone(0)
{
	//Get the time per frame
	m_timePerFrame = m_duration / m_maxFrames;

	m_overallSize = { 0,0, frames.at(0).w * m_maxFrames, frames.at(0).h };
}

void AnimationComponent::Animation::setLoop(bool b)
{
	m_loop = b;
	//If loop is true, set completed to false, else set it to true
	if (m_currentFrame >= m_maxFrames)
		m_completed = m_loop ? false : true;
}

void AnimationComponent::Animation::resetAnimation()
{
	m_currentFrame = 0;
	m_timeGone = 0;
	m_completed = false;
}

bool & AnimationComponent::Animation::getLoop()
{
	return m_loop;
}

bool & AnimationComponent::Animation::getCompleted()
{
	return m_completed;
}

int & AnimationComponent::Animation::getCurrentFrame()
{
	return m_currentFrame;
}

int & AnimationComponent::Animation::getMaxFrames()
{
	return m_maxFrames;
}

float & AnimationComponent::Animation::getTimeGone()
{
	return m_timeGone;
}

float & AnimationComponent::Animation::getTimePerFrame()
{
	return m_timePerFrame;
}
