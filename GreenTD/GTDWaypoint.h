#pragma once

#include <iostream>
#include <math.h>
#include "GTDWaypointNode.h"

using namespace std;

class GTDWaypoint
{
public:
	GTDWaypoint();
	GTDWaypoint(GTDWaypointNode *head); //create waypoint with an existing head node
	void addNode(GTDWaypointNode *node);
	void advance();
	GTDWaypointNode *first;
private:

};