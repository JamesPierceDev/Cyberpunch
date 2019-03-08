#pragma once
#include "System.h"
#include "AnimationComponent.h"

class AnimationSystem : public System
{
public:
	AnimationSystem() {};
	void addComponent(Component * comp);
	void update(double dt);
private:
};