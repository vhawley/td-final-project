//GTProjectile implementation

#include <iostream>
#include "GTDProjectile.h"

GTDProjectile::GTDProjectile(int x, int y, int xd, int yd)
{
	//default constructor
	
	hitTarget = false;
	xcoord = x;
	ycoord = y;
	xdir = xd;
	ydir = yd;
	
}
void GTDProjectile::spawn()
{
	//creates a new instance of a GTDProjectile
}

void GTDProjectile::move(int x, int y){
	//moves projectile in accordance to time
	while(!getHitTarget()){
		//if next move (location + next) is not off map
		//i.e. if getMapBoard(m, n) returns -1
		if(map->getMapBoard(xcoord + xdir, ycoord + ydir)){
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

bool GTDProjectile::getHitTarget(){
	//returns true if the projectile hit the target
	bool hit = false;
	//if next location == occupied, then hit = true;
	
	return hit;
}

int GTDProjectile::getXcoord(){
	return xcoord;
}
int GTDProjectile::getYcoord(){
	return ycoord;
}

