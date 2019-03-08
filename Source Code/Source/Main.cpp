#include "Game.h"

int main(int argc, char* args[])
{
	//Create new game objetc with target fps of 60
	auto game = Game(60);

	//If the game didnt fail to initialise
	if (game.init())
	{
		//If the game didnt fail to load resources
		if (game.loadMedia())
		{
			//Run the game
			game.run();
		}
	}

	return 0;
}