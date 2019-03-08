#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include "Box2DBridge.h"
#include "PositionComponent.h"
#include "AudioComponent.h"

class PhysicsComponent : public Component
{
public:
	PhysicsComponent(PositionComponent* pos) : m_body(nullptr), posPtr(pos){}

	//The body of the physics component
	Box2DBody* m_body;

	//Hold a ptr to the position component that the physics modifies
	PositionComponent * posPtr; 

	//This is used for filtering collisions in our collision listener
	struct ColData
	{
	public:
		ColData(std::string _tag, void* data) : tag(_tag), objectData(data) {}

		std::string Tag() { return tag; }
		void* Data() { return objectData; }
	private:
		std::string tag;
		void* objectData;
		
	};
};

#endif
