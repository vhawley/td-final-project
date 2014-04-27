#pragma once

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include "GTDUnit.h"
#include "GTDWaypoint.h"

using namespace std;

class GTDLevel
{
public:
	GTDLevel(char *filename, GTDRect *s);
	~GTDLevel();
	
	bool init(char *filename);
	int getNumTypes(GTDUnit::GTDWaveUnit w);

	GTDRect * getSpawn();
	GTDWaypoint * getWaypoint();
private:
	std::map<GTDUnit::GTDWaveUnit, int> unitMap;
	GTDRect *spawn;
	GTDWaypoint *waypoint; //waypoint for units to take
};
