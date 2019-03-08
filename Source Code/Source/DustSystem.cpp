#include "DustSystem.h"
#include "PlayerPhysicsComponent.h"
#include "RenderSystem.h"
#include "AnimationComponent.h"

DustSystem::DustSystem(std::map<std::string, System*>* systems, std::vector<Entity*>* players, ResourceHandler* rH) :
	m_systems(systems),
	m_players(players),
	m_resourcesPtr(rH)
{
	for (int i = 0; i < 20; i++)
	{
		m_dustFrames.push_back({ i * 90, 0, 90, 50 });
	}
}

void DustSystem::addComponent(Component * c)
{
	m_components.push_back(c);
}

void DustSystem::update(double dt)
{
	//Loops through all the players
	for (auto& player : *m_players)
	{
		if (static_cast<DustTriggerComponent*>(&player->getComponent("Dust Trigger"))->toCreate())
		{
			auto pPhys = static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"));
			static_cast<DustTriggerComponent*>(&player->getComponent("Dust Trigger"))->toCreate() = false; //Reset the trigger

			auto d = new Entity("Dust");
			auto pos = new PositionComponent(pPhys->m_body->getPosition().x, pPhys->m_body->getPosition().y + 25);
			d->addComponent("Pos", pos);
			d->addComponent("Dust", new DustComponent());
			d->addComponent("Sprite", new SpriteComponent(pos, Vector2f(1800, 50), Vector2f(90, 50), m_resourcesPtr->getTexture("Player Dust"), 1));
			auto anim = new AnimationComponent(&d->getComponent("Sprite"));
			anim->addAnimation("Destroy", m_resourcesPtr->getTexture("Player Dust"), m_dustFrames, .5f);
			anim->playAnimation("Destroy", false);
			d->addComponent("Animation", anim);
			(*m_systems)["Animation"]->addComponent(anim);
			(*m_systems)["Render"]->addComponent(&d->getComponent("Sprite"));

			//Add the dust particle to the vector
			m_dustParticles.push_back(d);
		}
	}

	//Loop through all dust particles and check if they need to be deleted
	for (auto& dust : m_dustParticles)
	{
		auto dComp = static_cast<DustComponent*>(&dust->getComponent("Dust"));

		dComp->getTTL() -= dt;

		//If the time to live is up, delete the dust particle from the systems
		if (dComp->getTTL() <= 0)
		{
			m_dustToDelete.push_back(dust); //Add to the vector to delete
		}
	}

	//If the dust to delete is not empty, then delete them all
	if (!m_dustToDelete.empty())
	{
		for (auto& dust : m_dustToDelete)
		{
			(*m_systems)["Animation"]->deleteComponent(&dust->getComponent("Animation"));
			(*m_systems)["Render"]->deleteComponent(&dust->getComponent("Sprite"));
			m_dustParticles.erase(std::remove(m_dustParticles.begin(), m_dustParticles.end(), dust), m_dustParticles.end());

			for (auto& c : dust->m_components)
			{
				delete c.second;
			}

			delete dust;
		}

		m_dustToDelete.clear();
	}
}
