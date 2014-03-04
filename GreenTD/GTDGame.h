#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <algorithm>    // std::max/min
#include "GTDPlayer.h"
#include "GTDMap.h"
#include "GTDTimer.h"

class GTDGame
{
public:
	GTDGame();
	bool init();
	void run();
	void quit();
private:
	bool loadTextures();
	int IsGameOver;
	int debug = 1;

	int mouseX;
	int mouseY;
	
	int screenX;
	int screenY;

	SDL_Window *window;
	SDL_Renderer *renderer;

	GTDMap map;
	GTDPlayer player;

};