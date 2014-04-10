#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>

#ifndef GTDPLAYER_H
#define GTDPLAYER_H

class GTDPlayer
{
public:
	GTDPlayer();
	void processInput();
	int getOver();
	bool isHoldingMouse();
	void queueBuilding();
	bool hasBuildingQueued();
	int getCurrentlySelectedBuilding();
	void endQueueBuilding();
	bool isSelectingBuildLocation();
	int getOldMouseX();
	int getOldMouseY();
	int getMoney();
	void spend(int cost);
	void earn(int bounty);
private:
	SDL_Event e;
	const char *key;
	const char *lastkey;
	bool building;
	int currentlySelectedBuilding;
	bool buildingQueued;
	bool mouseHold;
	int oldMouseX;
	int oldMouseY;
	int over;
	int money;
	std::vector<int> selectedUnits;
};

#endif