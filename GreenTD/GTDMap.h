#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <SDL.h>
#include "GTDRect.h"
#include "GTDUnit.h"
#include "GTDProjectile.h"
#include "GTDPlayer.h"
#include "GTDWaypoint.h"
#include "GTDLevel.h"

class GTDMap
{
public:
	GTDMap();
	GTDMap(char *filename, SDL_Renderer *rend); //Runs init passing through parameters.
	bool init(char *filename, SDL_Renderer *rend); //Init map with file that contains 2D array of ints.
	int getMapBoard(int m, int n); //Get value at location on 2D array
	void setMapBoard(int m, int n, unsigned int value); //Set map board value
	void printMapBoard(); //prints 2D int array to console

	bool loadTextures(); //Loads textures into textures vector
	void spawnLevel(GTDLevel *level);

	void draw(int x, int y, SDL_Renderer *renderer); //Draws map and associated elements (units, projs, etc).

	void addUnit(GTDUnit *u); //adds unit at pointer to units vector
	void stepUnits(int timeElapsed); //applies game logic at each tick

	void addProj(GTDProjectile *p);

	void removeUnitsNotOnMap(); //removes units with isOnMap set to false
	int getNumWaveUnitsOnMap(); //counts waveunits with isOnMap set to true
	void selectUnitsInRect(GTDRect *rect); //sets selected flag for units in rect to true, others to false
	void issueAttackOrder(GTDPlayer *p, GTDRect *rect); //Order selected units owned by player p to attack random unit in rect

	GTDUnit * getSelectedUnit();

	int getMapW();
	int getMapH();
	int getTileW();
	int getTileH();
	int getLives();
	bool spaceIsBuildable(int m, int n);
	static bool rectContainsUnit(GTDRect rect, GTDUnit unit); //If rect contains unit, return yes
	~GTDMap();
private:
	int initialized;
	int **mapBoard;
	int a;
	int mapW;
	int mapH;
	int tileW;
	int tileH;
	int lives;
	SDL_Renderer *renderer;
	std::vector<SDL_Texture *> textures;
	std::vector<GTDUnit> units; //Units will be displayed on map but have ownership through their own "owner" pointer. *map should not be handling players*
	std::vector<GTDProjectile> projectiles;
	std::vector<GTDRect> rects;

};
