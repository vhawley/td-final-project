#include <iostream>
#include <string.h>
#include <SDL.h>
#include "GTDPlayer.h"

using namespace std;

class GTDUnit
{
public:
	GTDUnit();
private:
	int posX;
	int posY;
	SDL_Texture *text;
	string name;
	GTDPlayer *owner;
	int health;
	int armor;
	bool invuln;
};