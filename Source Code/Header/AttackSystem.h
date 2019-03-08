#pragma once
#include "SDL.h"
#include "System.h"
#include "AttackComponent.h"

class AttackSystem : public System
{
public:
	AttackSystem(Box2DBridge& world) :m_worldPtr(&world) { };
	void addComponent(Component * comp);
	void update(double dt);
private:
	Box2DBridge * m_worldPtr;
};