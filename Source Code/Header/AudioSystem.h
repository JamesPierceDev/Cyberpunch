#pragma once
#include "System.h"
#include "PhysicsComponent.h"

class AudioSystem : public System
{
public: 
	AudioSystem() : m_play(false) {}
	void addComponent(Component* comp);
	void update(double dt);
	void playSound();

private: 
	bool m_play;
	
};