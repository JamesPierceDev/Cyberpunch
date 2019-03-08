#ifndef PICKUP_H
#define PICKUP_H

#include "SDL.h"
#include "Box2DBridge.h"
#include "PhysicsSystem.h"
#include "ResourceManager.h"

class PickUp
{
public:
	PickUp();

	void  draw(SDL_Renderer& renderer);
	void createPickUp(Box2DBridge& world, PhysicsSystem& system);
	void  deletePickUp();
	void update(double dt);
	void flipGravity();

	
	
private:
	float m_moveSpeed;
	SDL_Rect m_pickUp;
	PhysicsSystem* physPtr;
	PhysicsComponent m_physComponent;
	PositionComponent m_posComonponent;
	bool m_draw = true;

	Box2DBridge * m_worldPtr;
};

#endif // !PICKUP_H