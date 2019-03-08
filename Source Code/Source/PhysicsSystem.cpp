#include "PhysicsSystem.h"

void PhysicsSystem::addComponent(Component * comp)
{
	//Add component to the system
	m_components.push_back(comp);
}

void PhysicsSystem::update(double dt)
{
	//Loop through all of the physics components and set the position components based on their physics position
	for (auto& comp : m_components)
	{
		//Convert the component to a physics component
		auto p = dynamic_cast<PhysicsComponent*>(comp);

		//Assign the bodies position the position component
		p->posPtr->position = p->m_body->getPosition();
	}
}

void PhysicsSystem::flipGravity()
{
	//Flip the boolean
	m_gravFlipped = !m_gravFlipped;
}

void PhysicsSystem::addForce(Box2DBody & body, Vector2f force)
{
	body.getBody()->ApplyLinearImpulse(b2Vec2(force.x, m_gravFlipped ? force.y * -1 : force.y), body.getBody()->GetWorldCenter(), true);
}

void PhysicsSystem::addForce(Box2DBody & body, float forceX, float forceY)
{
	body.getBody()->ApplyLinearImpulse(b2Vec2(forceX, m_gravFlipped ? forceY * -1 : forceY), body.getBody()->GetWorldCenter(), true);
}
