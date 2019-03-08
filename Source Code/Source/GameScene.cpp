#include "GameScene.h"
#include "RenderSystem.h"
#include "PlayerRespawnSystem.h"
#include "AnimationComponent.h"
#include "DustSystem.h"
#include "PhysicsSystem.h"
#include "PickUpSystem.h"
#include "PlayerPhysicsSystem.h"
#include "PlatformComponent.h"

GameScene::GameScene() :
	m_bgEntity("Game BG"),
	m_gameStart("Start Timer"),
	m_gameEndE("End winner"),
	m_platformsCreated(false),
	m_camera(false),
	m_gameStartTimer(3),
	m_achievListener(),
	m_achiPopup("Pop Up"),
	m_popupSet(false)
{
	m_numOfAIPlayers = 0;
}

void GameScene::start()
{
	addObserver(&m_achievListener); //Add from the observer list

	if (m_audioCreated == false)
	{
		m_audio.addSound("GameMusic0", Scene::resources().getMusic("Along Song"));
		m_audio.addSound("GameMusic1", Scene::resources().getMusic("Song 2"));
		m_audio.addSound("GameMusic2", Scene::resources().getMusic("Laser"));
		m_audio.addSound("GameMusic3", Scene::resources().getMusic("Pulse"));
	}
	//sets the volume for the game and plays the first music track of the game
	Mix_VolumeMusic(40);
	m_audio.playSound("GameMusic0", true);
	m_rendererPtr = NULL;
	m_gameOver = false;
	m_endGameTimer = 10; //10 seconds to show the winner
	m_startTimerEnded = false;
	m_gameStarted = false;
	m_gameStartTimer = 3; //3 Seconds
	m_physicsWorld.initWorld(); //Create the physics world
	m_physicsWorld.addContactListener(m_collisionListener); //Add collision listener to the world

	//Recreate the attack system
	static_cast<PlayerPhysicsSystem*>(Scene::systems()["Player Physics"])->setWorld(m_physicsWorld);
	Scene::systems()["Attack"] = new AttackSystem(m_physicsWorld);
	//Recreate the dust system
	Scene::systems()["Dust"] = new DustSystem(&Scene::systems(), &m_allPlayers, &Scene::resources());
	Scene::systems()["Respawn"] = new PlayerRespawnSystem();
	static_cast<PickUpSystem*>(Scene::systems()["Pickup"])->setWorld(m_physicsWorld);
	Scene::systems()["Booth"] = new DJBoothSystem(&Scene::resources(), &m_platforms, &m_bgEntity, &m_audio);

	//Create background entity
	auto bgPos = new PositionComponent(960 , 540);
	m_bgEntity.addComponent("Pos", bgPos);
	m_bgEntity.addComponent("Sprite", new SpriteComponent(bgPos, Vector2f(1920, 1080), Vector2f(1920, 1080), Scene::resources().getTexture("Game BG0"), 0));
	//Add bg sprite component to the render system
	Scene::systems()["Render"]->addComponent(&m_bgEntity.getComponent("Sprite"));

	if (static_cast<OnlineSystem*>(Scene::systems()["Network"])->isConnected)
	{
		m_numOfLocalPlayers = PreGameScene::playerIndexes.localPlyrs.size();
		m_numOfOnlinePlayers = PreGameScene::playerIndexes.onlinePlyrs.size();
		m_numOfAIPlayers = PreGameScene::playerIndexes.botPlyrs.size();
	}
	else {
		m_numOfLocalPlayers = PreGameScene::playerIndexes.localPlyrs.size();
		m_numOfOnlinePlayers = PreGameScene::playerIndexes.onlinePlyrs.size();
		m_numOfAIPlayers = PreGameScene::playerIndexes.botPlyrs.size();
	}

	//Create players, pass in the spawn locations to respawn players
	std::vector<Vector2f> spawnPos;

	for (int i = 0; i < Scene::resources().getLevelData()["Spawn Points"].size(); i++)
	{
		auto point = Scene::resources().getLevelData()["Spawn Points"].at(i);
		spawnPos.push_back(Vector2f(point["X"], point["Y"]));
	}

	achi::Listener::m_localPlayers = m_numOfLocalPlayers; //Set the amount of local players in the global space

	for (int i = 0; i < m_numOfLocalPlayers; i++)
	{
		int dex = PreGameScene::playerIndexes.localPlyrs[i].second;
		m_localPlayers.push_back(createPlayer(dex, PreGameScene::playerIndexes.localPlyrs[i].first, spawnPos.at(dex).x, spawnPos.at(dex).y, true, spawnPos));
		m_allPlayers.emplace_back(m_localPlayers.at(i)); //Add local to all players vector
	}
	for (int i = 0; i < m_numOfOnlinePlayers; i++)
	{
		int dex = PreGameScene::playerIndexes.onlinePlyrs[i];
		m_onlinePlayers.push_back(createPlayer(dex, 0, spawnPos.at(dex).x, spawnPos.at(dex).y, false, spawnPos));
		m_allPlayers.emplace_back(m_onlinePlayers.at(i)); //Add online players to all players vector
	}
	//m_numOfAIPlayers = 1;
	for (int i = 0; i < m_numOfAIPlayers; i++)
	{
		int dex = PreGameScene::playerIndexes.botPlyrs[i];
		m_AIPlayers.push_back(createAI(dex, spawnPos.at(dex).x, spawnPos.at(dex).y, true, spawnPos));
		m_allPlayers.emplace_back(m_AIPlayers.at(i)); //Add ai to all players vector
	}
	
	//pickup Entity
	m_pickUp = new Entity("PickUp");
	auto pos = new PositionComponent(0,0);
	m_pickUp->addComponent("Pos", pos);
	m_pickUp->addComponent("PickUp",new PickUpComponent(m_pickUp));
	m_pickUp->addComponent("Sprite", new SpriteComponent(&m_pickUp->getComponent("Pos"), Vector2f(1500, 50), Vector2f(50, 50), Scene::resources().getTexture("Record"), 1));
	auto anim = new AnimationComponent(&m_pickUp->getComponent("Sprite"));		
	std::vector<SDL_Rect> m_spinAnimation;
	for (int i = 0; i < 30; i++)
		m_spinAnimation.push_back({i*50, 0, 50, 50});

	//all the animations of the record and plays the spinning animation
	anim->addAnimation("Spin", Scene::resources().getTexture("Record"), m_spinAnimation, 1.75f);
	anim->playAnimation("Spin", true);

	//adds that animation component
	Scene::systems()["Animation"]->addComponent(anim);
	m_pickUp->addComponent("Animation", anim);
	Scene::systems()["Pickup"]->addComponent(&m_pickUp->getComponent("PickUp"));

	//creates an audio component adds the sound
	auto audio = new AudioComponent();
	audio->addSound("PickUp 1", Scene::resources().getSFX("PickUp 1"));
	m_pickUp->addComponent("Audio", audio);

	//DJBooths created here
	//sets their position by looping through until all three are set
	auto& booths = Scene::resources().getLevelData()["Booth"];

	for (int i = 0; i < booths.size() - 1; i++)
	{
		m_djBooths.push_back(createDJB(i, booths.at(i)["X"], booths.at(i)["Y"]));
	}

	auto& kb = Scene::resources().getLevelData()["Kill Boxes"];
	for (int i = 0; i < kb.size(); i++)
	{
		m_killboxes.push_back(createKillBox(kb.at(i)["X"], kb.at(i)["Y"], kb.at(i)["W"], kb.at(i)["H"]));
	}

	auto bannerPos = new PositionComponent(960, 1110); //Bottom of the screen
	auto iconPos = new PositionComponent(960 - 48, 1110); //Bottom of the screen
	m_achiPopup.addComponent("Pos", bannerPos);
	m_achiPopup.addComponent("Icon Pos", iconPos);
	m_achiPopup.addComponent("Sprite", new SpriteComponent(bannerPos, Vector2f(250, 50), Vector2f(250, 50), Scene::resources().getTexture("Achievement Banner"), 5)); //Draw over everything
	m_achiPopup.addComponent("Icon Sprite", new SpriteComponent(iconPos, Vector2f(500, 50), Vector2f(50, 50), NULL, 6)); //Draw over everything
	static_cast<SpriteComponent*>(&m_achiPopup.getComponent("Sprite"))->useCamera() = false;
	static_cast<SpriteComponent*>(&m_achiPopup.getComponent("Icon Sprite"))->useCamera() = false;

	auto popanim = new AnimationComponent(&m_achiPopup.getComponent("Icon Sprite"));
	std::vector<SDL_Rect> fs;
	for (int i = 0; i < 10; i++)
		fs.push_back({50*i, 0, 50, 50});
	popanim->addAnimation("Unlock", NULL, fs, 1.0f);
	m_achiPopup.addComponent("Animation", popanim);
	popanim->playAnimation("Unlock", false);

	setupUi();

	//Setup timer
	setupTimer();
}

void GameScene::setupTimer()
{
	auto pos = new PositionComponent(960, 540); //Center of the screen
	m_gameStart.addComponent("Pos", pos);
	m_gameStart.addComponent("Sprite", new SpriteComponent(pos, Vector2f(1700, 200), Vector2f(350, 200), NULL, 5)); //Layer 5 to be drawn infront of everything
	std::vector<SDL_Rect> m_animRects;
	for (int i = 0; i < 20; i++)
		m_animRects.push_back({i * 350, 0, 350, 200});
	auto anim = new AnimationComponent(&m_gameStart.getComponent("Sprite"));
	anim->addAnimation("1", Scene::resources().getTexture("Timer 1"), m_animRects, .2f);
	anim->addAnimation("2", Scene::resources().getTexture("Timer 2"), m_animRects, .2f);
	anim->addAnimation("3", Scene::resources().getTexture("Timer 3"), m_animRects, .2f);
	anim->addAnimation("Fight", Scene::resources().getTexture("Timer Fight"), m_animRects, .2f);
	anim->playAnimation("3", false); //Play the first part of the animation
	m_gameStart.addComponent("Animation", anim);

	pos = new PositionComponent(960, 540);
	m_gameEndE.addComponent("Pos", pos);
	m_gameEndE.addComponent("Sprite", new SpriteComponent(pos, Vector2f(9000, 200), Vector2f(450, 200), NULL, 5));
	auto winAnim = new AnimationComponent(&m_gameEndE.getComponent("Sprite"));
	m_animRects.clear();
	for (int i = 0; i < 20; i++)
		m_animRects.push_back({i* 450, 0, 450, 200});
	winAnim->addAnimation("Win", Scene::resources().getTexture("Winner"), m_animRects, 0.35f);
	m_gameEndE.addComponent("Animation", winAnim);

	Scene::systems()["Animation"]->addComponent(anim);
	Scene::systems()["Animation"]->addComponent(winAnim);
	Scene::systems()["Render"]->addComponent(&m_gameStart.getComponent("Sprite"));
}

void GameScene::setupUi()
{
	int index = 0;
	int maxIndex = m_allPlayers.size();
	bool done = false;

	for (int i = 0; i < 2; i++)
	{
		if (done)
			break;
		for (int j = 0; j < 2; j++)
		{
			auto ent = new Entity("UI");
			auto pos = new PositionComponent(100 + 1720 * j, 45 + 990 * i);
			auto dmg0Pos = new PositionComponent(pos->position.x + 35, pos->position.y - 14);
			auto dmg1Pos = new PositionComponent(pos->position.x + 52, pos->position.y - 14);
			auto dmg2Pos = new PositionComponent(pos->position.x + 69, pos->position.y - 14);
			auto sup0Pos = new PositionComponent(pos->position.x + 35, pos->position.y + 14);
			auto sup1Pos = new PositionComponent(pos->position.x + 52, pos->position.y + 14);
			auto sup2Pos = new PositionComponent(pos->position.x + 69, pos->position.y + 14);
			ent->addComponent("Pos", pos);
			ent->addComponent("Dmg Pos 0", dmg0Pos);
			ent->addComponent("Dmg Pos 1", dmg1Pos);
			ent->addComponent("Dmg Pos 2", dmg2Pos);
			ent->addComponent("Sup Pos 0", sup0Pos);
			ent->addComponent("Sup Pos 1", sup1Pos);
			ent->addComponent("Sup Pos 2", sup2Pos);
			ent->addComponent("Sprite", new SpriteComponent(pos, Vector2f(200, 90), Vector2f(200, 90), Scene::resources().getTexture("Portrait"), 9));

			static_cast<SpriteComponent*>(&ent->getComponent("Sprite"))->useCamera() = false;

			for (int k = 0; k < 3; k++)
			{
				auto sprite = new SpriteComponent(&ent->getComponent("Dmg Pos " + std::to_string(k)), Vector2f(320, 32), Vector2f(32, 32), Scene::resources().getTexture("Numbers Coloured"), 10);
				sprite->setTextureRect({0,0,32,32});
				sprite->useCamera() = false;
				ent->addComponent("Dmg" + std::to_string(k), sprite);
				Scene::systems()["Render"]->addComponent(&ent->getComponent("Dmg" + std::to_string(k)));
			}
			for (int k = 0; k < 3; k++)
			{
				auto sprite = new SpriteComponent(&ent->getComponent("Sup Pos " + std::to_string(k)), Vector2f(320, 32), Vector2f(32, 32), Scene::resources().getTexture("Numbers Coloured"), 10);
				sprite->setTextureRect({ 0,0,32,32 });
				sprite->useCamera() = false;
				ent->addComponent("Sup" + std::to_string(k), sprite);
				Scene::systems()["Render"]->addComponent(&ent->getComponent("Sup" + std::to_string(k)));
			}
				
			Scene::systems()["Render"]->addComponent(&ent->getComponent("Sprite"));

			ent->addComponent("Head", static_cast<SpriteComponent*>(&m_allPlayers.at(index)->getComponent("Portrait")));
			m_allPlayers.at(index)->removeComponent("Portrait");
			auto head = static_cast<SpriteComponent*>(&ent->getComponent("Head"));
			head->useCamera() = false;
			head->setPosPtr(new PositionComponent(pos->position.x - 55, pos->position.y));

			Scene::systems()["Render"]->addComponent(head);

			m_ui[m_allPlayers.at(index)] = ent;

			index++;

			//Leave if we have reached the max amount of players, then exit
			if (index == maxIndex)
			{
				done = true;
				break;
			}
		}
	}

	Scene::systems()["UI"] = new UISystem(&m_ui);
}

void GameScene::stop()
{
	removeObserver(&m_achievListener); //Remove from the observer list
	m_physicsWorld.deleteWorld(); //Delete the physics world
	m_platforms.clear(); //Delete the platforms of the game
	m_numOfLocalPlayers = 0;
	m_platformsCreated = false;
	SDL_RenderSetScale(m_rendererPtr, 1.0f, 1.0f);

	auto menuInput = Scene::systems()["Input"]->m_components.at(0);
	Scene::systems()["Input"]->removeAllComponents();
	Scene::systems()["Input"]->addComponent(menuInput);
	Scene::systems()["Render"]->removeAllComponents();
	Scene::systems()["Player Physics"]->removeAllComponents();
	Scene::systems()["Physics"]->removeAllComponents();
	Scene::systems()["Pickup"]->removeAllComponents();
	Scene::systems()["Attack"]->removeAllComponents();
	Scene::systems()["AI"]->removeAllComponents();
	Scene::systems()["Dust"]->removeAllComponents();
	Scene::systems()["Respawn"]->removeAllComponents();
	Scene::systems()["Animation"]->removeAllComponents();
	Scene::systems()["Booth"]->removeAllComponents();
	Scene::systems()["UI"]->removeAllComponents();
	Scene::systems()["Network"]->removeAllComponents();
	delete Scene::systems()["UI"];

	for (auto ai : m_AIPlayers)
		delete ai;
	for (auto player : m_localPlayers)
		delete player;
	for (auto onlineP : m_onlinePlayers)
		delete onlineP;
  
	m_audio.stop();

	m_allPlayers.clear();
	m_AIPlayers.clear();
	m_localPlayers.clear();
	m_onlinePlayers.clear();
	m_playersToDel.clear();
	m_ui.clear();
	delete m_pickUp;
	if (static_cast<OnlineSystem*>(Scene::systems()["Network"])->isConnected)
	{
		vector<int> ret;
		for (int i = 0; i < m_numOfLocalPlayers; i++)
			ret.push_back(PreGameScene::playerIndexes.localPlyrs[i].second);
		/*for (int i = 0; i < m_numOfOnlinePlayers; i++)
			ret.push_back(PreGameScene::playerIndexes.onlinePlyrs[i]);*/
		for (int i = 0; i < m_numOfAIPlayers; i++)
			ret.push_back(PreGameScene::playerIndexes.botPlyrs[i]);

		static_cast<OnlineSystem*>(Scene::systems()["Network"])->disconnect(ret);
	}
}

void GameScene::update(double dt)
{
	float scalar = static_cast<DJBoothSystem*>(Scene::systems()["Booth"])->getScalar();
	//Update the physics world, do this before ANYTHING else
	m_physicsWorld.update(dt * scalar);
	//Update the player physics system

	Scene::systems()["Player Physics"]->update(dt * scalar);
	Scene::systems()["Physics"]->update(dt * scalar);
	Scene::systems()["Attack"]->update(dt * scalar);
	Scene::systems()["Pickup"]->update(dt * scalar);
	Scene::systems()["Booth"]->update(dt);
	Scene::systems()["Animation"]->update(dt * scalar); //Update the animation components
	Scene::systems()["AI"]->update(dt * scalar);
	Scene::systems()["Dust"]->update(dt * scalar);
	Scene::systems()["Respawn"]->update(dt * scalar);
	Scene::systems()["UI"]->update(dt);

	//Update the game start timer
	updateStartTimer(dt);

	//Update camera
	updateCamera(dt * scalar);


	//Removing players from the game if they are dead
	for (auto& player : m_allPlayers)
	{
		if(static_cast<PlayerComponent&>(player->getComponent("Player")).isDead())
			m_playersToDel.emplace_back(player); //Add to the players to delete vector
	}

	//If there ar eplayers to delete, remove them from the systems and not draw them
	if (m_playersToDel.empty() == false)
	{
		for (auto& player : m_playersToDel)
		{
			Scene::systems()["Render"]->deleteComponent(&player->getComponent("Sprite"));
			Scene::systems()["Attack"]->deleteComponent(&player->getComponent("Attack"));
			Scene::systems()["Animation"]->deleteComponent(&player->getComponent("Animation"));
			Scene::systems()["Player Physics"]->deleteComponent(&player->getComponent("Player Physics"));
			Scene::systems()["Respawn"]->deleteComponent(&player->getComponent("Player"));

			m_physicsWorld.deleteBody(static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"))->m_body);
			m_physicsWorld.deleteBody(static_cast<PlayerPhysicsComponent*>(&player->getComponent("Player Physics"))->m_jumpSensor);

			if (player->m_ID == "AI")
			{
				Scene::systems()["AI"]->deleteComponent(&player->getComponent("AI"));
			}

			//Remove the player from the all players vector
			m_allPlayers.erase(std::remove(m_allPlayers.begin(), m_allPlayers.end(), player), m_allPlayers.end());
			//delete player;
			//player = nullptr;
		}

		m_playersToDel.clear();
	}

	//If only one player is in the vector, set them as the winner
	if (m_allPlayers.size() == 1 && m_gameOver == false)
	{
		//Set the player as the winner
		static_cast<PlayerComponent&>(m_allPlayers.at(0)->getComponent("Player")).isWinner() = true;
		Scene::systems()["Render"]->addComponent(&m_gameEndE.getComponent("Sprite"));
		static_cast<AnimationComponent&>(m_gameEndE.getComponent("Animation")).playAnimation("Win", false);
		m_gameOver = true;
		achi::Listener::notify(m_allPlayers.at(0), MATCH_ENDED);
		if(m_numOfOnlinePlayers != 0) //If online, send an online match ended event
			achi::Listener::notify(m_allPlayers.at(0), ONLINE_MATCH_ENDED);
	}

	updateEndGameTimer(dt);

	handleAchievementPopup(dt);
}

void GameScene::updateStartTimer(double dt)
{
	//If the game hasnt started yet, decrement the timer
	if (m_startTimerEnded == false)
	{
		auto anim = static_cast<AnimationComponent*>(&m_gameStart.getComponent("Animation"));

		m_gameStartTimer -= dt; //Take dt away from our start timer

		if (m_gameStartTimer <= -1)
		{
			Scene::systems()["Animation"]->deleteComponent(anim);
			Scene::systems()["Render"]->deleteComponent(static_cast<SpriteComponent*>(&m_gameStart.getComponent("Sprite")));
			m_startTimerEnded = true;
		}
		else if(m_gameStartTimer <= 0)
		{
			anim->playAnimation("Fight", false);
			m_gameStarted = true;
			m_camera.setActive(true);
		}
		else if (m_gameStartTimer <= 1)
		{
			anim->playAnimation("1", false);
		}
		else if (m_gameStartTimer <= 2)
		{
			anim->playAnimation("2", false);
		}
	}
}

void GameScene::updateEndGameTimer(double dt)
{
	if (m_gameOver)
	{
		m_endGameTimer -= dt;

		auto winPos = static_cast<PositionComponent*>(&m_gameEndE.getComponent("Pos"));
		auto playerPos = static_cast<PositionComponent*>(&m_allPlayers.at(0)->getComponent("Pos"));

		winPos->position.x = playerPos->position.x;
		winPos->position.y = playerPos->position.y - 100;

		if (m_endGameTimer <= 0)
		{
			Scene::goToScene("Main Menu");
		}
	}
}

void GameScene::handleAchievementPopup(double dt)
{
	if (!m_popupSet)
	{
		if (!achi::Listener::m_newUnlocks.empty())
		{
			auto achiname = achi::Listener::m_newUnlocks.back(); //Get the name in the last element
			achi::Listener::m_newUnlocks.pop_back(); //Remove the last element

			auto anim = static_cast<AnimationComponent*>(&m_achiPopup.getComponent("Animation"));
			anim->getCurrentAnimation()->setTexture(Scene::resources().getTexture(achiname));
			
			anim->playAnimation("Unlock", false);

			Scene::systems()["Animation"]->addComponent(anim);
			Scene::systems()["Render"]->addComponent(&m_achiPopup.getComponent("Sprite"));
			Scene::systems()["Render"]->addComponent(&m_achiPopup.getComponent("Icon Sprite"));

			m_popupSet = true;
			m_popupTime = 3.0f;
			m_popupHalfPoint = false;
		}
	}
	if (m_popupSet)
	{
		auto pos = static_cast<PositionComponent*>(&m_achiPopup.getComponent("Pos"));
		auto iconPos = static_cast<PositionComponent*>(&m_achiPopup.getComponent("Icon Pos"));

		if (m_popupHalfPoint == false)
		{
			if (pos->position.y > 1040)
			{
				pos->position.y -= 100 * dt;
				if (pos->position.y < 1040)
				{
					m_popupHalfPoint = true;
					pos->position.y = 1040;
					static_cast<AnimationComponent*>(&m_achiPopup.getComponent("Animation"))->getCurrentAnimation()->resetAnimation();
					static_cast<AnimationComponent*>(&m_achiPopup.getComponent("Animation"))->playAnimation("Unlock", false);
				}
			}
		}
		else
		{
			m_popupTime -= dt;

			if (m_popupTime <= 0)
			{
				pos->position.y += 100 * dt;
			}
		}
		if (pos->position.y >= 1110)
		{
			m_popupSet = false;
			pos->position.y = 1110;
			Scene::systems()["Animation"]->deleteComponent(&m_achiPopup.getComponent("Animation"));
			Scene::systems()["Render"]->deleteComponent(&m_achiPopup.getComponent("Sprite"));
			Scene::systems()["Render"]->deleteComponent(&m_achiPopup.getComponent("Icon Sprite"));
		}

		iconPos->position.y = pos->position.y;
	}
}

void GameScene::updateCamera(double dt)
{
	//The average position of the players
	auto avgPos = Vector2f(960 , 540);
	float maxDist = 0.0f;
	auto center = Vector2f();

	Vector2f lastP;
	int divisors = 0;

	for (auto& player : m_allPlayers)
	{
		//Only include the playe rif they are not dead and not respawning
		if (static_cast<PlayerComponent&>(player->getComponent("Player")).isDead() == false && 
			static_cast<PlayerComponent&>(player->getComponent("Player")).isRespawning() == false)
		{
			auto pos = static_cast<PositionComponent*>(&player->getComponent("Pos"))->position;
			divisors++;
			avgPos += pos;

			if (lastP.x != 0 && lastP.y != 0)
			{
				//Get distance
				float dist = lastP.distance(pos);

				if (dist > maxDist)
				{
					maxDist = dist;
				}
			}

			lastP = pos;
		}
	}

	if (divisors > 0)
	{
		maxDist += 50;

		auto minZoom = Vector2f(960, 540);
		auto maxZoom = Vector2f(1920 * m_camera.MAX_ZOOM / 2, 1080 * m_camera.MAX_ZOOM / 2);

		auto diff = minZoom.distance(maxZoom);
		maxDist = maxDist > diff ? diff : maxDist;

		float percentage = maxDist / diff;

		m_camera.zoom(m_camera.MAX_ZOOM - percentage * .55f);
	}

	m_camera.centerCamera(avgPos / (divisors + 1));
	//Update the camera
	m_camera.update(dt);
}

Entity * GameScene::createPlayer(int playerNumber,int controllerNumber, int posX, int posY, bool local, std::vector<Vector2f> spawnPositions)
{
	//Creates new entity of the player and adds all the appropriate components that the player needs
	auto p = new Entity("Player");
	p->addComponent("Pos", new PositionComponent(0,0));
	p->addComponent("Dust Trigger", new DustTriggerComponent());
	p->addComponent("Attack", new AttackComponent());
	p->addComponent("Player", new PlayerComponent(spawnPositions, p, playerNumber));
	p->addComponent("Sprite", new SpriteComponent(&p->getComponent("Pos"), Vector2f(1700,85), Vector2f(85, 85), Scene::resources().getTexture("Player Idle"), 2));
	auto animation = new AnimationComponent(&p->getComponent("Sprite"));
	p->addComponent("Animation", animation);
	auto audio = new AudioComponent();
	//loads in all the necessary audio files that the player eill need 
	audio->addSound("Spawn", Scene::resources().getSFX("Spawn"));
	audio->addSound("KnockOut", Scene::resources().getSFX("KnockOut"));
	audio->addSound("Punch", Scene::resources().getSFX("Punch"));
	audio->addSound("Jump", Scene::resources().getSFX("Jump"));
	audio->addSound("Whoosh", Scene::resources().getSFX("Whoosh"));
	audio->addSound("Footsteps", Scene::resources().getSFX("Footsteps"));
	p->addComponent("Audio", audio);

	std::vector<SDL_Rect> m_animRects, m_stunRects; //The rectangles for the animations

	for (int i = 0; i < 20; i++)
	{
		if (i < 10)
			m_stunRects.push_back({ 85 * i, 0, 85, 85 });
		m_animRects.push_back({85 * i, 0, 85, 85});
	}
	//adds all the necessary animations that the player will need
	animation->addAnimation("Run", Scene::resources().getTexture("Player Run" + std::to_string(playerNumber)), m_animRects, .75f);
	animation->addAnimation("Idle", Scene::resources().getTexture("Player Idle" + std::to_string(playerNumber)), m_animRects, .5f);
	animation->addAnimation("Punch 0", Scene::resources().getTexture("Player Left Punch" + std::to_string(playerNumber)), m_animRects, .175f);
	animation->addAnimation("Punch 1", Scene::resources().getTexture("Player Right Punch" + std::to_string(playerNumber)), m_animRects, .175f);
	animation->addAnimation("Ground Kick", Scene::resources().getTexture("Player Ground Kick" + std::to_string(playerNumber)), m_animRects, .4f);
	animation->addAnimation("Uppercut", Scene::resources().getTexture("Player Uppercut" + std::to_string(playerNumber)), m_animRects, .4f);
	animation->addAnimation("Jump", Scene::resources().getTexture("Player Jump" + std::to_string(playerNumber)), m_animRects, .4f);
	animation->addAnimation("Super Stun", Scene::resources().getTexture("Player Super Stun" + std::to_string(playerNumber)), m_stunRects, .25f);
	animation->addAnimation("Small Stun", Scene::resources().getTexture("Player Small Stun" + std::to_string(playerNumber)), m_stunRects, .25f);
	animation->addAnimation("Big Stun", Scene::resources().getTexture("Player Big Stun" + std::to_string(playerNumber)), m_stunRects, .25f);
	animation->playAnimation("Idle", true); //Play the idle animation from the start

	//Add components to the system
	Scene::systems()["Animation"]->addComponent(&p->getComponent("Animation"));
	Scene::systems()["Render"]->addComponent(&p->getComponent("Sprite"));
	//Add the players attack component to the attack system
	Scene::systems()["Attack"]->addComponent(&p->getComponent("Attack"));
	Scene::systems()["Respawn"]->addComponent(&p->getComponent("Player"));

	//Create and initialise the input component
	if (local) {
		auto input = new PlayerInputComponent();
		Scene::systems()["Input"]->addComponent(input);
		input->initialiseJoycon(controllerNumber);
		input->m_playerNumber = playerNumber;
		p->addComponent("Input", input);
	}
	else {
		auto input = new OnlineInputComponent();
		static_cast<OnlineSystem*>(Scene::systems()["Network"])->addReceivingPlayer(input);
		input->m_playerNumber = playerNumber;
		p->addComponent("Input", input);
	}


	//Create the physics component and set up the bodies
	auto phys = new PlayerPhysicsComponent(&p->getComponent("Pos"));
	phys->m_body = m_physicsWorld.createBox(posX, posY, 30, 78, false, false, b2BodyType::b2_dynamicBody);
	phys->m_jumpSensor = m_physicsWorld.createBox(posX, posY, 27, 5, false, false, b2BodyType::b2_dynamicBody);

	m_physicsWorld.addProperties(*phys->m_body, 1, 0.05f, 0.0f, false, new PhysicsComponent::ColData("Player Body", p));
	m_physicsWorld.addProperties(*phys->m_jumpSensor, 1, 0.05f, 0.0f, true, new PhysicsComponent::ColData("Jump Sensor", p));

	//Set the gravity scale to 2, this makes the player less floaty
	phys->m_body->getBody()->SetGravityScale(2.0f);

	//Create the joint between the player and the jump sensor
	phys->createJoint(m_physicsWorld);

	//Try to add a sender to the server
	auto netSys = static_cast<OnlineSystem*>(Scene::systems()["Network"]);
	if (netSys->isConnected && local)
	{
		auto net = new OnlineSendComponent();
		net->m_playerNumber = playerNumber;
		p->addComponent("Send", net);
		netSys->addSendingPlayer(net);
	} //if it can't connect to the server, it didn't need to be online anyway

	//Add the components to the entity
	p->addComponent("Player Physics", phys);

	//Add the physics component to the player physics system
	Scene::systems()["Player Physics"]->addComponent(phys);


	//Add the head picture to the playe rso the ui can get it from the player and display it in the correct corner
	p->addComponent("Portrait", new SpriteComponent(nullptr, Vector2f(59, 65), Vector2f(59, 65), Scene::resources().getTexture("Head" + std::to_string(playerNumber)), 11));
	static_cast<SpriteComponent*>(&p->getComponent("Portrait"))->setScale((playerNumber == 0 || playerNumber == 2) ? -1 : 1, 1);

	return p; //Return the created entity
}

Entity * GameScene::createKillBox(int posX, int posY, int width, int height)
{
	auto kb = new Entity("KillBox");
	auto pos = new PositionComponent(0,0);
	kb->addComponent("Pos", pos);
	auto phys = new PhysicsComponent(pos);
	phys->m_body = m_physicsWorld.createBox(posX, posY, width, height, false, false, b2_staticBody);
	m_physicsWorld.addProperties(*phys->m_body, 0, 0, 0, true, new PhysicsComponent::ColData("Kill Box", kb));
	kb->addComponent("Physics", phys);
	Scene::systems()["Physics"]->addComponent(phys);
	return kb;
}

Entity* GameScene::createDJB(int index, int posX, int posY)
{
	//creates an entity for the DJ booths
	auto booth = new Entity("Booth");
	auto pos = new PositionComponent(0, 0);
	booth->addComponent("Pos", pos);

	//creates a Box2d body for the djbooth defines its propoerties and applies a sprite
	auto phys = new PhysicsComponent(pos);
	phys->m_body = m_physicsWorld.createBox(posX, posY, 150, 50, false, false, b2BodyType::b2_staticBody);
	m_physicsWorld.addProperties(*phys->m_body, 1, 0.05f, 0.0f, true, new PhysicsComponent::ColData("Booth", booth));
	booth->addComponent("Physics", phys);
	Scene::systems()["Physics"]->addComponent(phys);
	booth->addComponent("Sprite", new SpriteComponent(pos, Vector2f(152, 93), Vector2f(152, 93), Scene::resources().getTexture("Booth" + std::to_string(index)), 1));
	Scene::systems()["Render"]->addComponent(&booth->getComponent("Sprite"));
	auto audio = new AudioComponent();
	//adds all the appropriate audio for the DJ Booths
	audio->addSound("Switch", Scene::resources().getSFX("Switch"));
	audio->addSound("Switch2", Scene::resources().getSFX("Switch2"));
	booth->addComponent("Audio", audio);

	//Index for the three booths so that it spawns the correct booth in the correct location with the right sprite
	if (index == 0)
	{	
		booth->addComponent("DJ Booth", new GravityBoothComponent(m_allPlayers, &m_physicsWorld, static_cast<PlayerPhysicsSystem*>(Scene::systems()["Player Physics"]), &m_collisionListener, m_pickUp));		
	}
	else if (index == 1)
	{
		
		booth->addComponent("DJ Booth", new SlowBoothComponent(m_pickUp));
	}
	else if (index == 2)
	{
		
		booth->addComponent("DJ Booth", new PlatformBoothComponent(&m_platforms, m_pickUp));
	}

	Scene::systems()["Booth"]->addComponent(&booth->getComponent("DJ Booth"));
	return booth;
}

/// <summary>
/// 
/// </summary>
/// <param name="index"></param>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <returns></returns>
Entity * GameScene::createAI(int index, int posX, int posY, bool local, std::vector<Vector2f> spawnPositions)
{
	auto ai = new Entity("AI");
	auto pos = new PositionComponent(0, 0);
	auto input = new AiInputComponent();
	auto player = new PlayerComponent(spawnPositions, ai, index);

	ai->addComponent("Input", input);
	ai->addComponent("Pos", pos);
	ai->addComponent("Player", player);
	ai->addComponent("Dust Trigger", new DustTriggerComponent());
	ai->addComponent("Player", player);
	ai->addComponent("Attack", new AttackComponent());
	ai->addComponent("Sprite", new SpriteComponent(&ai->getComponent("Pos"), Vector2f(1700, 85), Vector2f(85, 85), Scene::resources().getTexture("Player Run"), 2));
	auto animation = new AnimationComponent(&ai->getComponent("Sprite"));
	auto behaviour = new AIComponent(&m_allPlayers, input, ai, player, m_physicsWorld);
	ai->addComponent("AI", behaviour);
	auto audio = new AudioComponent();
	audio->addSound("Spawn", Scene::resources().getSFX("Spawn"));
	audio->addSound("KnockOut", Scene::resources().getSFX("KnockOut"));
	audio->addSound("Punch", Scene::resources().getSFX("Punch"));
	audio->addSound("Jump", Scene::resources().getSFX("Jump"));
	audio->addSound("Whoosh", Scene::resources().getSFX("Whoosh"));
	audio->addSound("Footsteps", Scene::resources().getSFX("Footsteps"));
	ai->addComponent("Audio", audio);
	std::vector<SDL_Rect> m_animRects, m_stunRects; //The rectangles for the animations

	for (int i = 0; i < 20; i++)
	{
		if (i < 10)
			m_stunRects.push_back({ 85 * i, 0, 85, 85 });
		m_animRects.push_back({ 85 * i, 0, 85, 85 });
	}

	animation->addAnimation("Run", Scene::resources().getTexture("Player Run" + std::to_string(index)), m_animRects, .75f);
	animation->addAnimation("Idle", Scene::resources().getTexture("Player Idle" + std::to_string(index)), m_animRects, .5f);
	animation->addAnimation("Punch 0", Scene::resources().getTexture("Player Left Punch" + std::to_string(index)), m_animRects, .175f);
	animation->addAnimation("Punch 1", Scene::resources().getTexture("Player Right Punch" + std::to_string(index)), m_animRects, .175f);
	animation->addAnimation("Ground Kick", Scene::resources().getTexture("Player Ground Kick" + std::to_string(index)), m_animRects, .4f);
	animation->addAnimation("Uppercut", Scene::resources().getTexture("Player Uppercut" + std::to_string(index)), m_animRects, .4f);
	animation->addAnimation("Jump", Scene::resources().getTexture("Player Jump" + std::to_string(index)), m_animRects, .4f);
	animation->addAnimation("Super Stun", Scene::resources().getTexture("Player Super Stun" + std::to_string(index)), m_stunRects, .25f);
	animation->addAnimation("Small Stun", Scene::resources().getTexture("Player Small Stun" + std::to_string(index)), m_stunRects, .25f);
	animation->addAnimation("Big Stun", Scene::resources().getTexture("Player Big Stun" + std::to_string(index)), m_stunRects, .25f);
	animation->playAnimation("Idle", true);
	ai->addComponent("Animation", animation);
	
	//Add the AI component to the AI system
	Scene::systems()["AI"]->addComponent(&ai->getComponent("AI"));

	//Add AIinput component to the input system
	Scene::systems()["Input"]->addComponent(input);

	//Add the players attack component to the attack system
	Scene::systems()["Attack"]->addComponent(&ai->getComponent("Attack"));

	Scene::systems()["Render"]->addComponent(&ai->getComponent("Sprite"));

	Scene::systems()["Animation"]->addComponent(&ai->getComponent("Animation"));

	Scene::systems()["Respawn"]->addComponent(&ai->getComponent("Player"));

	//Create the physics component and set up the bodies
	auto phys = new PlayerPhysicsComponent(&ai->getComponent("Pos"));
	phys->m_body = m_physicsWorld.createBox(posX, posY, 30, 78, false, false, b2BodyType::b2_dynamicBody);
	phys->m_jumpSensor = m_physicsWorld.createBox(posX, posY, 27, 5, false, false, b2BodyType::b2_dynamicBody);

	m_physicsWorld.addProperties(*phys->m_body, 1, 0.05f, 0.0f, false, new PhysicsComponent::ColData("Player Body", ai));
	m_physicsWorld.addProperties(*phys->m_jumpSensor, 1, 0.05f, 0.0f, true, new PhysicsComponent::ColData("Jump Sensor", ai));

	//Set the gravity scale to 2, this makes the player less floaty
	phys->m_body->getBody()->SetGravityScale(2.0f);

	//Create the joint between the player and the jump sensor
	phys->createJoint(m_physicsWorld);


	//Try to add a sender to the server
	auto netSys = static_cast<OnlineSystem*>(Scene::systems()["Network"]);
	if (netSys->isConnected && local)
	{
		auto net = new OnlineSendComponent();
		net->m_playerNumber = index;
		ai->addComponent("Send", net);
		netSys->addSendingPlayer(net);
	} //if it can't connect to the server, it didn't need to be online anyway

	//Add the components to the entity
	ai->addComponent("Player Physics", phys);

	ai->addComponent("Portrait", new SpriteComponent(nullptr, Vector2f(59, 65), Vector2f(59, 65), Scene::resources().getTexture("Head" + std::to_string(index)), 11));
	static_cast<SpriteComponent*>(&ai->getComponent("Portrait"))->setScale((index == 0 || index == 2) ? -1 : 1, 1);

	//Add the physics component to the playe rphysics system
	Scene::systems()["Player Physics"]->addComponent(phys);

	return ai;
}

/// <summary>
/// 
/// </summary>
/// <param name="renderer"></param>
void GameScene::createPlatforms(SDL_Renderer& renderer)
{	
	//Create all of the platforms for the game
	for (auto& platform : Scene::resources().getLevelData()["Platforms"])
	{
		auto platComp = new PlatformComponent(); //Create the platform component
		//Get the X,Y,Width and Height of the platform
		int x = platform["X"], y = platform["Y"], w = platform["W"], h = platform["H"], angle = platform["Angle"];
		std::string tag = platform["Tag"];

		//Creta ethe platform entity
		auto newPlat = new Entity(tag);
		auto platPos = new PositionComponent(x, y);
		newPlat->addComponent("Pos", platPos);
		newPlat->addComponent("Platform", platComp);
		auto phys = new PhysicsComponent(platPos);
		phys->m_body = m_physicsWorld.createBox(x, y, angle != 90 ? w : h, angle != 90 ? h : w, false, true, b2BodyType::b2_staticBody);
		m_physicsWorld.addProperties(*phys->m_body, 0, .1f, 0, false, new PhysicsComponent::ColData(tag, newPlat));
		newPlat->addComponent("Physics", phys);
		Scene::systems()["Physics"]->addComponent(phys);

		SDL_Rect rect, srcRect;

		auto size = Vector2f(50 * (tag == "Floor" ? 1 : .5f), 50 * (tag == "Floor" ? 1 : .5f));
		auto smallW = tag == "Floor" ? 50 : 25;
		auto offset = Vector2f(size.x - w, size.y - w);
		auto start = Vector2f((int)x - ((int)w / 2), (int)y - ((int)h / 2));
		int numOfTiles = w / (tag == "Floor" ? 50 : 25);

		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = 50;
		srcRect.h = 50;

		std::vector<std::string> colours = { "Green", "Pink", "Orange", "Blue" };
		//Loop 4 times, create textures for each colour

		for (auto& col : colours)
		{
			//Create the texture for the platform
			auto texture = SDL_CreateTexture(&renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, w, h);
			SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
			//Set it to draw to the texture we just created
			SDL_SetRenderTarget(&renderer, texture);

			//Loop through the tiles and draw to the texture we just created the shape of the platform
			for (int i = 0; i < numOfTiles; i++)
			{
				rect.w = size.x;
				rect.h = size.y;
				rect.x = i * smallW ;
				rect.y = 0;

				if (i == 0)
				{
					SDL_RenderCopy(&renderer, Scene::resources().getTexture("Platform " + col + " 0"), &srcRect, &rect);
					SDL_RenderCopy(&renderer, Scene::resources().getTexture("Platform " + col + " 0"), &srcRect, &rect);
				}
				else if (i == (numOfTiles - 1))
				{
					SDL_RenderCopy(&renderer, Scene::resources().getTexture("Platform " + col + " 2"), &srcRect, &rect);
					SDL_RenderCopy(&renderer, Scene::resources().getTexture("Platform " + col + " 2"), &srcRect, &rect);
				}
				else
				{
					SDL_RenderCopy(&renderer, Scene::resources().getTexture("Platform " + col + " 1"), &srcRect, &rect);
					SDL_RenderCopy(&renderer, Scene::resources().getTexture("Platform " + col + " 1"), &srcRect, &rect);
				}
			}
			SDL_SetRenderTarget(&renderer, NULL);
			SDL_RenderCopy(&renderer, texture, NULL, &rect);

			if (col == "Green")
				platComp->setGreen(texture);
			else if (col == "Blue")
				platComp->setBlue(texture);
			else if (col == "Pink")
				platComp->setPink(texture);
			else
				platComp->setOrange(texture);
		}

		//Set the texture of the platform to the green platform texture
		newPlat->addComponent("Sprite", new SpriteComponent(platPos, Vector2f(w, h), Vector2f(w, h), platComp->getTexture("Game BG0"), 1));
		static_cast<SpriteComponent*>(&newPlat->getComponent("Sprite"))->setAngle(angle);
		Scene::systems()["Render"]->addComponent(&newPlat->getComponent("Sprite"));
		m_platforms.push_back(newPlat);
	}

	//DJBooths created here 
	auto& booths = Scene::resources().getLevelData()["Booth"];

	m_djBooths.push_back(createDJB(2, booths.at(2)["X"], booths.at(2)["Y"]));

	//Set platforms created as true
	m_platformsCreated = true;
}

/// <summary>
/// 
/// </summary>
/// <param name="renderer"></param>
void GameScene::draw(SDL_Renderer & renderer)
{
	if (NULL == m_rendererPtr)
		m_rendererPtr = &renderer; 

	if (m_platformsCreated == false)
		createPlatforms(renderer);
	
	SDL_Rect rect;

	//Draw sprites in the render system
	auto renderSystem = static_cast<RenderSystem*>(Scene::systems()["Render"]);
	renderSystem->render(renderer, m_camera);
}

void GameScene::handleInput(InputSystem & input)
{
	//Update the input system
	Scene::systems()["Input"]->update(0);

	//Only check for input if the game has started
	if (m_gameStarted)
	{
		for (auto& player : m_allPlayers)
		{
			auto input = dynamic_cast<InputComponent*>(&player->getComponent("Input"));
			auto onlineInput = dynamic_cast<OnlineInputComponent*>(&player->getComponent("Input"));
			auto aiInput = dynamic_cast<AiInputComponent*>(&player->getComponent("Input"));
			if (nullptr != aiInput)
			{
				aiInput->handleInput("", player);
			}
			else if (nullptr != onlineInput)
			{
				onlineInput->handleInput(player);
			}
			else if (nullptr != input)
			{
				input->handleInput(player);
			}
		}
	}
}