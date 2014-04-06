//GTProjectile implementation

#include <iostream>
#include <SDL.h>
#include "GTDProjectile.h"
#include "GTDMap.h"

GTDProjectile::GTDProjectile(int x, int y, int xd, int yd){
	//default constructor
	
	hitTarget = false;
	xcoord = x;
	ycoord = y;
	xdir = xd;
	ydir = yd;
	
}
void GTDProjectile::spawn{
	
	//creates a new instance of a GTDProjectile
}

void GTDProjectile::move(int x, int y){
	//moves projectile in accordance to time
	while(!hitTarget()){
		//if next move (location + next) is not off map
		//i.e. if getMapBoard(m, n) returns -1
		if(getMapBoard(xcoord + xdir, ycood + ydir)){
			//if true, it can move forward
			xcoord = xcoord + xdir;
			ycoord = ycoord + ydir;
		}
		//move one unit		
		
	}

}

void GTDProjectile::die(){
	//ends the instance of the projectile
	
	//if hitTarget == true
	
	//delete
}

bool GTDProjectile::hitTarget(){
	//returns true if the projectile hit the target
	bool hit;
	//if next location == occupied, then hit = true;
	
	return hit;
}

int getXcoord(){
	return xcoord;
}
int getYcoord(){
	return ycoord;
}

