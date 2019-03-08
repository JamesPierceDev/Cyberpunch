#include "..\Header\PickUpComponent.h"

PickUpComponent::PickUpComponent(Entity* pickupEntity) :
	m_pickupEntity(pickupEntity),
	m_timeLive(10),
	m_timeTillSpawn(10),
	m_timeInBooth(10),
	m_spawned(false),
	m_currentPos(0),
	m_body(nullptr)
{
}

void PickUpComponent::spawn(Box2DBridge & world)
{
	m_end = false;
	m_spawned = true;
	m_timeLive = 10; //10 seconds
	delete m_body;
	m_body = new PhysicsComponent(new PositionComponent(0,0));

	if (m_currentPos == 1)
	{
		m_position = m_pos2;
		//m_teleportLocationB = m_position;
	}
	else if (m_currentPos == 2)
	{
		m_position = m_pos3;
		//m_teleportLocationB = m_position;
	}
	else if (m_currentPos == 3)
	{
		m_position = m_pos4;
		//m_teleportLocationB = m_position;
	}
	else if (m_currentPos == 4)
	{
		m_position = m_pos5;
		//m_teleportLocationB = m_position;
	}
	m_currentPos = rand() % 5 + 1;
	//creates a box2d body for the pickup and defines it proporties
	m_body->m_body = world.createBox(m_position.x, m_position.y, 50, 50, false, false, b2BodyType::b2_staticBody);
	world.addProperties(*m_body->m_body, 0, 0, 0, true, new PhysicsComponent::ColData("Pickup", m_pickupEntity));
	
	static_cast<PositionComponent*>(&m_pickupEntity->getComponent("Pos"))->position = Vector2f(m_position);
}

void PickUpComponent::despawn(Box2DBridge & world)
{
	//despawns the record after its been spawned for 10 seconds
	m_timeTillSpawn = 10; //10 seconds
	m_spawned = false;
	m_currentPos = rand() % 5 + 1;

	world.deleteBody(m_body->m_body);
}