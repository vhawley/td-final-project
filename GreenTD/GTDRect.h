#pragma once

#include <iostream>
#include <string>
#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif _WIN64
#include <SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

class GTDRect
{
public:
	GTDRect();
	GTDRect(int newX, int newY, int newW, int newH);
	void setX(int a);
	void setY(int a);
	void setW(int a);
	void setH(int a);

	int getX();
	int getY();
	int getW();
	int getH();

private:
	int x; //top left corner x
	int y; //top left corner y
	int w; //width
	int h; //height
};
