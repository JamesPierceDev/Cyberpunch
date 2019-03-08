#include "CollisionListener.h"
#include "Entity.h"
#include "PlayerPhysicsComponent.h"
#include "AttackComponent.h"
#include "PickUpComponent.h"
#include "DustTriggerComponent.h"
#include "DJboothComponent.h"
#include "PlayerComponent.h"
#include "AnimationComponent.h"
#include "AIComponent.h"


void CollisionListener::BeginContact(b2Contact * contact)
{
	//Convert both contact points to a ColData struct, from here we can check what tthe bodies are tagged with
	auto dataA = static_cast<PhysicsComponent::ColData *>(contact->GetFixtureA()->GetUserData());
	auto dataB = static_cast<PhysicsComponent::ColData *>(contact->GetFixtureB()->GetUserData());

	//If the players jump sensor has hit a platform, set the player to be able to jump
	if ((dataA->Tag() == "Jump Sensor" && dataB->Tag() == "Platform")
	|| (dataB->Tag() == "Jump Sensor" && dataA->Tag() == "Platform")
	|| (dataA->Tag() == "Jump Sensor" && dataB->Tag() == "Floor")
	|| (dataB->Tag() == "Jump Sensor" && dataA->Tag() == "Floor"))
	{
		//Convert the collision data to a player pointer and call the allow jump method
		auto player = static_cast<Entity*>(dataA->Tag() == "Jump Sensor" ? dataA->Data() : dataB->Data());
		auto playerPhys = static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"));

		//If the player has hit a platform, set his can fall boolean
		if (dataA->Tag() == "Platform" || dataB->Tag() == "Platform")
		{
			b2Fixture* sensor = dataA->Tag() == "Jump Sensor" ? contact->GetFixtureA() : contact->GetFixtureB();
			b2Fixture* platform = dataA->Tag() == "Platform" ? contact->GetFixtureA() : contact->GetFixtureB();

			auto platHeight = platform->GetBody()->GetFixtureList()->GetAABB(0).GetExtents().y / 2.0f; //get height of the sensor
			auto sensPos = sensor->GetBody()->GetPosition(); //Get position of the sensor
			auto platPos = platform->GetBody()->GetPosition(); //Get position of the platform

			if ((!m_gravFlipped && sensPos.y <= platPos.y + platHeight)
			|| (m_gravFlipped && sensPos.y >= platPos.y - platHeight))
			{
				playerPhys->setCanFall(true);
				playerPhys->setCanJump(true);
				//Create a dust particle for landing on the ground
				static_cast<DustTriggerComponent*>(&player->getComponent("Dust Trigger"))->setCreate();
			}
		}
		else if ((dataA->Tag() == "Jump Sensor" && dataB->Tag() == "Floor")
			|| (dataB->Tag() == "Jump Sensor" && dataA->Tag() == "Floor"))
		{
			playerPhys->setCanJump(true);
			//Create a dust particle for landing on the ground
			static_cast<DustTriggerComponent*>(&player->getComponent("Dust Trigger"))->setCreate();
		}
	}

	if ((dataA->Tag() == "Jump Sensor" && dataB->Tag() == "Player Body")
		|| (dataB->Tag() == "Jump Sensor" && dataA->Tag() == "Player Body"))
	{
		auto player = static_cast<Entity*>(dataA->Tag() == "Jump Sensor" ? dataA->Data() : dataB->Data());
		auto playerPhys = static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"));

		playerPhys->setCanJump(true);
		playerPhys->setOnPlayer(true);
	}
	
	/// <summary>
	/// Collision for the Player and the pickup, after collision triggers responses 
	/// </summary>
	/// <param name="contact"></param>
	if ((dataA->Tag() == "Player Body" && dataB->Tag() == "Pickup")
		|| (dataB->Tag() == "Player Body" && dataA->Tag() == "Pickup"))
	{
		auto player = static_cast<Entity*>(dataA->Tag() == "Player Body" ? dataA->Data() : dataB->Data());
		auto pickUp = static_cast<Entity*>(dataB->Tag() == "Pickup" ? dataB->Data() : dataA->Data());

		static_cast<PickUpComponent*>(&pickUp->getComponent("PickUp"))->teleport(player);
		static_cast<AudioComponent&>(pickUp->getComponent("Audio")).playSound("PickUp 1", false);
		auto p = static_cast<PlayerComponent*>(&player->getComponent("Player"));
		//If player hits pickup, set status as dj
		p->setDJ(true);
	}
	/// <summary>
	/// Collision for when the player hits the booth, after collision triggers responses
	/// </summary>
	/// <param name="contact"></param>
	if ((dataA->Tag() == "Attack" && dataB->Tag() == "Booth")
		|| (dataB->Tag() == "Attack" && dataA->Tag() == "Booth"))
	{
		auto booth = static_cast<Entity*>(dataA->Tag() == "Booth" ? dataA->Data() : dataB->Data());
		auto djB = static_cast<DJBoothComponent*>(&booth->getComponent("DJ Booth"));
		djB->run();
		static_cast<PickUpComponent&>(djB->m_pickUp->getComponent("PickUp")).m_end = true;
		static_cast<AudioComponent&>(booth->getComponent("Audio")).playSound("Switch2", false);
	}

	if ((dataA->Tag() == "Kill Box" && dataB->Tag() == "Player Body")
		|| (dataB->Tag() == "Kill Box" && dataA->Tag() == "Player Body"))
	{
		auto player = static_cast<Entity*>(dataA->Tag() == "Player Body" ? dataA->Data() : dataB->Data());

		static_cast<PlayerComponent*>(&player->getComponent("Player"))->respawn();
	}
	
	//Check if a player has attacked another player
	checkPlayerAttack(contact);
}

void CollisionListener::EndContact(b2Contact * contact)
{
	//Convert both contact points to a ColData struct, from here we can check what tthe bodies are tagged with
	auto dataA = static_cast<PhysicsComponent::ColData *>(contact->GetFixtureA()->GetUserData());
	auto dataB = static_cast<PhysicsComponent::ColData *>(contact->GetFixtureB()->GetUserData());

	//If the players jump sensor has stopped hitting a platform, set the player to be able to jump
	if ((dataA->Tag() == "Jump Sensor" && dataB->Tag() == "Platform")
		|| (dataB->Tag() == "Jump Sensor" && dataA->Tag() == "Platform")
		|| (dataA->Tag() == "Jump Sensor" && dataB->Tag() == "Floor")
		|| (dataB->Tag() == "Jump Sensor" && dataA->Tag() == "Floor"))
	{
		////Convert the collision data to a player pointer and call the allow jump method
		auto player = static_cast<Entity*>(dataA->Tag() == "Jump Sensor" ? dataA->Data() : dataB->Data());
		auto phys = static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"));
		phys->setCanJump(false);
		phys->setCanFall(false);
	}

	if ((dataA->Tag() == "Jump Sensor" && dataB->Tag() == "Player Body")
		|| (dataB->Tag() == "Jump Sensor" && dataA->Tag() == "Player Body"))
	{
		auto player = static_cast<Entity*>(dataA->Tag() == "Jump Sensor" ? dataA->Data() : dataB->Data());
		auto playerPhys = static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"));

		playerPhys->setCanJump(false);
		playerPhys->setOnPlayer(false);
	}

	if ((dataA->Tag() == "Right Edge Sensor" && dataB->Tag() == "Platform")
		|| (dataB->Tag() == "Right Edge Sensor" && dataA->Tag() == "Platform")
		|| (dataA->Tag() == "Right Edge Sensor" && dataB->Tag() == "Floor")
		|| (dataB->Tag() == "Right Edge Sensor" && dataA->Tag() == "Floor"))
	{
		auto ai = static_cast<Entity*>(dataA->Tag() == "Right Edge Sensor" ? dataA->Data() : dataB->Data());
		auto comp = static_cast<AIComponent *>(&ai->getComponent("AI"));
		auto phys = static_cast<PlayerPhysicsComponent *>(&ai->getComponent("Player Physics"));

		//If the right edge sensor is not touching the platform, but the AI is on a platform
		if (phys->canJump())
		{
			//Set bool true in AI component
			comp->onEdgeRight = true;
		}
	}

	if ((dataA->Tag() == "Left Edge Sensor" && dataB->Tag() == "Platform")
		|| (dataB->Tag() == "Left Edge Sensor" && dataA->Tag() == "Platform")
		|| (dataA->Tag() == "Left Edge Sensor" && dataB->Tag() == "Floor")
		|| (dataB->Tag() == "Left Edge Sensor" && dataA->Tag() == "Floor"))
	{
		auto ai = static_cast<Entity*>(dataA->Tag() == "Left Edge Sensor" ? dataA->Data() : dataB->Data());
		auto comp = static_cast<AIComponent *>(&ai->getComponent("AI"));
		auto phys = static_cast<PlayerPhysicsComponent *>(&ai->getComponent("Player Physics"));

		//If the left edge sensor is not touching the platform, but the AI is on a platform
		if (phys->canJump())
		{
			//Set bool true in AI component
			comp->onEdgeLeft = true;
		}
	}

	//if a players body has hit a platform
	if ((dataA->Tag() == "Player Body" && dataB->Tag() == "Platform")
		|| (dataB->Tag() == "Player Body" && dataA->Tag() == "Platform"))
	{
		//Convert the collision data to a player pointer and call the allow jump method
		auto player = dataA->Tag() == "Player Body" ? contact->GetFixtureA() : contact->GetFixtureB();
		auto platform = dataA->Tag() == "Platform" ? contact->GetFixtureA() : contact->GetFixtureB();

		auto pPtr = static_cast<Entity*>(dataA->Tag() == "Player Body" ? dataA->Data() : dataB->Data());

		auto pHeight = player->GetBody()->GetFixtureList()->GetAABB(0).GetExtents().y; //get height of the player

		//Convert the collision data to a player pointer and call the allow jump method
		auto phys = static_cast<PlayerPhysicsComponent*>(&pPtr->getComponent("Player Physics"));

		//If the player is jumping up from below a platform, set it sensor to true
		if ((!m_gravFlipped && player->GetBody()->GetPosition().y + pHeight < platform->GetBody()->GetPosition().y)
			|| (m_gravFlipped && player->GetBody()->GetPosition().y - pHeight > platform->GetBody()->GetPosition().y))
		{
			//Set contact as disabled so the player can move through floors
			phys->falling() = false;
		}
	}
}

void CollisionListener::checkPlayerAttack(b2Contact * contact)
{
	//Cant handle attacks with other players yet, must get the player entities and convert them to Attacking/physics components etc..
	auto dataA = static_cast<PhysicsComponent::ColData *>(contact->GetFixtureA()->GetUserData());
	auto dataB = static_cast<PhysicsComponent::ColData *>(contact->GetFixtureB()->GetUserData());
	Entity* attackingP = nullptr;
	Entity* otherP = nullptr;
	PlayerPhysicsComponent* otherPPhys = nullptr;
	PlayerPhysicsComponent* attackingPPhys = nullptr;
	AttackComponent* attackHit = nullptr;
	float xImpulse;
	float yImpulse;
	int dmgP;
	bool applyDmg = false;

	//If a player has attacked and hit a player
	if ((dataA->Tag() == "Attack" && dataB->Tag() == "Player Body")
	|| (dataB->Tag() == "Attack" && dataA->Tag() == "Player Body"))
	{
		//get our player pointers
		attackingP = static_cast<Entity*>(dataA->Tag() == "Attack" ? dataA->Data() : dataB->Data());
		otherP = static_cast<Entity*>(dataA->Tag() == "Player Body" ? dataA->Data() : dataB->Data());
		otherPPhys = static_cast<PlayerPhysicsComponent*>(&otherP->getComponent("Player Physics"));

		if (attackingP == otherP)
			return;

		attackingPPhys = static_cast<PlayerPhysicsComponent*>(&attackingP->getComponent("Player Physics"));
		attackHit = static_cast<AttackComponent*>(&attackingP->getComponent("Attack"));

		//If the attacking player punched
		if (attackHit->attacked() && !attackingPPhys->stunned())
		{
			applyDmg = true;
			dmgP = attackHit->damage(); //Get the damage
			xImpulse = attackHit->xImpulse();
			yImpulse = attackHit->yImpulse();
		}
	}

	//If the player was not found return
	if (nullptr == attackingP || nullptr == otherP)
		return;
	else if(applyDmg)
	{
		auto attackPlayerVariables = static_cast<PlayerComponent*>(&attackingP->getComponent("Player"));
		auto otherPlayerVariables = static_cast<PlayerComponent*>(&otherP->getComponent("Player"));

		otherPlayerVariables->m_hitBy = static_cast<Entity*>(dataA->Tag() == "Attack" ? dataA->Data() : dataB->Data());
		otherPlayerVariables->m_hitWith = attackHit->m_currentAttackTag;

		if (otherPlayerVariables->m_hitWith == "Punch" && attackingP->m_ID != "AI")
			notify(nullptr, PUNCH_FIRST);

		//If you attack a player that isnt super stunned, add to the super percentage
		if(otherPPhys->superStunned() == false)
			attackingPPhys->addSuper(dmgP); //Add damage to our super percentage
		if (attackingPPhys->isSupered() && otherPPhys->superStunned() == false)
		{
			attackPlayerVariables->m_supersUsed++;
			otherPlayerVariables->m_timesSuperStunned++;

			otherPPhys->superStun(); //Super stun the other player
			attackingPPhys->endSuper(); //End super for the player that hit with it
			notify(otherP, Event::SUPER_STUN); //Send super stun event to the observer
			static_cast<AnimationComponent&>(otherP->getComponent("Animation")).playAnimation("Super Stun", false);
		}

		otherPPhys->damage(dmgP); //Add damage of the punch to the other players damage percentage
		otherPPhys->applyDamageImpulse(xImpulse, yImpulse); //Knock back the other player back
		otherPPhys->stun();

		//Add to the player componenet variables
		attackPlayerVariables->m_dmgDealt += dmgP;
		otherPlayerVariables->m_dmgTaken += dmgP;

		//Notify our subject of the events that have happened
		notify(attackingP, Event::DAMAGE_DEALT);
		notify(otherP, Event::DAMAGE_TAKEN);

		if(xImpulse < 75 && yImpulse < 75)
			static_cast<AnimationComponent&>(otherP->getComponent("Animation")).playAnimation("Small Stun", false);
		else
			static_cast<AnimationComponent&>(otherP->getComponent("Animation")).playAnimation("Big Stun", false);

		attackHit->destroyAttack() = true;
		static_cast<AudioComponent&>(otherP->getComponent("Audio")).playSound("Punch", false);
	}

}

void CollisionListener::PreSolve(b2Contact * contact, const b2Manifold * oldManifold)
{
	//Convert both contact points to a ColData struct, from here we can check what tthe bodies are tagged with
	auto dataA = static_cast<PhysicsComponent::ColData *>(contact->GetFixtureA()->GetUserData());
	auto dataB = static_cast<PhysicsComponent::ColData *>(contact->GetFixtureB()->GetUserData());
	
	//if a players body has hit a platform
	if ((dataA->Tag() == "Player Body" && dataB->Tag() == "Platform")
		|| (dataB->Tag() == "Player Body" && dataA->Tag() == "Platform"))
	{
		//Convert the collision data to a player pointer and call the allow jump method
		auto player = dataA->Tag() == "Player Body" ? contact->GetFixtureA() : contact->GetFixtureB();
		auto platform = dataA->Tag() == "Platform" ? contact->GetFixtureA() : contact->GetFixtureB();

		auto pPtr = static_cast<Entity*>(dataA->Tag() == "Player Body" ? dataA->Data() : dataB->Data());

		auto pHeight = player->GetBody()->GetFixtureList()->GetAABB(0).GetExtents().y; //get height of the player

		//Convert the collision data to a player physics pointer and call the allow jump method
		auto phys = static_cast<PlayerPhysicsComponent*>(&pPtr->getComponent("Player Physics"));

		//If the player is jumping up from below a platform, set it sensor to true
		if ((!m_gravFlipped && player->GetBody()->GetPosition().y + (pHeight / 2) >= platform->GetBody()->GetPosition().y)
			|| (m_gravFlipped && player->GetBody()->GetPosition().y - (pHeight / 2) <= platform->GetBody()->GetPosition().y)
			|| (phys->falling()))
		{
			//Set contact as disabled so the player can move through floors
			contact->SetEnabled(false);
		}
	}
}

void CollisionListener::PostSolve(b2Contact * contact, const b2ContactImpulse * impulse)
{

}

void CollisionListener::flipGravity()
{
	m_gravFlipped = !m_gravFlipped;
}

