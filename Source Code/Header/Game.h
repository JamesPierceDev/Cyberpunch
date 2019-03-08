#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include <bitset>
#include <thread>

#include "MenuManager.h" //For menus
#include "ResourceManager.h" //For resource loading

//Systems
#include "AnimationSystem.h"
#include "RenderSystem.h"
#include "PlayerPhysicsSystem.h"
#include "AISystem.h"
#include "Component.h"
//#include "RenderComponent.h"
#include "AIComponent.h"
#include "AISystem.h"
#include "RenderSystem.h"
#include "Entity.h"
#include "AttackSystem.h"
#include "PickUpSystem.h"
#include "OnlineSystem.h"

class Game
{
public:
	Game(int fps);

	//Update the game passing in delta time
	void update(double dt);

	//Draw game components
	void draw();

	void processEvents(SDL_Event& e);

	void run();
	//Starts up SDL and creates window
	bool init();
	//Loads media
	bool loadMedia();
	//Frees media and shuts down SDL
	void close();

private:
	//Screen dimension constants
	const int SCREEN_WIDTH = 1920;
	const int SCREEN_HEIGHT = 1080;

	//The window we'll be rendering to
	SDL_Window* m_window;
	//The surface contained by the window
	SDL_Surface* m_screenSurface;
	//The renderer, we will use this to draw images
	SDL_Renderer* m_renderer;

	float m_msPerFrame;
	bool m_quit;

	//Our menu manager to handle our scenes
	MenuManager m_mManager;
	//Our resource handler
	ResourceHandler m_resources;

	//Our map of systems
	std::map<std::string, System*> m_systems;
};

#endif