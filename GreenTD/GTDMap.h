#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <SDL.h>
#include "GTDRect.h"
#include "GTDUnit.h"

#ifndef GTDMAP_H
#define GTDMAP_H

class GTDMap
{
public:
	GTDMap();
	GTDMap(char *filename, SDL_Renderer *renderer); //Runs init passing through parameters.
	bool init(char *filename, SDL_Renderer *renderer); //Init map with file that contains 2D array of ints.
	int getMapBoard(int m, int n); //Get value at location on 2D array
	void setMapBoard(int m, int n, unsigned int value); //Set map board value
	void printMapBoard(); //prints 2D int array to console
	bool loadTextures(SDL_Renderer *renderer); //Loads textures into textures vector
	void draw(int x, int y, SDL_Renderer *renderer); //Draws base map.
	int getMapW();
	int getMapH();
	int getTileW();
	int getTileH();
	bool rectContainsUnit(GTDRect rect, GTDUnit unit); //If rect contains unit, return yes
	~GTDMap();
private:
	int initialized;
	int **mapBoard;
	int a;
	int mapW;
	int mapH;
	int tileW;
	int tileH;
	std::vector<SDL_Texture *> textures;
	std::vector<GTDUnit> units; //Units will be displayed on map but have ownership through their own "owner" pointer.
	std::vector<GTDPlayer> players;
	std::vector<GTDRect> rects;

};

#endif