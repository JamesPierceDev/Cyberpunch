#ifndef  DJBOOTHSYSTEM_H
#define DJBOOTHSYSTEM_H

#include "System.h"
#include "PickUpComponent.h"
#include "ResourceManager.h"
#include "Entity.h"
#include "AudioComponent.h"

class DJBoothSystem : public System
{
public:
	DJBoothSystem(ResourceHandler* Res, std::vector<Entity*>* platforms, Entity* bg,
		AudioComponent* audio) :
		m_audioPtr(audio),
		m_resourcePtr(Res),
		m_bgPtr(bg),
		m_platformsPtr(platforms),
		m_currentBg(0),
		m_currentPc(0),
		m_currentTrack(0)
	{}
		
	void addComponent(Component* comp);
	void update(double dt);
	float getScalar();
private:
	AudioComponent * m_audioPtr;
	ResourceHandler* m_resourcePtr;
	Entity* m_bgPtr;
	std::vector<Entity*>* m_platformsPtr;
	int m_currentBg;
	int m_currentPc;
	int m_currentTrack;
	Entity* m_pcPtr;
};

#endif // !  DJBOOTHSYSTEM_H

