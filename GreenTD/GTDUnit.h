#include <iostream>
#include <string.h>
#include <SDL.h>
#include "GTDPlayer.h"

#ifndef GTDUNIT_H
#define GTDUNIT_H

using namespace std;

class GTDUnit
{
public:
	GTDUnit();
	int getPosX();
	int getPosY();
private:
	int posX;
	int posY;
	SDL_Texture *text;
	string name;
	GTDPlayer *owner; //Units must have an owner
	int health;
	int armor;
	bool invuln;
	int bounty;
	int bountyrange;
	int attackDMG;
	int attackDMGRange;
};

#endif