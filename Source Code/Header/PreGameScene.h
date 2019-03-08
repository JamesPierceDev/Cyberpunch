#pragma once
#include "Scene.h"
#include "OnlineSystem.h"
#include "AiInputComponent.h"
#include "PlayerInputComponent.h"
#include "Camera.h"

using std::map;
using std::pair;

class PreGameScene : public Scene {
public:
	static struct PlayersInfo {
		vector<pair<int,int>> localPlyrs;
		vector<int> onlinePlyrs;
		vector<int> botPlyrs;
	} playerIndexes;

	PreGameScene();
	void start();
	void stop();
	void update(double dt);
	void draw(SDL_Renderer& renderer);
	void handleInput(InputSystem& input);

	Entity* createBadge(int x, int y, bool isPlayer, int index);
	void reconstructBadges();

	//static PlayersInfo playerIndexes;

private:
	void checkForUpdates();
	vector<bool> m_availablePlyrs;
	vector<pair<InputComponent*, int>> m_input;
	//vector<bool> m_hasJoined;
	int m_numOfPossibleLocalPlayers;
	bool isGameOnline = false;
	OnlineSystem* m_network;

	bool m_addedInput;

	double lastUpdate = 0;

	int m_maxPlayers = 4;

	bool m_netGame = false;

	std::vector<Entity*> m_playerIcons;

	Entity m_bg;
	Camera m_camera;
	bool m_setupBg;
};