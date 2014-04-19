#include <iostream>
#include <math.h>
#include <time.h>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include "GTDGame.h"

int main(int argc, char **argv)
{
	srand(time(0));
	if (SDL_Init(SDL_INIT_EVERYTHING)){
		std::cout << "Could not execute SDL_Init. Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	int audio_rate = 44100;
	int audio_format = AUDIO_S16;
	int audio_channels = 2;
	int audio_buffers = 4096;
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers)) {
		printf("Unable to open audio!\n");
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
	SDL_Quit();
	return 0;
}