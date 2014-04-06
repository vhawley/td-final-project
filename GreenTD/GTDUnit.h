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
	GTDUnit(string n, int x, int y, int col, string texture, GTDPlayer *own, int h, int a, bool inv, int bount, int bountRng, int atkDMG, int atkDMGRng);
	int getPosX();
	int getPosY();
private:
	int posX;
	int posY;
	int collision; //width and height for collision
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