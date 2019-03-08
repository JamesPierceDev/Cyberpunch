#pragma once
#include "Observer.h"
#include "SDL.h"
#include <iostream>
#include "InputSystem.h"
#include "ResourceManager.h"
#include "AudioComponent.h"
#include "AchievementComponent.h"

//Base scene class, this will be inherited by every other scene
class Scene : public Subject
{
public:
	Scene() { m_stgt = ""; m_cs = false; }
	virtual ~Scene() {}
	virtual void start() = 0;
	virtual void stop() = 0;
	virtual void update(double dt) = 0;
	virtual void handleInput(InputSystem& input) = 0;
	virtual void draw(SDL_Renderer& renderer) = 0;
	void goToScene(std::string sceneName)
	{
		m_stgt = sceneName;
		m_cs = true;
	}
	bool changeScene() { return m_cs; }
	std::string getNewScene() { return m_stgt; }
	void resetSceneChange() { m_cs = false; }
	void setResourceHandler(ResourceHandler& resources) { m_resources = &resources; }
	void setSystemsPtr(std::map<std::string, System*>& map) { m_systems = &map; }
	ResourceHandler& resources() { return *m_resources; }
	std::map<std::string, System*>& systems() { return *m_systems; };
	std::string getStgt() { return m_stgt; }
	AudioComponent& audio() { return m_audio; }
	AchievementComponent& achievements() { return m_achievements; }
private:
	AudioComponent m_audio;
	AchievementComponent m_achievements;
	ResourceHandler * m_resources;
	std::map<std::string, System*> * m_systems;
	std::string m_stgt;
	bool m_cs;
};