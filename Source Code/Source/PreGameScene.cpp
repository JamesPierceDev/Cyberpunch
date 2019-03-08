#include "..\Header\PreGameScene.h"
#include "RenderSystem.h"


PreGameScene::PlayersInfo PreGameScene::playerIndexes;
PreGameScene::PreGameScene() :
	m_bg("BG"),
	m_camera(false),
	m_setupBg(false)
{
	m_availablePlyrs.push_back(true);
	m_availablePlyrs.push_back(true);
	m_availablePlyrs.push_back(true);
	m_availablePlyrs.push_back(true);
}

void PreGameScene::start()
{
	for (auto b : m_availablePlyrs)
		b = true;
	playerIndexes.localPlyrs.clear();
	playerIndexes.onlinePlyrs.clear();
	playerIndexes.botPlyrs.clear();

	m_input.clear();

	m_numOfPossibleLocalPlayers = SDL_NumJoysticks();

	if (!m_setupBg)
	{
		auto bgPos = new PositionComponent(960, 540);
		m_bg.addComponent("Pos", bgPos);
		m_bg.addComponent("Sprite", new SpriteComponent(bgPos, Vector2f(1920, 1080), Vector2f(1920, 1080), Scene::resources().getTexture("Pre Game BG"), 0));
	}

	Scene::systems()["Render"]->addComponent(&m_bg.getComponent("Sprite"));

	m_network = static_cast<OnlineSystem*>(Scene::systems()["Network"]);

	m_input.push_back(pair<InputComponent*,int>(static_cast<InputComponent*>(Scene::systems()["Input"]->m_components.at(0)), -1));
	//m_input[0].first.initialiseJoycon(0);
	//m_hasJoined.push_back(true);

	if (m_network->isConnected)
	{
		m_netGame = true;
		if (m_network->m_isHost)
		{
			//m_input[0] gets to be player 1
			playerIndexes.localPlyrs.push_back(pair<int,int>(0,0));
			m_availablePlyrs[0] = false;
			m_input[0].second = 0;
		}
		else {
			//m_input[0] becomes the first available player
			playerIndexes.localPlyrs.push_back(pair<int, int>(0,m_network->m_playerNumber));//move from member var to getter
			m_availablePlyrs[m_network->m_playerNumber] = false;
			m_input[0].second = m_network->m_playerNumber;
		}
		vector<int> players = m_network->getPlayers();

		for (auto num : players)
		{
			if (num != playerIndexes.localPlyrs.back().second)
			{
				playerIndexes.onlinePlyrs.push_back(num);
				m_availablePlyrs[num] = false;

				//playerIndexes.botPlyrs.push_back(num);
			}
		}

	}
	else {
		playerIndexes.localPlyrs.push_back(pair<int, int>(0,0));
		m_availablePlyrs[0] = false;
		m_input[0].second = 0;
	}

	for (int i = 1; i < m_numOfPossibleLocalPlayers; i++)
	{
		m_input.push_back(pair<InputComponent*,int>(new InputComponent(), -1));
		m_input[i].first->initialiseJoycon(i);
		Scene::systems()["Input"]->addComponent(m_input[i].first);
		//m_hasJoined.push_back(false);
	}

	//Create first badge for the host player
	m_playerIcons.push_back(createBadge(240, 540, true, 0));

	m_setupBg = true;
}

void PreGameScene::stop()
{
	Scene::systems()["Render"]->deleteComponent(&m_bg.getComponent("Sprite"));

	for (auto& badge : m_playerIcons)
	{
		Scene::systems()["Render"]->deleteComponent(&badge->getComponent("Sprite"));
		Scene::systems()["Render"]->deleteComponent(&badge->getComponent("Ind Sprite"));

		delete &badge->getComponent("Pos");
		delete &badge->getComponent("Ind Pos");
		delete &badge->getComponent("Sprite");
		delete &badge->getComponent("Ind Sprite");
		delete badge;
	}

	//CLear ethe vector so we dont have redundant memory stored
	m_playerIcons.clear();
}

void PreGameScene::update(double dt)
{
	lastUpdate += dt;
	if (m_netGame && !m_network->isConnected)
	{
		Scene::goToScene("Main Menu");
	}
	if (m_network->isConnected && lastUpdate > 0.2)
	{
		checkForUpdates();
		lastUpdate = 0;
	}
	if (m_network->gameStarted)
	{
		Scene::goToScene("Game");
	}
}

void PreGameScene::draw(SDL_Renderer & renderer)
{
	static_cast<RenderSystem*>(Scene::systems()["Render"])->render(renderer, m_camera);
}

void PreGameScene::handleInput(InputSystem & input)
{
	//Update the input system
	Scene::systems()["Input"]->update(0);
	bool playersChanged = false;

	//If the main menu input component is not added to the system, add it
	if (!m_addedInput)
	{
		//input.addComponent(&m_input[0].first);
		m_addedInput = true;
	}
	else
	{
		if (m_input[0].first->isButtonPressed("XBTN"))
		{
			if (!m_network->isConnected)
			{
				Scene::goToScene("Game");
			}
			else if (m_network->m_isHost)
			{
				m_network->startGame();
				Scene::goToScene("Game");
			}
		}
		else if (m_input[0].first->isButtonPressed("ABTN"))
		{
			//tell network you are leaving. It's only polite
			//you'll have to tell it to shut down the lobby too probs....
			if (m_network->isConnected) {
				//vector<int> p = playerIndexes.localPlyrs;
				//p.insert(p.end(), playerIndexes.onlinePlyrs.begin(), playerIndexes.onlinePlyrs.end());
				vector<int> p{ 0,1,2,3 };
				m_network->disconnect(p);
				m_network->m_isHost = false;
			}
			playerIndexes.localPlyrs.clear();
			playerIndexes.onlinePlyrs.clear();
			playerIndexes.botPlyrs.clear();
			Scene::goToScene("Main Menu");
		}
		else if (m_input[0].first->isButtonPressed("BBTN"))
		{
			if (!m_network->isConnected || m_network->m_isHost)
			{
				//tell the network a slot is being taken up
				//add AI
				for (int j = 0; j < m_availablePlyrs.size(); j++)
				{
					if (m_availablePlyrs[j])
					{
						m_availablePlyrs[j] = false;
						playerIndexes.botPlyrs.push_back(j);
						m_playerIcons.push_back(createBadge(240 + m_playerIcons.size() * 480, 540, false, j));
						playersChanged = true;
						break;
					}
				}
			}
		}
		else if (m_input[0].first->isButtonPressed("YBTN"))
		{
			if (!m_network->isConnected || m_network->m_isHost)
			{
				//remove AI
				for (int j = m_availablePlyrs.size(); j > 0; j--)
				{
					if (std::find(playerIndexes.botPlyrs.begin(), playerIndexes.botPlyrs.end(), j) != playerIndexes.botPlyrs.end() && !m_availablePlyrs[j])
					{
						m_availablePlyrs[j] = true;
						playerIndexes.botPlyrs.push_back(j);
						playerIndexes.botPlyrs.erase(std::remove(playerIndexes.botPlyrs.begin(), playerIndexes.botPlyrs.end(), j), playerIndexes.botPlyrs.end());
						playersChanged = true;
						break;
					}
				}
			}

		}
		for (int i = 1; i < m_numOfPossibleLocalPlayers; i++)
		{
			if (m_input[i].first->isButtonPressed("XBTN"))//try to join the game;
			{
				if (m_input[i].second < 0)//controllers not joined are set to -1
				{
					if (m_network->isConnected)
						checkForUpdates();
					//join the game
					for (int j = 0; j < m_availablePlyrs.size(); j++)
					{
						if (m_availablePlyrs[j])
						{
							m_availablePlyrs[j] = false;
							playerIndexes.localPlyrs.push_back(pair<int, int>(i,j));
							m_input[i].second = j;
							//tell the network you've joined
							playersChanged = true;
							break;
						}
					}
				}
			}
			else if (m_input[i].first->isButtonPressed("ABTN"))
			{
				//remove this player from the game if they are in
				if (m_input[i].second > 0)//controllers not joined are set to -1
				{
					m_availablePlyrs[m_input[i].second] = true;
					playerIndexes.localPlyrs.erase(std::remove(playerIndexes.localPlyrs.begin(), playerIndexes.localPlyrs.end(), pair<int,int>(i,m_input[i].second)), playerIndexes.localPlyrs.end());
					m_input[i].second = -1;
					//tell the network you've left
					playersChanged = true;
				}
			}
		}
	}

	if (m_network->isConnected && playersChanged)//actually send the message to say which slots are taken
	{
		m_network->assignPlayerSlots(m_availablePlyrs);
	}

	if(playersChanged)
		reconstructBadges();
}

Entity * PreGameScene::createBadge(int x, int y, bool isPlayer, int index)
{
	auto ent = new Entity("Badge");
	auto pos = new PositionComponent(x, y);
	auto indPos = new PositionComponent(x, y - 200);
	ent->addComponent("Pos", pos);
	ent->addComponent("Ind Pos", indPos);
	ent->addComponent("Sprite", new SpriteComponent(pos, Vector2f(300, 300), Vector2f(300, 300), Scene::resources().getTexture("Pre Game Head" + std::to_string(index)), 1));
	ent->addComponent("Ind Sprite", new SpriteComponent(indPos, Vector2f(200, 75), Vector2f(200, 75), Scene::resources().getTexture(isPlayer ? "Player Indicator" : "Cpu Indicator"), 1));

	Scene::systems()["Render"]->addComponent(&ent->getComponent("Sprite"));
	Scene::systems()["Render"]->addComponent(&ent->getComponent("Ind Sprite"));

	return ent;
}

void PreGameScene::reconstructBadges()
{
	for (auto& badge : m_playerIcons)
	{
		Scene::systems()["Render"]->deleteComponent(&badge->getComponent("Sprite"));
		Scene::systems()["Render"]->deleteComponent(&badge->getComponent("Ind Sprite"));

		delete &badge->getComponent("Pos");
		delete &badge->getComponent("Ind Pos");
		delete &badge->getComponent("Sprite");
		delete &badge->getComponent("Ind Sprite");
		delete badge;
	}

	//CLear ethe vector so we dont have redundant memory stored
	m_playerIcons.clear();

	for (auto index : playerIndexes.localPlyrs)
	{
		m_playerIcons.push_back(createBadge(240 + 480 * index.second, 540, true, index.second));
	}
	for (auto index : playerIndexes.onlinePlyrs)
	{
		m_playerIcons.push_back(createBadge(240 + 480 * index, 540, true, index));
	}
	for (auto index : playerIndexes.botPlyrs)
	{
		m_playerIcons.push_back(createBadge(240 + 480 * index, 540, false, index));
	}
}

void PreGameScene::checkForUpdates()
{
	vector<int> players = m_network->getPlayers();
	playerIndexes.onlinePlyrs.clear();
	//playerIndexes.botPlyrs.clear();
	for (auto num : players)
	{
		vector<int> localPlyrs;
		for (auto l : playerIndexes.localPlyrs)
			localPlyrs.push_back(l.second);
		//bool notInLocal = !(std::find(playerIndexes.localPlyrs.begin(), playerIndexes.localPlyrs.end(), num) != playerIndexes.localPlyrs.end());
		bool notInLocal = !(std::find(localPlyrs.begin(), localPlyrs.end(), num) != localPlyrs.end());
		bool notInBots = !(std::find(playerIndexes.botPlyrs.begin(), playerIndexes.botPlyrs.end(), num) != playerIndexes.botPlyrs.end());
		if( notInLocal && notInBots)
		{
			playerIndexes.onlinePlyrs.push_back(num);
			m_availablePlyrs[num] = false;
		}
	}
	reconstructBadges();
}
