#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>    // std::max/min
#ifdef TARGET_OS_MAC
#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#elif _WIN64
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#elif _WIN32
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif
#include "GTDPlayer.h"
#include "GTDUnit.h"
#include "GTDMap.h"
#include "GTDTimer.h"

class GTDGame
{
public:
	enum GTDGameState { PREGAME, WAVEINPROGRESS, WAVECOMPLETE, GAMELOST, GAMEWON };
	GTDGame();
	~GTDGame();
	bool init();
	void run();
	void quit();
	void moveScreen(int time);
	void drawBoxSelection(); //when player is box selecting
	void drawBuildBox(); //blue box when player is building
	void buildPlayerBuilding(); //perform build on map
	void performSelection(); //when player lets go of box select, select units in rect
	void drawUI(); //draws text on bottom of screen
	void updateGameState(int time); //checks game conditions at each tick
	void updateStatusMessage(); //updates utility text at bottom right depending on situtaiton
	void queueAttacks(); //if player right clicks with buildings he owns selected, issue attack
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

	string statusMessage;
	string statusAux;

	GTDGameState currentState;
	GTDMap map;
	GTDPlayer player;
	double timeTilSpawn;
	int currentLevel;
	int totalLevels;
	int numRemaining;
};
