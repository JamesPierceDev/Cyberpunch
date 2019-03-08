#include "Observer.h"
#include "AchievementComponent.h"
#include "PlayerComponent.h"

void AchievementsListener::onNotify(Entity * ent, Event event)
{
	//Switch case on the event
	switch (event)
	{
	case DAMAGE_TAKEN:
		//If the player has taken 1000 damage, unlock the achievement for it
		if (static_cast<PlayerComponent*>(&ent->getComponent("Player"))->m_dmgTaken >= 1000)
		{
			static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Punching Bag");
		}
		break;
	case PUNCH_FIRST:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Punch First");
		break;
	case PLAYER_DEATH:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("First Blood");
		break;
	case DAMAGE_DEALT:
		//If the player has dealt 1000 damage, unlock the achievement for it
		if (static_cast<PlayerComponent*>(&ent->getComponent("Player"))->m_dmgDealt >= 1000)
		{
			static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Damage Dealer");
		}
		break;
	case SUPER_STUN:
		//If the player has used 5 super stuns, unlock the achievement for it
		if (static_cast<PlayerComponent*>(&ent->getComponent("Player"))->m_supersUsed >= 5)
		{
			static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Stunner");
		}
		break;
	case SUPER_ACTIVATED:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Final Form");
		break;
	case AI_DEATH:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Eh Aye");
		break;
	case PHASE_DOWN:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Going Down");
		break;
	case SLOW_DOWN:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Back to The Future");
		break;
	case GRAV_FLIP:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("The Upside Down");
		break;
	case PLATFORM_MOVE:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Mover");
		break;
	case KICK_DEATH:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Bend it like Beckham");
		break;
	case ONLINE_MATCH_ENDED:
		static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Friends!");
		break;
	case MATCH_ENDED:
		auto playerComp = dynamic_cast<PlayerComponent*>(&ent->getComponent("Player"));
		//Unlock the achievement
		if (nullptr != playerComp)
		{
			if (playerComp->isWinner() && ent->m_ID != "AI")
			{
				static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Last Woman Standing");
			}
		}
		//If a game of 4 local players was played, unlock party time achievment
		if (achi::Listener::m_localPlayers == 4)
		{
			static_cast<AchievementComponent*>(achi::Listener::m_AchisPtr)->unlockAchievement("Party Time");
		}
		break;
	}
}