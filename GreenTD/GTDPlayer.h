#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <SDL.h>

using namespace std;

class GTDPlayer
{
public:
	GTDPlayer();
	void processInput();
	int getOver();


	bool isHolding(string key);

	void queueSelection();
	bool isSelecting();
	void endQueueSelection();

	void queueBuilding();
	bool hasBuildingQueued();
	int getCurrentlySelectedBuilding();
	bool hasAttackQueued();
	void setAttackQueued(bool t);
	void endQueueBuilding();
	bool isSelectingBuildLocation();
	int getOldMouseX();
	int getOldMouseY();
	int getMoney();
	int getKills();
	void spend(int cost);
	void earn(int bounty);
	void addKill();

private:
	SDL_Event e;
	const char *key;
	const char *lastkey;
	bool building;
	int currentlySelectedBuilding;
	bool buildingQueued;
	bool selecting;
	bool attackQueued;

	map<string, bool> keyDown;

	int oldMouseX;
	int oldMouseY;
	int over;
	int money;
	int kills;
	vector<int> selectedUnits;
};