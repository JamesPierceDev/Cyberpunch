#pragma once
#include "System.h"
#include "PhysicsComponent.h"

class PhysicsSystem : public System
{
public:
	PhysicsSystem() : m_gravFlipped(false) {}
	void addComponent(Component* comp);
	void update(double dt);
	void flipGravity();
	void addForce(Box2DBody& body, Vector2f force);
	void addForce(Box2DBody& body, float forceX, float forceY);
private:
	bool m_gravFlipped;
	float m_scaler = 0.0;
};