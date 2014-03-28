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
	GTDMap(char *filename, SDL_Renderer *renderer);
	bool init(char *filename, SDL_Renderer *renderer);
	int getMapBoard(int m, int n);
	void setMapBoard(int m, int n, unsigned int value);
	void printMapBoard();
	bool loadTextures(SDL_Renderer *renderer);
	void draw(int x, int y, SDL_Renderer *renderer);
	int getMapW();
	int getMapH();
	int getTileW();
	int getTileH();
	bool rectContainsUnit(GTDRect rect, GTDUnit unit);
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
	std::vector<GTDUnit> units;
	std::vector<GTDPlayer> players;
	std::vector<GTDRect> rects;

};

#endif