#include "GTDWaypoint.h"

GTDWaypoint::GTDWaypoint()
{

}

GTDWaypoint::GTDWaypoint(GTDWaypointNode *head)
{
	first = head;
}

void GTDWaypoint::addNode(GTDWaypointNode *node) //adds node to end of waypoint
{
	GTDWaypointNode *temp = first;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = node;
}

void GTDWaypoint::advance()
{
	first = first->next;
}

