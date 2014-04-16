#pragma once

#include <iostream>
#include <math.h>
#include <string.h>
#include <SDL.h>
#include "GTDPlayer.h"
#include "GTDRect.h"
#include "GTDWaypoint.h"

using namespace std;

class GTDUnit
{
public:
	enum GTDUnitType{ BUILDING, WAVEUNIT };
	enum GTDBuilding{ NORMAL, FIRE, ICE, LIGHTNING, EARTH, SPEEDASSIST, DMGASSIST };
	enum GTDWaveUnit{ VILLAGER, SWORDSMAN, KNIGHT, KING };

	GTDUnit();
	GTDUnit(enum GTDBuilding b, GTDPlayer *own, double x, double y, SDL_Renderer *rend); //Building
	GTDUnit(enum GTDWaveUnit w, double x, double y, SDL_Renderer *rend, GTDWaypoint *way); //WaveUnit
	double getPosX();
	double getPosY();
	int getCollision();

	SDL_Texture * getTexture();
	GTDPlayer * getOwner();

	void step(int timeElapsed);

	void issueMoveToPoint(int x, int y);
	void issueMoveToRect(GTDRect *rect);

	bool isBuilding();
	bool isWaveUnit();
	GTDRect getCurrentDest();

	bool isSelected();
	int getMaxHealth();
	int getHealth();
	int getArmor();
	bool getInvuln();
	int getMovespeed();
	int getBounty();
	int getBountyrange();
	int getAttackDMG();
	int getAttackDMGRange();
	int getAttackRange();

	static int getCollision(enum GTDBuilding b);
	static int getCost(enum GTDBuilding b);

private:
	void setPosX(double x);
	void setPosY(double y);

	bool atDestination(); //Used for waypoints. Use similar map function for other purposes.

	bool loadUnitTexture(string fn, SDL_Renderer *renderer);

	double posX;
	double posY;
	int collision; //width and height for collision
	int cost;

	SDL_Texture *text;
	SDL_Renderer *renderer;
	string name;
	GTDPlayer *owner; //Units must have an owner
	enum GTDUnitType unitType;
	GTDWaypoint *waypoint;
	GTDRect *currentDest;

	bool selected;
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