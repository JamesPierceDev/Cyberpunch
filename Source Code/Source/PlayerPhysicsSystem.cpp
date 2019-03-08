#include "PlayerPhysicsSystem.h"

void PlayerPhysicsSystem::setWorld(Box2DBridge & world)
{
	m_worldPtr = &world;
}

void PlayerPhysicsSystem::addComponent(Component * comp)
{
	m_components.push_back(comp);
}

void PlayerPhysicsSystem::update(double dt)
{
	for (auto& comp : m_components)
	{
		//Convert the component down to a player physics component
		auto p = static_cast<PlayerPhysicsComponent*>(comp);

		if (p->toSetStatic())
		{
			p->changeBodyType(b2BodyType::b2_staticBody);
			p->toSetStatic() = false;
		}
		else if (p->toSetDynamic())
		{
			p->changeBodyType(b2BodyType::b2_dynamicBody);
			p->toSetDynamic() = false;
		}

		if (m_gravityChange)
			p->flipGravity(*m_worldPtr);

		//Get the velocity of the player
		p->m_currentVel = p->m_body->getBody()->GetLinearVelocity();

		//if we are to move, call move and the direction to move (-1, 1)
		if (p->m_desiredVel.x != 0)
			p->move(p->m_desiredVel.x);
		if (p->m_desiredVel.y == -1)
			p->moveUp();
		if (p->m_desiredVel.y == 1)
			p->moveDown();

		if (p->m_desiredVel.x == 0 && p->stunned() == false)
			p->m_currentVel.x = 0;

		//Set the velocity of the player
		p->m_body->getBody()->SetLinearVelocity(p->m_currentVel);

		//Reset desired velocity
		p->m_desiredVel = b2Vec2(0,0);

		//check if to unstun the player
		if (p->stunned())
		{
			p->stunLeft() -= dt;

			if (p->stunLeft() <= 0)
			{
				if (p->superStunned())
					p->endSuperStun();
				else
					p->stunned() = false;
			}
		}
		//If the player is supered
		else if (p->isSupered())
		{
			//Take away from the super time left
			p->superLeft() -= dt;

			//If the time has gone for the super, end the super for the player
			if (p->superLeft() <= 0)
				p->endSuper();
		}

		//set the position component of the player
		p->posPtr->position = p->m_body->getPosition();
	}

	if (m_gravityChange)
	{
		m_gravityChange = false;
	}
}

void PlayerPhysicsSystem::flipGravity()
{
	m_gravityChange = true;
	m_gravFlipped = !m_gravFlipped;
}
