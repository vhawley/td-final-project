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


	bool isHolding(string key); //checks if player is holding key using stl map

	void queueSelection(); //sets selecting flag to true for box selection
	bool isSelecting(); //checks whether selecting flag is true
	void endQueueSelection(); //sets selecting flag to false when box selection ends

	void queueBuilding(); //sets buildingQueued to true when building and mouse clicked
	bool hasBuildingQueued(); //checks buildingQueued
	void endQueueBuilding();
	int getCurrentlySelectedBuilding(); //returns currentlySelectedbuidling
	bool hasAttackQueued(); //returns attackQueued (right click attack)
	void setAttackQueued(bool t); //sets attackQueued (used by game when attack queue has processed)
	
	bool isSelectingBuildLocation(); //when player pressed b to build and conditions are met, returns true
	int getOldMouseX(); //mouse origin when selection begins
	int getOldMouseY(); //mouse origin when selection begins
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