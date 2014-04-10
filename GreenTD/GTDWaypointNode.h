#include <iostream>
#include "GTDRect.h"

#ifndef GTDWAYPOINTNODE_H
#define GTDWAYPOINTNODE_H

class GTDWaypointNode
{
public:
	GTDWaypointNode();
	GTDWaypointNode(GTDRect *r);
private:
	GTDRect *rect;
	GTDWaypointNode *next;
};

#endif