#ifndef ACTION_H
#define ACTION_H

#include "BehaviourTree.h"
#include "Entity.h"
#include "Component.h"
#include "PositionComponent.h"
#include "AIComponent.h"
#include "AiInputComponent.h"
#include "PlayerPhysicsComponent.h"

/// <summary>
/// Action base class. All other concrete action
/// classes are derived from this, and their function
/// overridden from this.
/// Pattern resembles command pattern.
/// </summary>
class Action : public BehaviourTree::Node
{
public:
	Action(Entity * e, AiInputComponent * a) :
	m_entity(e),
	m_input(a)
	{}
	//Run function called by the behaviour tree, overridden by the concrete actions
	virtual bool run() = 0; 
	//Stores a reference to the entity that owns the AI component
	Entity * m_entity; 
	//Stores a reference to the AIinput component for moving the AI player
	AiInputComponent * m_input; 
};

#endif

#ifndef WALKLEFTACTION_H
#define WALKLEFTACTION_H

class WalkLeftAction : public Action
{
public:
	WalkLeftAction(Entity * e, AiInputComponent * a) :
		Action(e, a)
	{
	}

	/// <summary>
	/// Run function overridden from Action base class
	/// function. When called moves the player left
	/// by calling the stickleft command in the inputhandler
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Passes STICKLEFT command to the AI input handler
		m_input->m_current["STICKLEFT"] = true;
		//Return true to maintain behaviour tree flow of execution
		return true;
	}
};

#endif

#ifndef WALKRIGHTACTION_H
#define WALKRIGHTACTION_H

class WalkRightAction : public Action
{
public:
	WalkRightAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	/// <summary>
	/// Run function overridden from Action base class
	/// function. When called moves the player right
	/// by calling the stickright command in the inputhandler
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Passes STICKRIGHT command to the AI input handler
		m_input->m_current["STICKRIGHT"] = true;
		//return function as true to keep tree runnning
		return true;
	}
};

#endif

#ifndef JUMPACTION_H
#define JUMPACTION_H

class JumpAction : public Action
{
public:
	JumpAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}
	/// <summary>
	/// Run function overridden from Action base class
	/// function. When called makes the AI player jump
	/// by calling the YBTN command in the inputhandler
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		auto phys = static_cast<PlayerPhysicsComponent *>(&m_entity->getComponent("Player Physics"));
		//Passes YBTN command to AI input handler
		if (phys->canJump())
		{
			m_input->m_current["YBTN"] = true;
		}
		return true;
	}
};

#endif

#ifndef PUNCHACTION_H
#define PUNCHACTION_H

class PunchAction : public Action
{
public:
	PunchAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	/// <summary>
	/// Makes the AI player jump by passing the XBTN command
	/// to the AI input handler object.
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		auto p = static_cast<PlayerPhysicsComponent *>(&m_entity->getComponent("Player Physics"));

		m_input->m_current["ABTN"] = false;
		m_input->m_current["XBTN"] = true;

		return true;
	}
};

#endif

#ifndef KICKACTION_H
#define KICKACTION_H

class KickAction : public Action
{
public:
	KickAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	bool run() override
	{
		m_input->m_current["XBTN"] = false;
		m_input->m_current["ABTN"] = true;
		return true;
	}
};

#endif

#ifndef CHECKNEAREST_H
#define CHECKNEAREST_H

class CheckNearest : public Action
{
public:
	CheckNearest(std::vector<Entity *>* e, Entity * s, AiInputComponent * a) :
		m_entities(e), Action(s, a)
	{
	}

	/// <summary>
	/// Behaviour tree run function overidden from Action base class.
	/// Loops through vector of all players in the current game and 
	/// finds the one nearest to the current AI entity.
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Container to store nearest entity
		auto nearest = new Entity("temp");

		//If entities vector is not empty, set nearest to first element
		if (!m_entities->empty())
		{
			nearest = m_entities->at(0);

			//Get position component of current AI entity
			auto self_pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));
			//Get position component from entity
			auto otherPos = dynamic_cast<PositionComponent *>(&nearest->getComponent("Pos"));

			//Loop through all entities
			for (auto entity : *m_entities)
			{
				if (entity != m_entity)
				{
					//Get position component of the current entity in the loop
					auto newPos = dynamic_cast<PositionComponent *>(&entity->getComponent("Pos"));
					//Check distance between self and previous position, and self and new position
					if (dist(newPos->position, self_pos->position) < dist(otherPos->position, self_pos->position))
					{
						//If new position is closer, the entity it belongs to is assigned to nearest
						nearest = entity;
					}
				}
			}
			//Cast AI component
			auto e = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));
			//Assign nearest player to variable in the AI 
			e->nearestPlayer = nearest;
			//delete nearest;
			return true;
		}
		return false;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
	std::vector<Entity *>* m_entities;
};
#endif

#ifndef CLOSEENOUGH_H
#define CLOSEENOUGH_H

class CloseEnough : public Action
{
public:
	CloseEnough(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));
		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));
		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));
		//Return true if dist between two entities is less than 100
		return dist(pos->position, nearest->position) < 50 ? true : false;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
};

#endif

#ifndef CHECKPLAYERDIRECTION_H
#define CHECKPLAYERDIRECTION_H

class CheckPlayerDirection : public Action
{
public:
	CheckPlayerDirection(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	/// <summary>
	/// Run function overidden from the Action base class
	/// casts a vector from the AI entity to the nearest
	/// player as one of 4 directions
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));

		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));

		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));

		//Check if the nearest player is to the left of AI
		if (nearest->position.x < pos->position.x)
		{
			//move left
			m_input->m_current["STICKLEFT"] = true;
		} //Check if the nearest player is to the right of AI
		else if (nearest->position.x > pos->position.x)
		{
			//move right
			m_input->m_current["STICKRIGHT"] = true;
		}
		else //If player and AI are on the same X coord, jump
		{
			//jump
			//m_input->m_current["YBTN"] = true;
		}
		//Return function as true to continue tree iteration
		return true;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
};

#endif

#ifndef CHECKHEALTH_H
#define CHECKHEALTH_H

class CheckHealth : public Action
{
public:
	CheckHealth(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	/// <summary>
	/// Run function overidden from the Action base class function.
	/// Down casts the self entity component to a play physics component.
	/// Returns true if the current damage percentage is greater than 50
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		auto p = dynamic_cast<PlayerPhysicsComponent *>(&m_entity->getComponent("Player Physics"));
		return p->damagePercentage() > 50 ? true : false;
	}
};

#endif

#ifndef FLEEACTION_H
#define FLEEACTION_H

class FleeAction : public Action
{
public:
	FleeAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{
	}

	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));

		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));

		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));

		//While the distance to the nearest enemy is less than 200
		if (dist(pos->position, nearest->position) < 200)
		{
			//If they're left of AI, move right
			if (nearest->position.x < pos->position.x)
			{
				m_input->m_current["STICKRIGHT"] = true;
			}
			else //otherwise move left
			{
				m_input->m_current["STICKLEFT"] = true;
			}
		}
		return true;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
};

#endif

#ifndef CHECKABOVE_H
#define CHECKABOVE_H

class CheckAbove : public Action
{
public:
	CheckAbove(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{}

	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));

		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));

		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));

		if (nearest->position.y < pos->position.y - 100)
		{
			if (dist(pos->position, nearest->position) < 150)
			{
				auto a = new JumpAction(m_entity, m_input);
				a->run();
				delete a;
			}
		}
		else if (nearest->position.y > pos->position.y + 200)
		{
			m_input->m_current["STICKDOWN"] = true;
		}
		else
		{
			return true;
		}
		return true;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
};

#endif

#ifndef CHECKPLAYERHEALTH_H
#define CHECKPLAYERHEALTH_H

class CheckPlayerHealth : public Action
{
public:
	CheckPlayerHealth(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{}

	/// <summary>
	/// Run function overidden from Action base class.
	/// Checks if the nearest players damage percentage
	/// is high enough to attack them
	/// </summary>
	/// <returns></returns>
	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));

		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PlayerPhysicsComponent *>(&comp->nearestPlayer->getComponent("Player Physics"));

		//Return true if nearest players damage % is greater than 50
		return nearest->damagePercentage() > 50 ? true : false;
	}
};

#endif

#ifndef DROPACTION_H
#define DROPACTION_H

class DropAction : public Action
{
public:
	DropAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{}

	bool run() override
	{
		m_input->m_current["STICKDOWN"] = true;
		return true;
	}
};

#endif

#ifndef DJACTION_H
#define DJACTION_H

class DJAction : public Action
{
public:
	DJAction(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{}

	bool run() override
	{
		auto acomp = static_cast<AIComponent *>(&m_entity->getComponent("AI"));
		auto p = static_cast<PlayerComponent *>(&m_entity->getComponent("Player"));
		auto pos = static_cast<PositionComponent *>(&m_entity->getComponent("Pos"));
		auto phys = static_cast<PlayerPhysicsComponent *>(&m_entity->getComponent("Player Physics"));
		int index = random(1, 3);
		Vector2f positionOne = Vector2f(1355, 80);
		Vector2f positionTwo = Vector2f(1035, 80);
		Vector2f positionThree = Vector2f(715, 80);

		if (p->isDJ())
		{
			switch (index)
			{
			case 1:
				phys->posPtr->position = positionOne;
				break;
			case 2:
				phys->posPtr->position = positionTwo;
				break;
			case 3:
				phys->posPtr->position = positionThree;
				break;
			}

			if (acomp->timer > 2)
			{
				auto a = new PunchAction(m_entity, m_input);
				a->run();
				delete a;
				acomp->timer = 0;
			}
			
		}
		acomp->timer += 1 / 60.f;
		p->setDJ(false);
		return true;
	}

	int random(int min, int max)
	{
		double x = rand() / static_cast<double>(RAND_MAX + 1);
		int val = min + static_cast<int>(x * (max - min));
		return val;
	}
};

#endif

#ifndef MOVETOPLAYER_H
#define MOVETOPLAYER_H

class MoveToPlayer : public Action
{
public:
	MoveToPlayer(Entity * e, AiInputComponent * a)
		: Action(e, a)
	{}

	bool run() override
	{
		//Get AI component
		auto comp = dynamic_cast<AIComponent *>(&m_entity->getComponent("AI"));
		//Cast self component to PositionComponent
		auto pos = dynamic_cast<PositionComponent *>(&m_entity->getComponent("Pos"));
		//Cast nearest player entity from Ai component to Position component
		auto nearest = dynamic_cast<PositionComponent *>(&comp->nearestPlayer->getComponent("Pos"));

		auto phys = static_cast<PlayerPhysicsComponent *>(&m_entity->getComponent("Player Physics"));

		//If the AI is more than 50 pixels from the player
		//dist(nearest->position, pos->position) > 50
		//If AI is right of player
		if (nearest->position.x < pos->position.x - 50)
		{
			//Move left
			m_input->m_current["STICKRIGHT"] = false;
			m_input->m_current["STICKLEFT"] = true;
		}
		else if (nearest->position.x > pos->position.x + 50) //AI is left of player
		{
			//Move right
			m_input->m_current["STICKLEFT"] = false;
			m_input->m_current["STICKRIGHT"] = true;
		}
		else
		{
			if (phys->isOnPlayer())
			{
				auto f = new FleeAction(m_entity, m_input);
				f->run();
				delete f;
			}
		}

		if (comp->onEdgeRight)
		{
			m_input->m_current["STICKLEFT"] = false;
			m_input->m_current["STICKRIGHT"] = true;
			auto a = new JumpAction(m_entity, m_input);
			a->run();
			delete a;
			comp->onEdgeRight = false;
		}
		else if (comp->onEdgeLeft)
		{
			m_input->m_current["STICKRIGHT"] = false;
			m_input->m_current["STICKLEFT"] = true;
			auto a = new JumpAction(m_entity, m_input);
			a->run();
			delete a;
			comp->onEdgeLeft = false;
		}
		return true;
	}

	//Euclidean distance function
	float dist(Vector2f p1, Vector2f p2)
	{
		return sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
	}
};

#endif