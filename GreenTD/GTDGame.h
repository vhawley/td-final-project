#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <algorithm>    // std::max/min
#include "GTDPlayer.h"
#include "GTDUnit.h"
#include "GTDMap.h"
#include "GTDTimer.h"

class GTDGame
{
public:
	GTDGame();
	bool init();
	void run();
	void quit();
	void moveScreen();
	void drawBoxSelection();
	void drawBuildBox();
	void drawBoxOnSelectedUnits();
	void buildPlayerBuilding();
	void performSelection();
private:
	bool loadTextures();
	int IsGameOver;
	int debug = 0;
	int mouseX;
	int mouseY;
	
	int screenX;
	int screenY;

	SDL_Window *window;
	SDL_Renderer *renderer;

	GTDMap map;
	GTDPlayer player;
};
