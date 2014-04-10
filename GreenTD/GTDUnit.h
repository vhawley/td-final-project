#include <iostream>
#include <string.h>
#include <SDL.h>
#include "GTDPlayer.h"
#include "GTDRect.h"
#include "GTDWaypoint.h"

#ifndef GTDUNIT_H
#define GTDUNIT_H

using namespace std;

class GTDUnit
{
public:
	enum GTDUnitType{ BUILDING, WAVEUNIT };
	enum GTDBuilding{ NORMAL, FIRE, ICE, LIGHTNING, EARTH, SPEEDASSIST, DMGASSIST };
	enum GTDWaveUnit{ VILLAGER, SWORDSMAN, KNIGHT, KING };

	GTDUnit();
	GTDUnit(enum GTDBuilding b, GTDPlayer *own, double x, double y, SDL_Renderer *renderer); //Building
	GTDUnit(enum GTDWaveUnit w, double x, double y, SDL_Renderer *renderer); //WaveUnit
	double getPosX();
	double getPosY();
	int getCollision();

	SDL_Texture * getTexture();
	GTDPlayer * getOwner();

	void step(int timeElapsed);

	void issueMoveToPoint(int x, int y);
	void issueMoveToRect(GTDRect rect);

	bool isBuilding();
	bool isWaveUnit();

	static int getCollision(enum GTDBuilding b);
	static int getCost(enum GTDBuilding b);

private:
	void setPosX(double x);
	void setPosY(double y);
	bool loadUnitTexture(string fn, SDL_Renderer *renderer);

	double posX;
	double posY;
	int collision; //width and height for collision
	SDL_Texture *text;
	string name;
	GTDPlayer *owner; //Units must have an owner
	enum GTDUnitType unitType;
	GTDWaypoint waypoint;
	int cost;
	int maxhealth;
	int health;
	int armor;
	bool invuln;
	int movespeed;
	int bounty;
	int bountyrange;
	int attackDMG;
	int attackDMGRange;
	int attackRange;
	
};

#endif