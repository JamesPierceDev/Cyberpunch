#include "PlayerRespawnSystem.h"
#include "PlayerPhysicsComponent.h"

PlayerRespawnSystem::PlayerRespawnSystem()
{
}

void PlayerRespawnSystem::addComponent(Component * c)
{
	m_components.push_back(c);
}

void PlayerRespawnSystem::update(double dt)
{
	for (auto& c : m_components)
	{
		auto cComp = static_cast<PlayerComponent*>(c);

		//If we have to respawn a player
		if (cComp->toRespawn())
		{
			//Minus dt from our spawn timer
			cComp->getTimer() -= dt;

			//If the time rhas reached 0, spawn the player again
			if (cComp->getTimer() < 0)
			{
				cComp->spawnPlayer();
			}
		}
	}
}
