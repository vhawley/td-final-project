#pragma once

#include <iostream>
#include <math.h>
#include <vector>
#include "GTDUnit.h"
#include "GTDWaypoint.h"

using namespace std;

class GTDLevel
{
public:
	GTDLevel();
	~GTDLevel();
private:
	std::vector<GTDUnit *> units;
	GTDWaypoint *waypoint; //waypoint for units to take
};
