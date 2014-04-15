#include <iostream>
#include <math.h>
#include <time.h>
#include <string>
#include <SDL.h>
#include "GTDGame.h"

int main(int argc, char **argv)
{
	srand(time(0));
	if (SDL_Init(SDL_INIT_EVERYTHING)){
		std::cout << "Could not execute SDL_Init. Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	GTDGame game;
	if (game.init())
	{
		game.run();
	}
	else
	{
		std::cout << "Could not initialize SDL or game. Exiting!" << std::endl;
		return 1;
	}
	return 0;
}