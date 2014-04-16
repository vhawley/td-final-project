#pragma once

#include <iostream>
#include "GTDRect.h"

class GTDWaypointNode
{
public:
	GTDWaypointNode(GTDRect *r);
	GTDWaypointNode *next;
	GTDRect *rect;
private:
};