#include "GTDRect.h"

GTDRect::GTDRect(int newX, int newY, int newW, int newH)
{
	setX(newX);
	setY(newY);
	setW(newW);
	setH(newH);
}

void GTDRect::setX(int a)
{
	if (a < 0)
	{
		x = 0;
	}
	else
	{
		x = a;
	}
}
void GTDRect::setY(int a)
{
	if (a < 0)
	{
		y = 0;
	}
	else
	{
		y = a;
	}
}
void GTDRect::setW(int a)
{
	if (a < 0)
	{
		w = 0;
	}
	else
	{
		w = a;
	}
}
void GTDRect::setH(int a)
{
	if (a < 0)
	{
		h = 0;
	}
	else
	{
		h = a;
	}
}

int GTDRect::getX()
{
	return x;
}
int GTDRect::getY()
{
	return y;
}
int GTDRect::getW()
{
	return w;
}
int GTDRect::getH()
{
	return h;
}