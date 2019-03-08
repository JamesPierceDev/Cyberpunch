#pragma once
#include "Component.h"
#include <iostream>
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

class AchievementComponent : public Component
{
public:
	AchievementComponent()
	{ 
	}

	void setAchievementData(json* data)
	{
		m_data = data; //Set pointer to original data

		//Loads in all the achievement data
		for (auto& pair : (*m_data)["Achievements"])
		{
			m_achievements[pair["Name"]] = pair["Value"] == "True" ? true : false;
		}
	}

	void unlockAchievement(std::string name)
	{
		bool hasAchiev = false;
		for (auto& pair : m_achievements)
		{
			if (pair.first == name)
			{
				hasAchiev = true;
				//If the achievement is not unlocked, unlock it
				if (pair.second == false)
				{
					achi::Listener::m_newUnlocks.push_back(name);
					std::cout << "Unlocked achievement: " << name << "\n";
					pair.second = true;
				}
				break;
			}
		}

		if (!hasAchiev)
			std::cout << "Tried unlocking an achievement that doesnt exist\n";
		else
		{
			//Update achievement data
			m_achievements[name] = true; //Set it to true

			for (auto& pair : (*m_data)["Achievements"])
			{
				if (pair["Name"] == name)
				{
					pair["Value"] = "True";
					break;
				}
			}

			std::ofstream file("Resources/Achievements.txt");
			
			//Save the new values to the achievements text file to update the newly unlocked achievements
			file << (*m_data);
		}
	}

	std::map<std::string, bool> m_achievements;
private:
	json* m_data;
};