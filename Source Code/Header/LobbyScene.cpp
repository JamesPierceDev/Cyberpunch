#include "LobbyScene.h"
#include "RenderSystem.h"

LobbyScene::LobbyScene() :
	m_currentIndex(0),
	m_addedInput(false),
	m_camera(false), //Dont use camera,
	m_bg("Lobby BG"),
	m_selectedBar("Select Bar")
{
}

void LobbyScene::start()
{
	m_currentIndex = 0; //Start main menu at hovering over local play button

	m_network = static_cast<OnlineSystem*>(Scene::systems()["Network"]);
	// Initialise SDL_net (Note: We don't initialise or use normal SDL at all - only the SDL_net library!)
	if (SDLNet_Init() == -1)
	{
		std::cerr << "Failed to intialise SDL_net: " << SDLNet_GetError() << std::endl;
		exit(-1);
	}
	if (!m_network->isConnected)
	{
		if (m_network->ConnectToServer())
		{
			//if it can connect to the server, make the UI and fetch lobbies
			m_lobbies = m_network->getLobbies();
			createLobbyButtons();
		}
		else {
			//else just tell the player to return to main menu
			//Scene::goToScene("Main Menu");
		}
	}
	else if (m_network->isConnected)
	{
		//if it is already connected, get the lobbies and display them
		m_lobbies = m_network->getLobbies();
		createLobbyButtons();
	}
	else {
		m_lobbies = m_network->getLobbies();
	}


	auto bgPos = new PositionComponent(960, 540);
	m_bg.addComponent("Sprite", new SpriteComponent(bgPos, Vector2f(1920, 1080), Vector2f(1920, 1080), Scene::resources().getTexture("Lobby BG"), 0));
	Scene::systems()["Render"]->addComponent(&m_bg.getComponent("Sprite"));
}

void LobbyScene::stop()
{
	//Remove components from systems
	for (auto& btn : m_buttons)
	{
		Scene::systems()["Render"]->deleteComponent(&btn->getComponent("Sprite"));
		Scene::systems()["Render"]->deleteComponent(&btn->getComponent("Player Num"));
		Scene::systems()["Render"]->deleteComponent(&btn->getComponent("Lobby Num"));
		Scene::systems()["Render"]->deleteComponent(&btn->getComponent("Password Sprite"));
	}
	Scene::systems()["Render"]->deleteComponent(&m_bg.getComponent("Sprite"));
	Scene::systems()["Render"]->deleteComponent(&m_selectedBar.getComponent("Sprite"));

	//Clear the buttons vector
	m_buttons.clear();
}

void LobbyScene::update(double dt)
{
	if (!m_network->isConnected)
		Scene::goToScene("Main Menu");
}

void LobbyScene::draw(SDL_Renderer & renderer)
{
	static_cast<RenderSystem*>(Scene::systems()["Render"])->render(renderer, m_camera);
}

void LobbyScene::handleInput(InputSystem & input)
{
	//Update the input system
	Scene::systems()["Input"]->update(0);

	//If the main menu input component is not added to the system, add it
	if (!m_addedInput)
	{
		m_input = static_cast<InputComponent*>(Scene::systems()["Input"]->m_components.at(0));
		m_addedInput = true;
	}
	else
	{
		int newIndex = m_currentIndex;
		if (m_input->isButtonPressed("STICKUP"))
		{
			newIndex--;
		}
		if (m_input->isButtonPressed("STICKDOWN"))
		{
			newIndex++;
		}
		if (m_input->isButtonPressed("XBTN"))
		{
			handleButtonPressed();//Join the selected server. If it fails, show a message and refresh
		}
		if (m_input->isButtonPressed("BBTN"))
		{
			requestHost();//try to start a game with no players and wait for them to join
		}
		if (m_input->isButtonPressed("YBTN"))
		{
			m_lobbies = m_network->getLobbies();//Refresh the page from the server
			createLobbyButtons();
		}
		if (m_input->isButtonPressed("ABTN"))
		{
			m_network->disconnect(vector<int>{m_network->m_playerNumber});
			Scene::goToScene("Main Menu");//Just go back to the main menu
		}

		if (newIndex < 0)
			newIndex =  ((m_buttons.size() - 1) < 0) ? (m_buttons.size() - 1) : 0;
		else if (newIndex >= m_buttons.size())
			newIndex = 0;

		//If the button we are highlighting has changed, deslect the current button and select the new one
		if (newIndex != m_currentIndex)
		{
			Scene::systems()["Render"]->addComponent(&m_buttons.at(m_currentIndex)->getComponent("Sprite"));

			//Set new button index
			m_currentIndex = newIndex;

			//Move the selected bar to the correct location
			static_cast<PositionComponent*>(&m_selectedBar.getComponent("Pos"))->position = Vector2f(960, 280 + (120 * m_currentIndex));

			Scene::systems()["Render"]->deleteComponent(&m_buttons.at(m_currentIndex)->getComponent("Sprite"));
		}
	}
}

void LobbyScene::handleButtonPressed()
{
	//placeholder

	if (m_buttons.size() != 0 && m_network->joinLobby(m_currentIndex+1))//plz to always be nonzero
	{
		Scene::goToScene("PreGame");
	}
	else {
		//Give failure message, refresh
		m_lobbies = m_network->getLobbies();
		//createLobbyButtons();
	}

	/*auto tag = static_cast<ButtonComponent*>(&m_buttons.at(m_currentIndex)->getComponent("Btn"))->getTag();

	if (tag == "Local")
	{
		Scene::goToScene("Game");
	}
	else if (tag == "Multiplayer")
	{
		Scene::goToScene("Main Menu"); // Temp, dont have a pre game lobby screen
	}
	else if (tag == "Options")
	{
		Scene::goToScene("Options");
	}
	else if (tag == "Exit")
	{
		// Need a way to exit the game
	}*/
}

void LobbyScene::createLobbyButtons()
{
	for (auto but : m_buttons)
	{
		Scene::systems()["Render"]->deleteComponent(&but->getComponent("Sprite"));
		Scene::systems()["Render"]->deleteComponent(&but->getComponent("Player Num"));
		Scene::systems()["Render"]->deleteComponent(&but->getComponent("Lobby Num"));
		Scene::systems()["Render"]->deleteComponent(&but->getComponent("Password Sprite"));
		Scene::systems()["Render"]->deleteComponent(&m_selectedBar.getComponent("Sprite"));
		delete(but);
	}
	m_buttons.clear();
	m_currentIndex = 0; //Set index to be the first index of the buttons
	//Only display 6 lobbies
	for (int i = 1; i < m_lobbies.size() && i < 6; i++) {
		m_buttons.push_back(createButton(Vector2f(960, 280 + ((i -1) * 120)), i, m_lobbies.at(i).name, std::stoi(m_lobbies.at(i).players), false, false));

		if (i == 1)
		{
			auto pos = new PositionComponent(960, 280);
			m_selectedBar.addComponent("Pos", pos);
			m_selectedBar.addComponent("Sprite", new SpriteComponent(pos, Vector2f(760, 135), Vector2f(760, 135), Scene::resources().getTexture("Lobby Bar Selected"), 2));
			Scene::systems()["Render"]->addComponent(&m_selectedBar.getComponent("Sprite"));
		}
	}
}


Entity* LobbyScene::createButton(Vector2f position, int index, std::string btnTag, int noOfPlayers, bool passProtected, bool selected)
{
	auto btn = new Entity("Button");
	auto pos = new PositionComponent(position.x, position.y);
	auto btnComp = new ButtonComponent(Scene::resources().getTexture("Lobby Bar Selected"), Scene::resources().getTexture("Lobby Bar"), btnTag, selected);
	btn->addComponent("Btn", btnComp);
	btn->addComponent("Pos", pos);
	btn->addComponent("Sprite", new SpriteComponent(pos, Vector2f(760, 135), Vector2f(760, 135), btnComp->getTexture(), 1));
	auto playerNum = new SpriteComponent(new PositionComponent(position.x - 10, position.y + 3), Vector2f(320, 32), Vector2f(32, 32), Scene::resources().getTexture("Numbers"), 1);
	playerNum->setSourceRect({(noOfPlayers * 32), 0, 32, 32 });
	btn->addComponent("Player Num", playerNum);
	auto lobbyNum = new SpriteComponent(new PositionComponent(position.x - 257, position.y + 3), Vector2f(320, 32), Vector2f(32, 32), Scene::resources().getTexture("Numbers"), 1);
	lobbyNum->setSourceRect({ index * 32, 0, 32, 32 });
	btn->addComponent("Lobby Num", lobbyNum);
	btn->addComponent("Password Sprite", new SpriteComponent(new PositionComponent(position.x + 259, position.y + 3),
		Vector2f(75, 50), Vector2f(75, 50), Scene::resources().getTexture(passProtected ? "Password Yes" : "Password No"), 1));

	//Add sprite component to the render system
	if(index != 1)
		Scene::systems()["Render"]->addComponent(&btn->getComponent("Sprite"));
	Scene::systems()["Render"]->addComponent(&btn->getComponent("Player Num"));
	Scene::systems()["Render"]->addComponent(&btn->getComponent("Lobby Num"));
	Scene::systems()["Render"]->addComponent(&btn->getComponent("Password Sprite"));

	//Return the created btn
	return btn;
}

void LobbyScene::requestHost()
{
	m_network->makeHost();
	cout << "hosting" << endl;
	Scene::goToScene("PreGame");
	//go to pregame lobby to wait for more players to join
}
