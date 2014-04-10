#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <SDL.h>

#ifndef GTDPLAYER_H
#define GTDPLAYER_H

using namespace std;

class GTDPlayer
{
public:
	GTDPlayer();
	void processInput();
	int getOver();

	bool isHolding(string key);
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

	map<string, bool> keyDown;

	int oldMouseX;
	int oldMouseY;
	int over;
	int money;
	vector<int> selectedUnits;
};

#endif