#include "CNet.h"

bool CNet::Init()
{
	if (SDLNet_Init() < 0) {
		std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
		return false;
	}
	else
		return true;
}

void CNet::Quit()
{
	SDLNet_Quit();
}
