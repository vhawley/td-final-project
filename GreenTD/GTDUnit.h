#pragma once

#include <iostream>
#include <algorithm>
#include <math.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "GTDPlayer.h"
#include "GTDRect.h"
#include "GTDWaypoint.h"

using namespace std;

class GTDUnit
{
public:
	enum GTDUnitType{ BUILDING, WAVEUNIT };
	enum GTDBuilding{ NORMAL, FIRE, ICE, LIGHTNING, EARTH, SPEEDASSIST, DMGASSIST }; //Count last for getting number of building types
	enum GTDWaveUnit{ VILLAGER, SWORDSMAN, KNIGHT, KING }; //Count for getting number of types

	GTDUnit(); //should not use
	GTDUnit(enum GTDBuilding b, GTDPlayer *own, double x, double y, SDL_Renderer *rend); //Building constructor
	GTDUnit(enum GTDWaveUnit w, double x, double y, SDL_Renderer *rend, GTDWaypoint way); //WaveUnit constructor
	double getPosX();
	double getPosY();
	int getCollision();

	SDL_Texture * getTexture();
	GTDPlayer * getOwner();
	string getName();
	GTDBuilding getBuildingType();
	GTDWaveUnit getWaveUnitType();

	void step(int timeElapsed); //applies game logic to unit 

	void issueMoveToPoint(int x, int y); //creates rect for unit and sets as current dest
	void issueMoveToRect(GTDRect *rect); //sets rect as dest
	bool isWithinDistanceOfUnit(double d, GTDUnit *u); //checks distance between unit and u

	//getters/setters
	bool isOnMap();
	bool isBuilding();
	bool isWaveUnit();
	GTDRect getCurrentDest();
	GTDUnit *getTarget();
	bool hasQueuedProjectile();
	void setTarget(GTDUnit *u);
	void setQueuedProjectile(bool t);
	bool hasTarget();
	bool isDead();

	void setOnMap(bool on);
	void select();
	void unselect();
	bool isSelected();
	bool didReachEnd();
	double getFacingAngle();

	void setAttackCooldown(int c);
	void setAttackDMG(int d);
	void setMoveSpeed(int s);
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

	void setHealth(int h);


	//static functions for building/waveunit attributes
	static string getName(enum GTDBuilding b);
	static int getCollision(enum GTDBuilding b);
	static int getCost(enum GTDBuilding b);
	static int getAttackDMG(enum GTDBuilding b);
	static int getAttackDMGRange(enum GTDBuilding b);
	static int getAttackRange(enum GTDBuilding b);
	static int getAttackCooldown(enum GTDBuilding b);

	static string getName(enum GTDWaveUnit w);
	static int getMaxHealth(enum GTDWaveUnit w);
	static int getBaseArmor(enum GTDWaveUnit w);
	static int getMoveSpeed(enum GTDWaveUnit w);
	static int getBounty(enum GTDWaveUnit w);
	static int getBountyRange(enum GTDWaveUnit w);

	~GTDUnit();
private:
	void setPosX(double x);
	void setPosY(double y);

	bool atDestination(); //Used for waypoints. Use similar map function for other purposes.

	bool loadUnitTexture(string fn, SDL_Renderer *renderer);

	void attackTarget();

	double posX;
	double posY;
	int collision; //width and height for collision
	int cost;

	SDL_Texture *text;
	SDL_Renderer *renderer;
	Mix_Chunk *attackLaunch;
	string name;
	GTDPlayer *owner; //Units must have an owner
	bool queuedProjectile;
	enum GTDUnitType unitType;
	enum GTDBuilding bType;
	enum GTDWaveUnit wType;
	GTDWaypoint waypoint;
	GTDRect *currentDest;
	GTDUnit *target;
	bool reachedEnd;
	bool onMap;

	double facingAngle;
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
	int attackCooldown;
	int atkCooldownTimer;
	
};