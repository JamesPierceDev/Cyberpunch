#pragma once
#include "System.h"
#include "Entity.h"
#include "DustComponent.h"
#include "DustTriggerComponent.h"
#include "ResourceManager.h"

class DustSystem : public System
{
public:
	DustSystem(std::map<std::string, System*>* systems, std::vector<Entity*>* players, ResourceHandler* rH);
	void addComponent(Component* c);
	void update(double dt);

private:
	ResourceHandler * m_resourcesPtr;
	std::vector<Entity*>* m_players;
	std::vector<Entity*> m_dustParticles; //All the dust in the game
	std::vector<Entity*> m_dustToDelete;
	//All the systems in the game
	std::map<std::string, System*>* m_systems;
	//Dust particles rectangles
	std::vector<SDL_Rect> m_dustFrames;
};