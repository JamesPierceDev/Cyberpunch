#pragma once
#include "Component.h"
#include "PlayerComponent.h"
#include "Commands.h"
#include "nlohmann/json.hpp"
#include <queue>

using std::string;
using nlohmann::json;
using std::queue;
using std::vector;

class OnlineInputComponent : public Component {
public:
	OnlineInputComponent();

	void handleInput(void* e);
	int addCommand(string);
	void addPositions(float px, float py, float vx, float vy, float dvx, float dvy);

	void syncPosition(Entity* entity, float px, float py, float vx, float vy, float dvx, float dvy);

	int m_playerNumber;
private:

	Command * m_currentCMD, *m_previousCMD;

	JumpCommand m_jumpCMD;
	MoveLeftCommand m_moveLeftCMD;
	MoveRightCommand m_moveRightCMD;
	PunchCommand m_punchCMD;
	KickCommand m_kickCMD;
	UppercutCommand m_uppercutCMD;
	IdleCommand m_idleCMD;
	PhaseDownCommand m_fallCMD;
	SuperCommand m_superCMD;

	queue<string> m_commandsToSend;
	queue<OnlineSendComponent::syncStruct> m_positionsToSyncTo;

};