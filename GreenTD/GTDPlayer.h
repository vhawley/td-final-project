#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>


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
};