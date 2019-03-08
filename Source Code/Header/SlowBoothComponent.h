#pragma once
#pragma once
#include "DJboothComponent.h"
#include "CollisionListener.h"
#include "DJboothComponent.h"
#include "Entity.h"
#include "PlayerPhysicsComponent.h"

class SlowBoothComponent : public DJBoothComponent
{
public:
	SlowBoothComponent(Entity* pickUp) :
		m_timer(0),
		m_active(false), m_scalar(1),
		m_halfPoint(false),
		DJBoothComponent(pickUp)
	{

	}
	void run()
	{
		notify(m_pickUp, SLOW_DOWN);
		m_active = true;
		m_timer = 15;
		m_halfPoint = false;
		bgSwitch = true;
	}

	void update(double dt)
	{
		//If run was called
		//if active will start slowing down dt this is time depended and will gradually slow down
		//and gradually speed up again
		if (m_active)
		{
			m_timer -= dt;
			
			if (m_halfPoint == false)
			{
				m_scalar -= .175f * dt;

				if (m_scalar <= 0.65f)
				{
					m_scalar = 0.65f;
					m_halfPoint = true;
				}
			}
			else if(m_timer <= 2)
			{
				m_scalar += .175f * dt;
			}

			if (m_timer <= 0)
			{
				m_active = false;
				m_scalar = 1;
			}
		}		
	}


	float& getTimeLeft() { return m_timer; }
	float& getScaler() { return m_scalar;  }
private: 
	bool m_active = true;
	float m_timer;
	float m_scalar;
	bool m_halfPoint;
};