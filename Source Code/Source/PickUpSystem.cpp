#include "PickUpSystem.h"
#include "PlayerPhysicsComponent.h"
#include "RenderSystem.h"

void PickUpSystem::setWorld(Box2DBridge & world)
{
	m_worldPtr = &world;
}

void PickUpSystem::addComponent(Component * comp)
{
	m_components.push_back(comp);
}

void PickUpSystem::update(double dt)
{
	//checks the time and either spawns are despawns
	for (auto& comp : m_components)
	{
		auto pickup = static_cast<PickUpComponent*>(comp);
		if (!m_netSysPtr->isConnected || m_netSysPtr->m_isHost)
		{
			if (pickup->getTimeTillSpawn() > 0)
			{
				pickup->getTimeTillSpawn() -= dt;

				if (pickup->getTimeTillSpawn() <= 0)
				{
					pickup->spawn(*m_worldPtr);
					m_renderSysPtr->addComponent(&pickup->getPickupEntity()->getComponent("Sprite"));
					if (m_netSysPtr->isConnected)
						m_netSysPtr->spawnPickup(pickup->m_currentPos);
				}

			}
		}
		else 
		{
			int loc = m_netSysPtr->pickupLocation();
			if (loc != -1)
			{
				pickup->m_currentPos = loc; 
				pickup->spawn(*m_worldPtr);
				m_renderSysPtr->addComponent(&pickup->getPickupEntity()->getComponent("Sprite"));
			}
		}

		//countdown for time in booths
		if (pickup->toTeleportB())
		{
			pickup->getTimeInBooth() -= dt;

			//checks to see if time is up for player in booth, sets teleport to false
			//sets timer back to 10 seconds and moves player back into the game
			if (pickup->getTimeInBooth() <= 0 || pickup->m_end)
			{
				auto p = static_cast<PlayerPhysicsComponent*>(&pickup->getPlayer()->getComponent("Player Physics"));
				pickup->toTeleport() = false;
				pickup->toTeleportB() = false;
				p->m_body->setPosition(pickup->getTeleportLocationB().x, pickup->getTeleportLocationB().y);
				p->m_jumpSensor->setPosition(pickup->getTeleportLocationB().x, pickup->getTeleportLocationB().y);
				pickup->getTimeInBooth() = 10;
			}

		}

		if (pickup->spawned())
		{
			
			//If ye have to teleport a player
			if (pickup->toTeleport())
			{
				// sends player to booth, despawns the pickup and sets the timer to countdown
				auto p = static_cast<PlayerPhysicsComponent*>(&pickup->getPlayer()->getComponent("Player Physics"));
				pickup->getTeleportLocationB() = p->m_body->getPosition();
				auto teleLoc = pickup->getTeleportLocation();
				p->m_body->setPosition(teleLoc.x, teleLoc.y);
				p->m_jumpSensor->setPosition(teleLoc.x, teleLoc.y);
				pickup->despawn(*m_worldPtr);
				m_renderSysPtr->deleteComponent(&pickup->getPickupEntity()->getComponent("Sprite"));
				pickup->toTeleportB() = true;
				pickup->getTimeInBooth() = 10;
			}

			//despawns the pickup after ten seconds if not collected
			if (pickup->getTimeLive() > 0)
			{
				pickup->getTimeLive() -= dt;

				if (pickup->getTimeLive() <= 0)
				{
					pickup->despawn(*m_worldPtr);
					m_renderSysPtr->deleteComponent(&pickup->getPickupEntity()->getComponent("Sprite"));
				}
			}

		}


		
	}

}


