#include "ResourceManager.h"

ResourceHandler::ResourceHandler(std::string filePath) :
	m_filePath(filePath)
{
}

ResourceHandler::~ResourceHandler()
{
}

//Load all of our textures we need for the game
void ResourceHandler::loadTextures(SDL_Renderer& renderer)
{
	//Load level data first
	loadLevelData();

	//Load achievement data
	loadAchievements();

	//Load textures here
	//You do not need to include the entire path, the resource manager
	//Will look for everything in the Resources folder, so you then need to only provide th erest of the path to the file
	//Like so
	//loadFromPath("Player.png")
	//If you had the player.png in another folder, then youd call it like this
	//loadFromPath("Characters/Player.png")

	m_map["Game BG0"] = loadFromPath("Backgrounds-01.png", renderer);
	m_map["Game BG1"] = loadFromPath("Backgrounds-02.png", renderer);
	m_map["Game BG2"] = loadFromPath("Backgrounds-03.png", renderer);
	m_map["Game BG3"] = loadFromPath("Backgrounds-04.png", renderer);
	m_map["Credits"] = loadFromPath("credits.png", renderer);

	auto colours = std::vector<std::string>({ "Green", "Blue", "Orange", "Pink" });

	for (int i = 0; i < 4; i++)
	{
		m_map["Player Run" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Player_Run.png", renderer);
		m_map["Player Idle" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Player_Idle.png", renderer);
		m_map["Player Ground Kick" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Player_Kick.png", renderer);
		m_map["Player Jump" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Player_Jump.png", renderer);
		m_map["Player Super Stun" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Player_Super_Stun.png", renderer);
		m_map["Player Small Stun" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Player_Small_Stun.png", renderer);
		m_map["Player Big Stun" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Player_Big_Stun.png", renderer);
		m_map["Player Uppercut" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Player_Uppercut.png", renderer);
		m_map["Player Left Punch" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Left_Hand_Punch.png", renderer);
		m_map["Player Right Punch" + std::to_string(i)] = loadFromPath("Player/" + colours.at(i) + "/Right_Hand_Punch.png", renderer);
	}

	m_map["Player Run"] = loadFromPath("Player/Player_Run.png", renderer);
	m_map["Player Idle"] = loadFromPath("Player/Player_Idle.png", renderer);
	m_map["Player Ground Kick"] = loadFromPath("Player/Player_Kick.png", renderer);
	m_map["Player Dust"] = loadFromPath("Player/Dust.png", renderer);
	m_map["Player Left Punch"] = loadFromPath("Player/Left_Hand_Punch.png", renderer);
	m_map["Player Right Punch"] = loadFromPath("Player/Right_Hand_Punch.png", renderer);
	m_map["Player Jump"] = loadFromPath("Player/Player_Jump.png", renderer);
	m_map["Player Super Stun"] = loadFromPath("Player/Player_Super_Stun.png", renderer);
	m_map["Player Small Stun"] = loadFromPath("Player/Player_Small_Stun.png", renderer);
	m_map["Player Big Stun"] = loadFromPath("Player/Player_Big_Stun.png", renderer);
	m_map["Record"] = loadFromPath("Record/Record.png", renderer);
	//Load all 3 platform pieces for each colour
	for (int i = 0; i < 3; i++)
	{
		m_map["Booth" + std::to_string(i)] = loadFromPath("DJBooth/DJBooth" + std::to_string(i) + ".png", renderer);
	}

	//Loading in GUI
	m_map["Button BG"] = loadFromPath("GUI/Button_BG.png", renderer);
	m_map["Exit Button"] = loadFromPath("GUI/ExitButton.png", renderer);
	m_map["Local Play Button"] = loadFromPath("GUI/LocalButton.png", renderer);
	m_map["Multiplayer Button"] = loadFromPath("GUI/MultiButton.png", renderer);
	m_map["Options Button"] = loadFromPath("GUI/OptionsButton.png", renderer);
	m_map["Lobby BG"] = loadFromPath("GUI/Lobby_Browser.png", renderer);
	m_map["Lobby Bar"] = loadFromPath("GUI/Lobby_Info_Bar.png", renderer);
	m_map["Lobby Bar Selected"] = loadFromPath("GUI/Lobby_Info_Bar_Selected.png", renderer);
	m_map["Numbers"] = loadFromPath("GUI/Numbers.png", renderer);
	m_map["Password Yes"] = loadFromPath("GUI/Yes_Password.png", renderer);
	m_map["Password No"] = loadFromPath("GUI/No_Password.png", renderer);
	m_map["Achievements Button"] = loadFromPath("GUI/AchievementsButton.png", renderer);
	m_map["Achievements BG"] = loadFromPath("GUI/Achievements_Screen.png", renderer);
	m_map["Achievement Locked"] = loadFromPath("Achievements/Achievement_Locked.png", renderer);
	m_map["Achievement Selected"] = loadFromPath("Achievements/Achievement Selected.png", renderer);
	m_map["Achievement Des Box"] = loadFromPath("Achievements/Achievement Description Box.png", renderer);
	m_map["Achievement Banner"] = loadFromPath("Achievements/Achievement_Banner.png", renderer);
	m_map["Portrait"] = loadFromPath("GUI/Portrait.png", renderer);
	m_map["Numbers Coloured"] = loadFromPath("GUI/Numbers Coloured.png", renderer);
	m_map["Logo"] = loadFromPath("Logo.png", renderer);
	m_map["Head0"] = loadFromPath("GUI/Green Head.png", renderer);
	m_map["Head1"] = loadFromPath("GUI/Blue Head.png", renderer);
	m_map["Head2"] = loadFromPath("GUI/Orange Head.png", renderer);
	m_map["Head3"] = loadFromPath("GUI/Pink Head.png", renderer);
	m_map["Pre Game BG"] = loadFromPath("GUI/Pre_Game_BG.png", renderer);
	m_map["Player Indicator"] = loadFromPath("GUI/Player Indicator.png", renderer);
	m_map["Cpu Indicator"] = loadFromPath("GUI/Cpu Indicator.png", renderer);

	for(int i = 0; i < colours.size(); i++)
		m_map["Pre Game Head" + std::to_string(i)] = loadFromPath("GUI/Pre Game Head" + std::to_string(i) + ".png", renderer);


	std::vector<std::string> names({ "Punch First",
		"Final Form",
		"First Blood",
		"Damage Dealer",
		"Stunner",
		"Eh Aye",
		"Friends!",
		"Bend it like Beckham",
		"Last Woman Standing",
		"Quitter...",
		"Punching Bag",
		"Party Time",
		"The Upside Down",
		"Going Down",
		"Back to The Future",
		"Mover" });

	//Achievements
	for (int i = 0; i < names.size(); i++)
	{
		m_map[names.at(i)] = loadFromPath("Achievements/" + names.at(i) + ".png", renderer);
		m_map[names.at(i) + " Des"] = loadFromPath("Achievements/" + names.at(i) + " Des.png", renderer);
	}

	//Game start animations
	m_map["Timer 1"] = loadFromPath("GUI/1 Timer.png", renderer);
	m_map["Timer 2"] = loadFromPath("GUI/2 Timer.png", renderer);
	m_map["Timer 3"] = loadFromPath("GUI/3 Timer.png", renderer);
	m_map["Timer Fight"] = loadFromPath("GUI/Fight Timer.png", renderer);
	m_map["Winner"] = loadFromPath("GUI/Winner.png", renderer);

	//The colours of the platforms
	for (auto& colour : colours)
	{
		//Load all 3 platform pieces for each colour
		for (int i = 0; i < 3; i++)
		{
			m_map["Platform " + colour + " " + std::to_string(i)] = loadFromPath("Platforms/" + colour + "/" + std::to_string(i) + ".png", renderer);
		}
	}

	//All the Audio Files
	m_music["Along Song"] = MusicLoadFromPath("Audio/ATG.ogg");
	m_sfx["Calculations"] = SFXLoadFromPath("Audio/Calculations.wav");
	m_sfx["Punch"] = SFXLoadFromPath("Audio/Fight_Hits.wav");
	m_sfx["FootStep"] = SFXLoadFromPath("Audio/Footsteps.wav");
	m_music["Good Song"] = MusicLoadFromPath("Audio/GoodOneToHave.ogg");
	m_sfx["Loading"] = SFXLoadFromPath("Audio/Loading.wav");
	m_sfx["Items 1"] = SFXLoadFromPath("Audio/Objects_Items.wav");
	m_sfx["PickUp 1"] = SFXLoadFromPath("Audio/PickUp1.wav");
	m_sfx["NightPulse"] = SFXLoadFromPath("Audio/nightPulse.wav");
	m_sfx["Splash Screen"] = SFXLoadFromPath("Audio/PossibleSplashScreen.wav");
	m_music["Sergey"] = MusicLoadFromPath("Audio/Sergey.ogg");
	m_sfx["Slow Heavy"] = SFXLoadFromPath("Audio/SlowHeavy.wav");
	m_music["Song 2"] = MusicLoadFromPath("Audio/song2.ogg");
	m_sfx["Spawn"] = SFXLoadFromPath("Audio/Spawn.wav");
	m_sfx["Spawn2"] = SFXLoadFromPath("Audio/Spawn2.wav");
	m_sfx["Weapon"] = SFXLoadFromPath("Audio/Weapon.wav");
	m_sfx["KnockOut"] = SFXLoadFromPath("Audio/KnockedOut.wav");
	m_sfx["Switch"] = SFXLoadFromPath("Audio/noidea.wav");
	m_sfx["Switch2"] = SFXLoadFromPath("Audio/Switch2.wav");
	m_sfx["Jump"] = SFXLoadFromPath("Audio/locked.ogg");
	m_sfx["Whoosh"] = SFXLoadFromPath("Audio/whoosh.ogg");
	m_sfx["Neon"] = SFXLoadFromPath("Audio/neonBuzz.wav");
	m_sfx["Neon2"] = SFXLoadFromPath("Audio/neon.wav");
	m_music["Pulse"] = MusicLoadFromPath("Audio/Song3.ogg");
	m_music["Laser"] = MusicLoadFromPath("Audio/Laser.ogg");

	
}

void ResourceHandler::loadLevelData()
{
	//Open an ifstream on the file
	std::ifstream ifs(m_filePath + "LevelData.txt");

	//Load the data into the string content
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	//Parse the loaded in file to a json object
	m_gameData = json::parse(content);
}

void ResourceHandler::loadAchievements()
{
	//Open an ifstream on the file
	std::ifstream ifs(m_filePath + "Achievements.txt");

	//Load the data into the string content
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));

	//Parse the loaded in file to a json object
	m_achievementData = json::parse(content);
}

SDL_Texture* ResourceHandler::loadFromPath(std::string fileName, SDL_Renderer& renderer)
{
	std::string path = m_filePath + fileName;
	SDL_Texture* newTexture = NULL; //This will be the loaded texture

	//Load image
	SDL_Surface* loadedS = IMG_Load(path.c_str());
	if (loadedS == NULL)
		std::cout << "Error loading image at path " << path << std::endl;
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(&renderer, loadedS);
		if (newTexture == NULL)
			std::cout << "Unable to create texture" << std::endl;

		//Get rid of old surface
		SDL_FreeSurface(loadedS);
	}

	return newTexture;
}

Mix_Chunk * ResourceHandler::SFXLoadFromPath(std::string fileName)
{
	std::string path = m_filePath + fileName;

	auto m = Mix_LoadWAV(path.c_str());
	if (NULL == m)
	{
		printf("Failed to load sounds! SDL_mixer Error: %s\n", Mix_GetError());
	}

	return m;
}

Mix_Music * ResourceHandler::MusicLoadFromPath(std::string fileName)
{
	std::string path = m_filePath + fileName;

	auto m = Mix_LoadMUS(path.c_str());
	if (NULL == m)
	{
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
	}

	return m;
}

void ResourceHandler::destroyTextures()
{
	//Loop through our map
	for (auto& texture : m_map)
	{
		//Destroy the texture
		SDL_DestroyTexture(texture.second);
	}
}

TTF_Font* ResourceHandler::loadFont(std::string filePath, std::string name)
{
	const char * path = filePath.c_str();
	m_fontMap[name] = TTF_OpenFont(path, 32);
	return m_fontMap[name];
}

TTF_Font * ResourceHandler::getFont(std::string name)
{
	return m_fontMap[name];
}

SDL_Texture * ResourceHandler::getTexture(std::string name)
{
	return m_map.at(name);
}

Mix_Chunk * ResourceHandler::getSFX(std::string name)
{
	return m_sfx[name];
}

Mix_Music * ResourceHandler::getMusic(std::string name)
{
	return m_music[name];
}
