#include <iostream>
#include "GTDRect.h"

class GTDWaypointNode
{
public:
	GTDWaypointNode();
private:
	GTDRect rect;
	GTDWaypointNode *next;
};