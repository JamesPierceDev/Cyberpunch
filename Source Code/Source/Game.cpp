#include "Game.h"
#include "SDL_mixer.h"
#include "AchievementComponent.h"

std::vector<Observer*> achi::Listener::obs = {};
Component* achi::Listener::m_AchisPtr = nullptr;
std::vector<std::string> achi::Listener::m_newUnlocks = {};
int achi::Listener::m_localPlayers = 0;
bool achi::Listener::m_exit = false;

Game::Game(int fps) :
	m_msPerFrame(fps / 60.0f), //Get the target fps
	m_window(NULL),
	m_screenSurface(NULL),
	m_quit(false),
	m_resources("./Resources/")
{
	//Create our systems and add them to our system map
	m_systems["Render"] = new RenderSystem();
	m_systems["Animation"] = new AnimationSystem();
	m_systems["Input"] = new InputSystem();
	m_systems["Physics"] = new PhysicsSystem();
	m_systems["Player Physics"] = new PlayerPhysicsSystem();
	m_systems["AI"] = new AISystem();
	m_systems["Network"] = new OnlineSystem();
	m_systems["Pickup"] = new PickUpSystem(m_systems["Render"], static_cast<OnlineSystem*>(m_systems["Network"]));
}

void Game::update(double dt)
{
	//Update the menu manager
	m_mManager.update(dt);
}

void Game::draw()
{
	//Clear the screen
	SDL_RenderClear(m_renderer);

	//Draw the current scene
	m_mManager.draw(*m_renderer);
	//testSystem->render(*m_renderer);

	//Render everything drawn to the renderer
	SDL_RenderPresent(m_renderer);
}

void Game::processEvents(SDL_Event& e)
{
	//Handle events on queue
	while (SDL_PollEvent(&e) != 0)
	{
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			//Set our bool to true
			m_quit = true;
		}

		//If keydown event
		if (e.type == SDL_KEYDOWN)
		{
			//If key is escape
			if (e.key.keysym.sym == SDLK_ESCAPE)
			{
				//Exit game
				m_quit = true;
			}
		}
	}
}


void Game::run()
{

	//Create our SDL event variable
	SDL_Event e;
	double dt = 0;
	auto now = std::chrono::system_clock::now();
	auto before = std::chrono::system_clock::now();

	//While our bool is false, loop indefinitely
	while (!m_quit)
	{
		m_quit = achi::Listener::m_exit;
		now = std::chrono::system_clock::now();
		dt = std::chrono::duration<double>(now - before).count();
		//Process any events that have occured
		processEvents(e);

		//Get the input system and pass it to the handleInput
		auto input = static_cast<InputSystem*>(m_systems["Input"]);

		//handle input in the scenes
		m_mManager.handleInput(*input);

		//update networked aspects
		auto net = static_cast<OnlineSystem*>(m_systems["Network"]);
		net->update(dt);

		//Update the game
		update(dt);

		//Draw the Game
		draw();
		

		//Make before time equal to the current time
		before = now;
	}

	//Call close if our game loop is over
	close();
}

bool Game::init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		m_window = SDL_CreateWindow("Cyber Punch", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (m_window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			if (TTF_Init() < 0)
			{
				printf("TTF_Init: %s\n", TTF_GetError());
			}

			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
			{
				printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
				success = false;
			}

			//Initialize PNG loading 
			int imgFlags = IMG_INIT_PNG; 
			if( !( IMG_Init( imgFlags ) & imgFlags ) )
			{ printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() ); }

			m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
			//Get window surface
			m_screenSurface = SDL_GetWindowSurface(m_window);
		}

	}
	return success;
}

bool Game::loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load any media here, we call our resource manager here
	m_resources.loadTextures(*m_renderer);
	m_mManager.setResourceHandler(m_resources);
	m_mManager.setSystemPtr(m_systems);	

	//Setup the achievement component
	m_mManager.m_scenes["Achievements"]->achievements().setAchievementData(&m_resources.getAchievementData());
	achi::Listener::m_AchisPtr = &m_mManager.m_scenes["Achievements"]->achievements();

	//Set the scene after the systems ptr has been set and the resource manager has been passed over
	m_mManager.setScene("Main Menu");

	return success;
}

void Game::close()
{
	//Destroy window
	SDL_DestroyWindow(m_window);
	m_window = NULL;

	TTF_Quit();

	//Quit SDL subsystems
	SDL_Quit();
}