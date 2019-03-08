#include "UISystem.h"
#include "PlayerComponent.h"
#include "PlayerPhysicsComponent.h"
#include "SpriteComponent.h"

UISystem::UISystem(std::map<Entity*, Entity*>* uimap) :
	m_map(uimap)
{
}

void UISystem::addComponent(Component * c)
{
}

void UISystem::update(double dt)
{
	for (auto& pair : *m_map)
	{
		//Get a pointer to the player the ui belongs to
		auto player = pair.first;
		//Get a pointer to the ui entity that belongs to the player
		auto ui = pair.second;

		auto dmg = std::to_string(static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"))->damagePercentage());
		auto p = static_cast<PlayerComponent*>(&player->getComponent("Player"));

		if (static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"))->isSupered())
		{
			auto phys = static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"));


			if (phys->superPercentage() > 0)
			{
				p->m_superPercentSpeed -= dt;

				if (p->m_superPercentSpeed <= 0)
				{
					p->m_superPercentSpeed = 0.05f;
					phys->superPercentage()--; //Take one off of the super percentage
				}
			}
		}
		else
		{
			p->m_superPercentSpeed = 0.05f;
		}

		auto super = std::to_string(static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"))->superPercentage());

		if(dmg.size() < 3)
			dmg.insert(dmg.begin(), 3 - dmg.length(), '0');
		if (super.size() < 3)
			super.insert(super.begin(), 3 - super.length(), '0');

		//Updating damage number
		for (int i = 0; i < 3; i++)
		{
			auto dNum =  (int)dmg.at(i) - 48;
			auto s = static_cast<SpriteComponent*>(&ui->getComponent("Dmg" + std::to_string(i)));
			s->setTextureRect({32 *dNum,0,32,32});
		}
		//Updating super number
		for (int i = 0; i < 3; i++)
		{
			auto sNum = (int)super.at(i) - 48;
			auto s = static_cast<SpriteComponent*>(&ui->getComponent("Sup" + std::to_string(i)));
			s->setTextureRect({ 32 * sNum,0,32,32 });
		}
	}
}
