#pragma once
#include "System.h"
#include "PlayerComponent.h"

class PlayerRespawnSystem : public System
{
public:
	PlayerRespawnSystem();

	void addComponent(Component* c);
	void update(double dt);
private:
};