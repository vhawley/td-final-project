#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>

#ifndef GTDPLAYER_H
#define GTDPLAYER_H

class GTDPlayer
{
public:
	GTDPlayer();
	void processInput();
	int getOver();
	bool isHoldingMouse();
	bool isBuilding();
	int getOldMouseX();
	int getOldMouseY();
private:
	SDL_Event e;
	const char *key;
	const char *lastkey;
	bool building;
	bool mouseHold;
	int oldMouseX;
	int oldMouseY;
	int over;
	int money;
};

#endif