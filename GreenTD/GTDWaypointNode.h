#pragma once

#include <iostream>
#include "GTDRect.h"

class GTDWaypointNode
{
public:
	GTDWaypointNode(GTDRect *r); //create node with rect pointer r
	GTDWaypointNode *next;
	GTDRect *rect;
private:
};