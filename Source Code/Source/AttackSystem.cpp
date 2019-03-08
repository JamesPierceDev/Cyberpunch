#include "AttackSystem.h"

void AttackSystem::addComponent(Component * comp)
{
	m_components.push_back(comp);
}

void AttackSystem::update(double dt)
{
	for (auto& comp : m_components)
	{
		auto hit = static_cast<AttackComponent*>(comp);

		//If there is a delay on the hit
		if (hit->delay())
		{
			//Take away from the delay
			hit->delayLeft() -= dt;

			if (hit->delayLeft() <= 0)
			{
				//Set spawn attack to true
				hit->spawnAttack() = true;
			}
		}

		//If the hit is to spawn an attack then spawn one
		if (hit->spawnAttack())
		{
			hit->spawn(*m_worldPtr);
		}

		//If a hit time to live is greater than 0,  minus from its time to live
		if (hit->timeToLive() > 0 && hit->delay() <= 0)
		{
			//minus dt from the time to live
			hit->timeToLive() -= dt;

			if (hit->timeToLive() <= 0)
			{
				hit->attackActive() = false;

				if (nullptr != hit->m_currentAttack)
					hit->deleteAttack(*m_worldPtr);
			}
			else
			{
				if (nullptr != hit->m_currentAttack)
					hit->updatePosition();
			}
		}

		//If the bool to destroy an attack is true, then delete the attack body
		if (hit->destroyAttack() && nullptr != hit->m_currentAttack)
		{
			hit->deleteAttack(*m_worldPtr);
		}
	}
}
