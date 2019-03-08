#pragma once
#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H
#include "SpriteComponent.h"
#include "SDL.h"

class AnimationComponent : public Component
{
public:
	//A struct for our animation variables
	struct Animation
	{
	public:
		Animation() {  }
		Animation(SDL_Texture* texture, std::string name, std::vector<SDL_Rect> frames, int maxFrames, float duration);
		void setLoop(bool b);
		void resetAnimation();

		std::string& getName() { return m_name; }
		SDL_Rect getCurrentTextureRect() { return m_frames.at(m_currentFrame); };
		SDL_Rect getOverallSize() { return m_overallSize; }
		SDL_Texture* getTexture() { return m_texture; }
		bool& getLoop();
		bool& getCompleted();
		int& getCurrentFrame();
		int& getMaxFrames();
		float& getTimeGone();
		float& getTimePerFrame();
		void setTexture(SDL_Texture* texture) { m_texture = texture; }
		std::vector<SDL_Rect> getFrames() { return m_frames; }
	private:
		std::string m_name; //Name of the animation
		SDL_Texture* m_texture; //The texture to animate
		std::vector<SDL_Rect> m_frames; //Frames of the animation
		SDL_Rect m_overallSize; //Size of the entire animation
		int m_currentFrame; //The current frame of the animation
		int m_maxFrames; //Maximum frames in the animation
		float m_duration; //The duration of the animation
		float m_timePerFrame; // The time to display each frame
		float m_timeGone; //The time gone since last animation frame
		bool m_loop; //Wheter the animation loops or plays once
		bool m_completed; //Wheter the animation is finished or not
	};

	AnimationComponent(Component* sprite);

	void addAnimation(std::string name, SDL_Texture* texture, std::vector<SDL_Rect> frames, float duration);
	void playAnimation(std::string name, bool loop);

	std::string getCurrentID() { return m_current->getName(); }
	Animation* getCurrentAnimation() { return m_current; }
	SpriteComponent* getSprite() { return static_cast<SpriteComponent*>(m_spriteToAnimate); }
private:
	Component * m_spriteToAnimate;
	Animation* m_current;
	std::map<std::string, Animation> m_animations; //Map of animations
};
#endif