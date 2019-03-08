#pragma once
#include "DJboothComponent.h"
#include "CollisionListener.h"
#include "Entity.h"

class PlatformBoothComponent : public DJBoothComponent
{
public:
	PlatformBoothComponent(std::vector<Entity*>* allPlatforms, Entity* pickUp):
		m_entities(allPlatforms),
		m_timer(0),
		m_active(false),
		m_speed(4000),
		m_halfPoint(false),
		m_currentIndex(1),
		DJBoothComponent(pickUp)
	{
		//vectors of positions for each set
		auto firstPoint = Vector2f(960, 540);
		auto secondPoint = Vector2f(2880, 540);
		auto thirdPoint = Vector2f(4800, 540);
		auto firstPair = OffsetPair();
		auto secondPair = OffsetPair();
		auto thirdPair = OffsetPair();
		//this handles which platform sets are moved and at what time
		for (int i = 7; i < m_entities->size(); i++)
		{
			if (i < 9)
			{
				firstPair.m_offsets.push_back(static_cast<PositionComponent&>(m_entities->at(i)->getComponent("Pos")).position - firstPoint);
				firstPair.m_platforms.push_back(m_entities->at(i));
			}
			else if (i < 15)
			{
				secondPair.m_offsets.push_back(static_cast<PositionComponent&>(m_entities->at(i)->getComponent("Pos")).position - secondPoint);
				secondPair.m_platforms.push_back(m_entities->at(i));
			}
			else if (i < 25)
			{
				thirdPair.m_offsets.push_back(static_cast<PositionComponent&>(m_entities->at(i)->getComponent("Pos")).position - thirdPoint);
				thirdPair.m_platforms.push_back(m_entities->at(i));
			}
		}

		m_offsetVectors.push_back(std::make_pair(firstPoint, firstPair));
		m_offsetVectors.push_back(std::make_pair(secondPoint, secondPair));
		m_offsetVectors.push_back(std::make_pair(thirdPoint, thirdPair));
	}
	void run()
	{
		notify(m_pickUp, PLATFORM_MOVE);
		m_active = true;
		m_timer = 20;
		m_halfPoint = false;
		bgSwitch = true;
	}

	void update(double dt)
	{
		if (m_active)
		{
			m_timer -= dt;
			
			if (m_halfPoint == false)
			{
				m_speed -= .175f * dt;

				//for (auto& comp : *m_entities)
				//{
					/*auto platform = static_cast<PhysicsComponent*>(&comp->getComponent("Physics"));

					if (comp->m_ID == "Platform")
					{
						float temp = platform->m_body->getPosition().x - (m_speed * dt);
						platform->m_body->setPosition(temp, platform->m_body->getPosition().y);
					}
					if (platform->m_body->getPosition().x <= -400)
					{
						float temp = platform->m_body->getPosition().y - (m_speed * dt);
						platform->m_body->setPosition(platform->m_body->getPosition().x + 5700, temp);
					}*/
				//}

				if (m_speed <= 0.65f)
				{
					m_speed += .175f * dt;
					m_halfPoint = true;
				}

				
			}
			else if (m_timer <= 2)
			{
				m_speed -= .175 * dt;
			}

			if (m_timer <= 0)
			{
				m_active = false;
				m_speed = 0;
				m_timer = 20;
				m_speed = 100;
			}	
			//this handles the offset of the platforms when the command for moving platfrom is called
			for (auto& pair : m_offsetVectors)
			{
				pair.first.x -= m_speed * dt;

				if (m_offsetVectors.at(m_currentIndex).first.x <= 960)
				{
					m_offsetVectors.at(m_currentIndex).first.x = 960;
					m_currentIndex = 1;
					m_active = false;
					std::sort(m_offsetVectors.begin(), m_offsetVectors.end(), sortFunc());
					
					for (int i = 0; i < 3; i++)
					{
						m_offsetVectors.at(i).first.x = 960 + (i * 1920);
					}
				}

				if (pair.first.x <= -960)
					pair.first.x = 4800;
				for (int i = 0; i< pair.second.m_offsets.size(); i++)
				{
					auto phys = static_cast<PhysicsComponent*>(&pair.second.m_platforms.at(i)->getComponent("Physics"));
					auto newPos = pair.first + pair.second.m_offsets.at(i);
					phys->m_body->setPosition(newPos.x, newPos.y);
				}
			}
		}
	}

	void movePlatforms()
	{

	}
	float& getTimeLeft() { return m_timer; }
	float& getSpeed() { return m_speed; }
private:
	float m_timer;
	bool m_active;
	float m_halfPoint;
	float m_speed;
	std::vector<Entity*>* m_entities;
	struct OffsetPair
	{
	public:
		std::vector<Vector2f> m_offsets;
		std::vector<Entity*> m_platforms;
	};


	struct sortFunc
	{
		bool operator()(std::pair<Vector2f, OffsetPair> a, std::pair<Vector2f, OffsetPair> b)
		{
			return a.first.x < b.first.x;
		}
	};


	std::vector<std::pair<Vector2f, OffsetPair>> m_offsetVectors;
	int m_currentIndex;
};