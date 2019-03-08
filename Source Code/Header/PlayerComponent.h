#pragma once
#include "Vector2f.h"
#include "Entity.h"
#include "PlayerPhysicsComponent.h"
#include "OnlineSendComponent.h"
#include "AudioComponent.h"

class PlayerComponent : public Component
{
public:
	PlayerComponent(std::vector<Vector2f> locations, Entity* player, int index) :
		m_dead(false),
		m_lives(3),
		m_newSpawn(nullptr),
		m_playerPtr(player),
		m_spawnLocations(locations),
		m_respawn(false),
		m_respawning(false),
		m_winner(false),
		m_playerIndex(index),
		m_dmgDealt(0),
		m_dmgTaken(0),
		m_supersUsed(0),
		m_hitBy(nullptr),
		m_hitWith(""),
		m_superPercentSpeed(0.05f)
	{

	}

	void respawn()
	{
		//Set the player to respawn
		m_lives--;


		if (m_hitWith == "Kick" && m_playerPtr->m_ID == "AI")
			achi::Listener::notify(nullptr, KICK_DEATH);
		
		if (m_playerPtr->m_ID == "AI" && (nullptr != m_hitBy))
		{
			if (m_hitBy->m_ID != "AI")
			{
				achi::Listener::notify(nullptr, AI_DEATH);
				achi::Listener::notify(nullptr, PLAYER_DEATH);
			}
		}

		//If the player is not the winner
		if (m_lives < 1 && m_winner == false)
		{
			m_dead = true;
		}

		//If the player is not dead, pick a spawn location
		if (m_dead == false)
		{
			m_respawning = true;
			m_respawn = true;
			m_spawnTimer = 2.5f; //Respawn after 2.5 seconds
			m_newSpawn = &m_spawnLocations.at(rand() % m_spawnLocations.size()); //Number between 0 and the size of the amount of spawn points	
		}
		static_cast<AudioComponent&>(m_playerPtr->getComponent("Audio")).playSound("KnockOut", false);
		auto net = static_cast<OnlineSendComponent*>(&m_playerPtr->getComponent("Send"));
		if (net != NULL)
		{
			auto phys = static_cast<PlayerPhysicsComponent*>(&m_playerPtr->getComponent("Player Physics"));
			net->addCommand("RESPAWN");
			net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
		}
	}

	void spawnPlayer()
	{
		m_respawn = false;
		m_respawning = false;
		m_spawnTimer = 0;
		//m_audio.playSound("Spawn", true);
		static_cast<AudioComponent&>(m_playerPtr->getComponent("Audio")).playSound("Spawn", false);
		auto phys = static_cast<PlayerPhysicsComponent*>(&m_playerPtr->getComponent("Player Physics"));
		//Set the players position to the new position
		phys->m_body->setPosition(m_newSpawn->x, m_newSpawn->y);
		phys->m_jumpSensor->setPosition(m_newSpawn->x, m_newSpawn->y);
		phys->m_body->getBody()->SetAngularVelocity(0);
		phys->m_body->getBody()->SetLinearVelocity(b2Vec2(0,0));
		phys->m_jumpSensor->getBody()->SetAngularVelocity(0);
		phys->m_jumpSensor->getBody()->SetLinearVelocity(b2Vec2(0, 0));
		phys->damagePercentage() = 0; //Reset the damage percentage

		m_hitWith = "";
		m_hitBy = nullptr;
	}

	Vector2f getSpawnLocation() { return *m_newSpawn; }
	Entity* getPlayer() { return m_playerPtr; }
	float& getTimer() { return m_spawnTimer; }
	int& getLives() { return m_lives; }
	bool& isDead() { return m_dead; }
	bool& toRespawn() { return m_respawn; }
	bool& isRespawning() { return m_respawning; }
	bool& isWinner() { return m_winner; }
	bool& isDJ() { return inDJBooth; }
	void setDJ(bool c) { inDJBooth = c; }
	int m_dmgTaken, m_dmgDealt, m_timesStunned, m_timesSuperStunned, m_supersUsed;
	int m_playerIndex;
	Entity* m_hitBy;
	std::string m_hitWith;
	float m_superPercentSpeed;
private:
	float m_spawnTimer;
	Entity * m_playerPtr;
	Vector2f* m_newSpawn;
	std::vector<Vector2f> m_spawnLocations;
	bool m_dead, m_respawn, m_respawning, m_winner;
	int m_lives;
	bool m_audioCreated;
	bool inDJBooth = false;
	AudioComponent m_audio;
};