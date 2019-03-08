#include "DJBoothSystem.h"
#include "DJboothComponent.h"
#include "SlowBoothComponent.h"
#include "SpriteComponent.h"
#include "PlatformComponent.h"


void DJBoothSystem::addComponent(Component * comp)
{
	m_components.push_back(comp);
}

void DJBoothSystem::update(double dt)
{
	
	for (auto& comp : m_components)
	{
		auto booth = static_cast<DJBoothComponent*>(comp);

		booth->update(dt);

		//increments the backgorund, platform colour and the in game song
		if (booth->bgSwitch)
		{
			m_currentBg++;
			m_currentPc++;
			m_currentTrack++;

			if (m_currentBg > 3)
				m_currentBg = 0;
			
			if (m_currentPc > 3)
				m_currentPc = 0;

			if (m_currentTrack > 3)
				m_currentTrack = 0;

			//sets the background, music and platform colour
			m_audioPtr->playSound("GameMusic" + std::to_string(m_currentTrack), true);
			auto bgSprite = static_cast<SpriteComponent*>(&m_bgPtr->getComponent("Sprite"));
			bgSprite->setTexture(m_resourcePtr->getTexture("Game BG" + std::to_string(m_currentBg)));

			//Loop through all platforms and floors and switch the colours
			for (auto& plat : *m_platformsPtr)
			{
				auto s = static_cast<SpriteComponent*>(&plat->getComponent("Sprite"));
				s->setTexture(static_cast<PlatformComponent*>(&plat->getComponent("Platform"))->getTexture("Game BG" + std::to_string(m_currentBg)));
			}

			booth->bgSwitch = false;

			
		}
	}
	
}

float DJBoothSystem::getScalar()
{
	for (auto& comp : m_components)
	{
		auto c = dynamic_cast<SlowBoothComponent*>(comp);

		if (nullptr != c)
		{
			return c->getScaler();
		}

	}

	return 1.0f;
}
