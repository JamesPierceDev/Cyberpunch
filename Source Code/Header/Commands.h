#ifndef COMMANDS_H
#define COMMANDS_H

#include "Entity.h"
#include "SpriteComponent.h"
#include "PlayerPhysicsComponent.h"
#include "OnlineSendComponent.h"
#include "AttackComponent.h"
#include "AnimationComponent.h"
#include "Observer.h"

class Command : public Subject
{
public:
	virtual ~Command() {}
	virtual void execute(Entity& e) = 0;
};

class JumpCommand : public Command
{
public:
	JumpCommand() {}
	void execute(Entity& e)
	{
		auto hit = static_cast<AttackComponent*>(&e.getComponent("Attack"));
		auto phys = static_cast<PlayerPhysicsComponent*>(&e.getComponent("Player Physics"));
		
		if (hit->attackActive() == false && phys->stunned() == false)
		{
			auto net = static_cast<OnlineSendComponent*>(&e.getComponent("Send"));
			if (net != NULL)
			{
				net->addCommand("JUMP");
				net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
			}
			//If the physics component can jump, then jump
			if (phys->canJump())
			{
				phys->jump();
				auto a = static_cast<AnimationComponent*>(&e.getComponent("Animation"));
				a->playAnimation("Jump", false);
				a->getCurrentAnimation()->resetAnimation();
				auto s = static_cast<SpriteComponent*>(&e.getComponent("Sprite"));
				s->setTexture(a->getCurrentAnimation()->getTexture());
				auto p = static_cast<AudioComponent*>(&e.getComponent("Audio"));
				p->playSound("Jump", false);
			}

		}
	}
};

class MoveLeftCommand : public Command
{
public:
	MoveLeftCommand() {}
	void execute(Entity& e)
	{
		auto hit = static_cast<AttackComponent*>(&e.getComponent("Attack"));
		auto phys = static_cast<PlayerPhysicsComponent*>(&e.getComponent("Player Physics"));

		//Only perform the command if the playe ris not stunned
		if (phys->stunned() == false)
		{
			if (hit->attackActive() == false || !phys->canJump())
			{
				auto net = static_cast<OnlineSendComponent*>(&e.getComponent("Send"));
				if (net != NULL)
				{
					net->addCommand("MOVE LEFT");
					net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
				}
				phys->moveLeft();
				//Play run animation
				auto a = static_cast<AnimationComponent*>(&e.getComponent("Animation"));

				//Play run if we are not playing th ejump animation and it isnt completed yet
				if (!(a->getCurrentID() == "Jump" && a->getCurrentAnimation()->getCompleted() == false))
				{
					a->playAnimation("Run", true);
					auto p = static_cast<AudioComponent*>(&e.getComponent("Audio"));
					p->playSound("Footsteps", true);
				}
				auto s = static_cast<SpriteComponent*>(&e.getComponent("Sprite"));
				s->setScale(1, s->getScale().y);
				s->setTexture(a->getCurrentAnimation()->getTexture());
			}
			else if(hit->attackActive() == false)
				static_cast<AnimationComponent*>(&e.getComponent("Animation"))->playAnimation("Idle", true);
		}

	}
};

class MoveRightCommand : public Command
{
public:
	MoveRightCommand() {}
	void execute(Entity& e)
	{
		//get the attack component from the entity
		auto hit = static_cast<AttackComponent*>(&e.getComponent("Attack"));
		auto phys = static_cast<PlayerPhysicsComponent*>(&e.getComponent("Player Physics"));

		//Only perform the command if the playe ris not stunned
		if (phys->stunned() == false)
		{
			auto net = static_cast<OnlineSendComponent*>(&e.getComponent("Send"));
			if (net != NULL)
			{
				net->addCommand("MOVE RIGHT");
				net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
			}
			if (hit->attackActive() == false || !phys->canJump())
			{
				phys->moveRight();
				auto a = static_cast<AnimationComponent*>(&e.getComponent("Animation"));
				//Play run if we are not playing th ejump animation and it isnt completed yet
				if (!(a->getCurrentID() == "Jump" && a->getCurrentAnimation()->getCompleted() == false))
				{
					a->playAnimation("Run", true);
					auto p = static_cast<AudioComponent*>(&e.getComponent("Audio"));
					p->playSound("Footsteps", true);
				}
				auto s = static_cast<SpriteComponent*>(&e.getComponent("Sprite"));
				s->setScale(-1, s->getScale().y);
				s->setTexture(a->getCurrentAnimation()->getTexture());
			}
			else if(hit->attackActive() == false)
				static_cast<AnimationComponent*>(&e.getComponent("Animation"))->playAnimation("Idle", true);
		}

	}
};

class PunchCommand : public Command
{
public:
	PunchCommand() {}
	void execute(Entity& e)
	{
		//get the attack component from the entity
		auto hit = static_cast<AttackComponent*>(&e.getComponent("Attack"));
		auto phys = static_cast<PlayerPhysicsComponent*>(&e.getComponent("Player Physics"));

		if (hit->attackActive() == false && phys->stunned() == false)
		{
			auto net = static_cast<OnlineSendComponent*>(&e.getComponent("Send"));
			if (net != NULL)
			{
				net->addCommand("PUNCH");
				net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
			}
			auto a = static_cast<AnimationComponent*>(&e.getComponent("Animation"));
			a->playAnimation("Punch " + std::to_string(rand() % 2), true);
			auto s = static_cast<SpriteComponent*>(&e.getComponent("Sprite"));
			s->setTexture(a->getCurrentAnimation()->getTexture());
			auto p = static_cast<AudioComponent*>(&e.getComponent("Audio"));
			p->playSound("Whoosh", false);

			auto tag = "Attack";
			auto offset = Vector2f(phys->isMovingLeft() ? -40 : 40, phys->isGravityFlipped() ? 12.5f : -12.5f);

			hit->attack(offset, Vector2f(30, 25), e, tag, .175f, "Punch", 0);
			hit->setAttackProperties(3, phys->isMovingLeft() ? -100 : 100, phys->isGravityFlipped() ? -30 : 30);
		}

	}
};

class KickCommand : public Command
{
public:
	KickCommand() {}
	void execute(Entity& e)
	{
		//get the attack component from the entity
		auto hit = static_cast<AttackComponent*>(&e.getComponent("Attack"));
		auto phys = static_cast<PlayerPhysicsComponent*>(&e.getComponent("Player Physics"));

		if (hit->attackActive() == false && phys->stunned() == false)
		{
			auto net = static_cast<OnlineSendComponent*>(&e.getComponent("Send"));
			if (net != NULL)
			{
				net->addCommand("KICK");
				net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
			}
			auto tag = "Attack";
			auto offset = Vector2f(phys->isMovingLeft() ? -50 : 50, phys->isGravityFlipped() ? -12.5f : 12.5f);

			hit->attack(offset, Vector2f(50, 25), e, tag, .4f, "Kick", 0);
			hit->setAttackProperties(7, phys->isMovingLeft() ? -175 : 175, phys->isGravityFlipped() ? -45 : 45);

			auto a = static_cast<AnimationComponent*>(&e.getComponent("Animation"));
			a->playAnimation("Ground Kick", false);
			static_cast<SpriteComponent*>(&e.getComponent("Sprite"))->setTexture(a->getCurrentAnimation()->getTexture());
			auto p = static_cast<AudioComponent*>(&e.getComponent("Audio"));
			p->playSound("Whoosh", false);
		}

	}
};

class UppercutCommand : public Command
{
public:
	UppercutCommand() {}
	void execute(Entity& e)
	{
		//get the attack component from the entity
		auto hit = static_cast<AttackComponent*>(&e.getComponent("Attack"));
		auto phys = static_cast<PlayerPhysicsComponent*>(&e.getComponent("Player Physics"));

		if (hit->attackActive() == false && phys->stunned() == false)
		{
			auto net = static_cast<OnlineSendComponent*>(&e.getComponent("Send"));
			if (net != NULL)
			{
				net->addCommand("UPPERCUT");
				net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
			}

			auto a = static_cast<AnimationComponent*>(&e.getComponent("Animation"));
			a->playAnimation("Uppercut", false);
			static_cast<SpriteComponent*>(&e.getComponent("Sprite"))->setTexture(a->getCurrentAnimation()->getTexture());

			auto tag = "Attack";
			auto offset = Vector2f(phys->isMovingLeft() ? -37.5f : 37.5f, 0);

			hit->attack(offset, Vector2f(25, 45), e, tag, .4f, "Uppercut", 0);
			hit->setAttackProperties(4, phys->isMovingLeft() ? -10 : 10, phys->isGravityFlipped() ? -125 : 125);
		}

	}
};

class PhaseDownCommand : public Command
{
public:
	PhaseDownCommand() {}
	void execute(Entity& e)
	{
		//get the attack component from the entity
		auto hit = static_cast<AttackComponent*>(&e.getComponent("Attack"));
		auto phys = static_cast<PlayerPhysicsComponent*>(&e.getComponent("Player Physics"));

		if (hit->attackActive() == false && phys->stunned() == false)
		{
			if (phys->canFall())
			{
				auto net = static_cast<OnlineSendComponent*>(&e.getComponent("Send"));
				if (net != NULL)
				{
					net->addCommand("FALL");
					net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
				}
				phys->jumpDown();

				if(e.m_ID != "AI")
					achi::Listener::notify(&e, PHASE_DOWN);
			}
		}
	}
};

class SuperCommand : public Command
{
public:
	SuperCommand() {}
	void execute(Entity& e)
	{
		//get the attack component from the entity
		auto hit = static_cast<AttackComponent*>(&e.getComponent("Attack"));
		auto phys = static_cast<PlayerPhysicsComponent*>(&e.getComponent("Player Physics"));

		//Only execute if we arent in the middle of attacking and we are not stunned
		if (hit->attackActive() == false && phys->stunned() == false && phys->canSuperUp() && phys->isSupered() == false)
		{
			auto net = static_cast<OnlineSendComponent*>(&e.getComponent("Send"));
			if (net != NULL)
			{
				net->addCommand("SUPER");
				net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
			}

			phys->beginSuper();
			achi::Listener::notify(&e, SUPER_ACTIVATED);

		}
	}
};

class IdleCommand : public Command
{
public:
	IdleCommand() {}
	void execute(Entity& e)
	{
		static_cast<AnimationComponent*>(&e.getComponent("Animation"))->playAnimation("Idle", true);
		auto phys = static_cast<PlayerPhysicsComponent*>(&e.getComponent("Player Physics"));

		auto net = static_cast<OnlineSendComponent*>(&e.getComponent("Send"));
		if (net != NULL)
		{
			net->addCommand("IDLE");
			net->setSync(phys->posPtr->position, Vector2f(phys->m_currentVel.x, phys->m_currentVel.y), Vector2f(phys->m_desiredVel.x, phys->m_desiredVel.y));
		}
	}
};

#endif
