#include <iostream>
#include <math.h>
#include "GTDWaypointNode.h"

#ifndef GTDWAYPOINT_H
#define GTDWAYPOINT_H

using namespace std;

class GTDWaypoint
{
public:
	GTDWaypoint();
private:
	GTDWaypointNode *first;
};

#endif