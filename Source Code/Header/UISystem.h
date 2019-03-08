#pragma once
#include "System.h"
#include "Entity.h"

class UISystem : public System
{
public:
	UISystem(std::map<Entity*, Entity*>* uimap);
	void addComponent(Component* c);
	void update(double dt);
private:
	std::map<Entity*, Entity*>* m_map;
};