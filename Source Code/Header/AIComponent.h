#ifndef AICOMPONENT_H
#define AICOMPONENT_H

#include "Component.h"
#include "BehaviourTree.h"
#include "Entity.h"
#include <vector>
#include "AiInputComponent.h"
#include "PlayerComponent.h"
#include "Box2DBridge.h"

class AIComponent : public Component
{
public:
	AIComponent(std::vector<Entity *>* e, AiInputComponent * i, Entity * self, PlayerComponent * p, Box2DBridge & world) :
		m_entities(e),
		m_input(i),
		m_self(self),
		m_player(p),
		m_world(world)
	{
		m_left = new PhysicsComponent(static_cast<PositionComponent *>(&self->getComponent("Pos")));
		m_right = new PhysicsComponent(static_cast<PositionComponent *>(&self->getComponent("Pos")));
		auto pos = static_cast<PositionComponent *>(&self->getComponent("Pos"))->position;
		m_left->m_body = m_world.createBox(pos.x, pos.y, 30, 78, false, false, b2_dynamicBody);
		m_world.addProperties(*m_left->m_body, 0, 0, 0, true, new PhysicsComponent::ColData("Left Edge Sensor", m_self));
		m_right->m_body = m_world.createBox(pos.x, pos.y, 30, 78, false, false, b2_dynamicBody);
		m_world.addProperties(*m_right->m_body, 0, 0, 0, true, new PhysicsComponent::ColData("Right Edge Sensor", m_self));
	}
	~AIComponent() {}

	//Declare Tree, selector nodes and sequence nodes
	BehaviourTree BT;
	BehaviourTree::Selector m_selectors[7];
	BehaviourTree::RandomSelector m_random;
	BehaviourTree::Sequence m_sequences[3];
	BehaviourTree::Sequence punchSequence;
	BehaviourTree::Succeeder m_succeeders[3];
	std::vector<Entity *>* m_entities;
	Entity * m_self;
	Entity * nearestPlayer;
	AiInputComponent * m_input;
	PlayerComponent * m_player;
	PhysicsComponent * m_right;
	PhysicsComponent * m_left;
	Box2DBridge m_world;

	bool onEdgeLeft = false;
	bool onEdgeRight = false;
	float timer = 0.f;
	
	//Declare actions for AI
};

#endif