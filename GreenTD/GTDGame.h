#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::max/min
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "GTDPlayer.h"
#include "GTDUnit.h"
#include "GTDMap.h"
#include "GTDTimer.h"

class GTDGame
{
public:
	GTDGame();
	~GTDGame();
	bool init();
	void run();
	void quit();
	void moveScreen(int time);
	void drawBoxSelection();
	void drawBuildBox();
	void buildPlayerBuilding();
	void performSelection();
	void drawUI();
private:
	bool loadTextures();
	int IsGameOver;
	int debug = 0;
	int mouseX;
	int mouseY;
	
	int screenX;
	int screenY;

	int UIheight;

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Color defColor;
	TTF_Font *font;

	GTDMap map;
	GTDPlayer player;
};
