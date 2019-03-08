#include "AttackComponent.h"

AttackComponent::AttackComponent() :
	m_e(nullptr),
	m_spawnAttack(false),
	m_attackActive(false),
	m_startDelay(false),
	m_attacked(false),
	m_ttl(0),
	m_delay(0),
	m_xImpulse(0),
	m_yImpulse(0),
	m_dmg(0),
	m_currentAttackTag("")
{
}

void AttackComponent::attack(Vector2f offset, Vector2f size, Entity & e, std::string tag, float ttl, std::string attackName, float delay = 0)
{
	m_currentAttackTag = attackName;
	m_attacked = true;
	m_startDelay = true;
	m_ttl = ttl;
	m_tag = tag;
	m_delay = delay; //Set the start delay
	m_offset = offset; //The offset of where it is positioned
	m_size = size; //The size of the attack
	m_e = &e; //Set the entity to spawn from
}

void AttackComponent::setAttackProperties(int dmg, float xImpulse, float yImpulse)
{
	m_dmg = dmg;
	m_xImpulse = xImpulse;
	m_yImpulse = yImpulse;
}

void AttackComponent::spawn(Box2DBridge& world)
{
	m_spawnAttack = false;
	m_startDelay = false;
	m_attackActive = true;

	//get the players position
	auto playerPos = static_cast<PositionComponent*>(&m_e->getComponent("Pos"))->position;

	//Create the new attack
	m_currentAttack = new PhysicsComponent(new PositionComponent(0,0));

	m_currentAttack->m_body = world.createBox(playerPos.x + m_offset.x, playerPos.y + m_offset.y,
		m_size.x, m_size.y, false, false, b2BodyType::b2_dynamicBody);
	world.addProperties(*m_currentAttack->m_body, 0, 0, 0, true, new PhysicsComponent::ColData(m_tag, m_e));

	//Make attack body not affected by gravity
 	m_currentAttack->m_body->getBody()->SetGravityScale(0);
}

void AttackComponent::deleteAttack(Box2DBridge & world)
{
	m_destroyAttack = false;
	m_attacked = false;

	//Reset attack properties
	m_dmg = 0;
	m_xImpulse = 0;
	m_yImpulse = 0;
	
	world.deleteBody(m_currentAttack->m_body);

	//Delete the current attack
	delete m_currentAttack;
	//Set current attack to nullptr
	m_currentAttack = nullptr;
}

void AttackComponent::updatePosition()
{
	auto playerPos = static_cast<PlayerPhysicsComponent*>(&m_e->getComponent("Player Physics"));
	m_currentAttack->m_body->setPosition(playerPos->m_body->getPosition().x + m_offset.x, playerPos->m_body->getPosition().y + m_offset.y);
}


