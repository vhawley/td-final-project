//GTProjectile

#include <iostream>
#include <SDL.h>

class GTDProjectile{
	
	public:
		GTDPlayer();
		void spawn();	//creates new instance of projectile
		void move();	//moves projectile
		void die();	//ends the instance of the projectile
		bool hitTarget();	//returns true if the projectile hit the target
		
				
	private:
		bool hitTarget;		//true if the projectile hit the target
		int xcoord;		//x coordinate of location
		int ycoord;		//y coordinate of location
		int speed;		//speed that the projectile will move at
		int xdir;		//x direction of motion (1, 0 or -1)
		int ydir;		//y direction of motion (1, 0 or -1)
		

};
