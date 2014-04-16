#pragma once

#include <iostream>
#include <SDL.h>
#include "GTDMap.h"


using namespace std;

class GTDProjectile
{
	public:
		GTDProjectile(int x, int y, int xd, int yd);
		void spawn();	//creates new instance of projectile
		void move(int x, int y);	//moves projectile
		void die();	//ends the instance of the projectile
		bool getHitTarget();	//returns true if the projectile hit the target	
		int getXcoord(); //get x coord
		int getYcoord(); //get y coord
	private:
		bool hitTarget;		//true if the projectile hit the target
		int xcoord;		//x coordinate of location
		int ycoord;		//y coordinate of location
		int speed;		//speed that the projectile will move at
		int xdir;		//x direction of motion (1, 0 or -1)
		int ydir;		//y direction of motion (1, 0 or -1)
		GTDMap *map; //map reference
};