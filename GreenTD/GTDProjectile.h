#pragma once

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>
#include "GTDPlayer.h"
#include "GTDUnit.h"
using namespace std;

class GTDProjectile
{
	public:
		enum GTDProjectileType{ NORMAL, FIRE, ICE, LIGHTNING, EARTH };
		GTDProjectile(GTDProjectileType p, GTDPlayer *o, int x, int y, int d, GTDUnit *t, SDL_Renderer *renderer);
		GTDPlayer *getOwner();
		GTDUnit *getTarget();
		SDL_Texture *getTexture();

		double getPosX();
		double getPosY();
		double getFacingAngle();

		void step(int timeElapsed); //applies game logic to projectiles
		void die(); //kills itself when not needed
		void setOnMap(bool t);
		bool getIsOnMap();
		bool hasReachedTarget();
		bool loadProjTexture(string fn, SDL_Renderer *renderer);

		static int getMoveSpeed(GTDProjectileType p);
	private:
		GTDProjectileType projType;
		GTDPlayer *owner;
		GTDUnit *target;
		Mix_Chunk *impact;
		SDL_Texture *texture;
		bool isOnMap;
		double posX;
		double posY;
		double facingAngle;
		int movespeed;
		int damage;
};