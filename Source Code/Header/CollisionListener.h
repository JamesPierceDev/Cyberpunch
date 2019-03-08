#pragma once
#include "Box2D/Box2D.h"
#include "Observer.h"
#include <string>
#include <iostream>

//Class listens for collisions, this is an observer class
//From here we can check if specific sensor have collided
class CollisionListener : public b2ContactListener, public Subject
{
public:
	CollisionListener() : 
		m_gravFlipped(false)
	{}

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void checkPlayerAttack(b2Contact* contact);
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	void flipGravity();
private:
	bool m_gravFlipped;
};