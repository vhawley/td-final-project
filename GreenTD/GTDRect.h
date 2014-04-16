#pragma once

#include <iostream>
#include <string>
#include <SDL.h>

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
