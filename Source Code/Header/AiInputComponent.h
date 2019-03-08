#ifndef AIINPUTCOMPONENT_H
#define AIINPUTCOMPONENT_H

#include "InputComponent.h"
#include "Commands.h"
#include "Entity.h"

class AiInputComponent : public InputComponent
{
public:
	AiInputComponent() :
		m_left(false),
		m_right(false),
		InputComponent()
	{
	}

	void handleInput(std::string s, Entity * e)
	{
		m_currentCMD = nullptr;

		if (aiIsButtonPressed("YBTN")) //Y
		{
			m_currentCMD = &m_jumpCMD;
		}
		else if (aiIsButtonPressed("XBTN")) //X
		{
			m_currentCMD = &m_punchCMD;
		}
		else if (isButtonHeld("RBBTN"))
		{
			m_currentCMD = &m_superCMD;
		}
		else if (aiIsButtonPressed("ABTN")) //A
		{
			m_currentCMD = &m_kickCMD;
		}
		else if (aiIsButtonPressed("STICKDOWN")) //Down
		{
			m_currentCMD = &m_phaseDownCMD;
		}
		else if (aiIsButtonHeld("STICKLEFT")) //Left
		{
			m_currentCMD = &m_moveLeftCMD;
		}
		else if (aiIsButtonHeld("STICKRIGHT")) //Right
		{
			m_currentCMD = &m_moveRightCMD;
		}

		//If a command is active, execute it
		if (nullptr != m_currentCMD)
		{
			m_currentCMD->execute(*e);
		} //Otherwise execute the idle command
		else if (static_cast<AttackComponent *>(&e->getComponent("Attack"))->attackActive() == false
			&& !(static_cast<AnimationComponent *>(&e->getComponent("Animation"))->getCurrentID() == "Jump"
				&& static_cast<AnimationComponent *>(&e->getComponent("Animation"))->getCurrentAnimation()->getCompleted() == false)
			&& static_cast<PlayerPhysicsComponent *>(&e->getComponent("Player Physics"))->stunned() == false)
		{
			m_idleCMD.execute(*e);
		}

		m_previousCMD = m_currentCMD;
	}
private:
	bool aiIsButtonHeld(std::string btn)
	{
		return m_previous[btn];
	}
	bool aiIsButtonPressed(std::string btn)
	{
		return m_previous[btn] && !m_current[btn];
	}
	bool aiIsButtonPreviouslyPressed(std::string btn)
	{
		return m_previous[btn];
	}

	JumpCommand m_jumpCMD;
	MoveLeftCommand m_moveLeftCMD;
	MoveRightCommand m_moveRightCMD;
	PunchCommand m_punchCMD;
	KickCommand m_kickCMD;
	UppercutCommand m_uppercutCMD;
	PhaseDownCommand m_phaseDownCMD;
	SuperCommand m_superCMD;
	IdleCommand m_idleCMD;
	Command * m_currentCMD;
	Command * m_previousCMD;
	bool m_left, m_right;
};

#endif