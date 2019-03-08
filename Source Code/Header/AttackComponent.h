#pragma once
#ifndef ATTACKCOMPONENT_H
#define ATTACKCOMPONENT_H
#include "PhysicsComponent.h"
#include "PlayerPhysicsComponent.h"
#include "Box2DBridge.h"
#include "Entity.h"
#include "Vector2f.h"

class AttackComponent : public Component
{
public:
	AttackComponent();

	//Spawns an attack positioned at offset with a size of size and it belongs to the entity
	//and the attack is tagged so we know what it is (punch, kick) and the time to live for that attack
	//And the delay on when the attack should start
	void attack(Vector2f offset, Vector2f size, Entity& e, std::string tag, float ttl, std::string attackName, float delay);
	void setAttackProperties(int dmg, float xImpulse, float yImpulse);
	void spawn(Box2DBridge& world); //Spawns an attack
	void deleteAttack(Box2DBridge& world); //Deletes the attack
	void updatePosition();

	//Setters
	void setDestoryAttack(bool b) { m_destroyAttack = b; };

	//Getters
	bool& spawnAttack() { return m_spawnAttack; }
	bool& attacked() { return m_attacked; }
	bool& attackActive() { return m_attackActive; }
	bool& destroyAttack() { return m_destroyAttack; }
	bool& delay() { return m_startDelay; }
	float& delayLeft() { return m_delay; }
	float& timeToLive() { return m_ttl; }
	float& yImpulse() { return m_yImpulse; }
	float& xImpulse() { return m_xImpulse; }
	int& damage() { return m_dmg; }
	Entity* entity() { return m_e; }
	PhysicsComponent * m_currentAttack; //A pointer to the attack
	std::string m_currentAttackTag;
private:
	float m_delay, m_xImpulse, m_yImpulse;
	int m_dmg;
	bool m_startDelay, m_attacked, m_delete;
	float m_ttl; //Time to live for the attack
	Vector2f m_offset, m_size; //The offset and size for the attack
	Entity* m_e; //The entity the attack belongs to
	std::string m_tag; //What to tag the attack as
	bool m_spawnAttack; //Bool to hold wheter to spawn the attack or not
	bool m_attackActive; //Bool to hold that the attack is active
	bool m_destroyAttack;
};
#endif