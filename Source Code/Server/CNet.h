#pragma once

#include <iostream>
#include <string>
#include <SDL_net.h>


class CNet {
public:
	static bool Init(); //Initialize SDL_net
	static void Quit(); //Exit SDL_net
};