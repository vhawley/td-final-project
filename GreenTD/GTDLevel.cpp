#include "GTDLevel.h"


GTDLevel::GTDLevel(char *filename, GTDRect *s, GTDWaypoint *way)
{
	spawn = s;
	//MAKE SURE TO CHANGE TO USE PARAMATER WAYPOINT
	waypoint = new GTDWaypoint(new GTDWaypointNode(new GTDRect(2000, 500, 50, 50)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(1800, 700, 50, 50)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(1600, 500, 50, 50)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(1800, 300, 50, 50)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(2000, 764, 50, 50)));
	if (!init(filename))
	{
		std::cout << "Error loading level from file" << std::endl;
	}
}


GTDLevel::~GTDLevel()
{

}

bool GTDLevel::init(char *filename)
{
	std::ifstream levelFile;
	levelFile.open(filename);
	if (!levelFile)
	{
		std::cout << "Could not open file: " << filename << std::endl;
		return false;
	}
	string a;
	string type;
	int num;
	while (levelFile >> a)
	{
		if (!isdigit(a.c_str()[0])) //if first letter is not a number, it is not a number
		{
			type = a;
		}
		else //if first letter is a number, assume it is a number
		{
			num = atoi(a.c_str());
		}
		if (levelFile.peek() == '\n')
		{
			std::cout << "TYPE: " << type << "\tNUMBER: " << num << std::endl;
			if (!strcmp(type.c_str(), "VILLAGER"))
			{
				unitMap[GTDUnit::GTDWaveUnit::VILLAGER] += num;
			}
			else if (!strcmp(type.c_str(), "SWORDSMAN"))
			{
				unitMap[GTDUnit::GTDWaveUnit::SWORDSMAN] += num;
			}
			else if (!strcmp(type.c_str(), "KNIGHT"))
			{
				unitMap[GTDUnit::GTDWaveUnit::KNIGHT] += num;
			}
			else if (!strcmp(type.c_str(), "KING"))
			{
				unitMap[GTDUnit::GTDWaveUnit::KING] += num;
			}
		}
	}
	return true;
}


int GTDLevel::getNumTypes(GTDUnit::GTDWaveUnit w)
{
	return unitMap[w];
}


GTDRect * GTDLevel::getSpawn()
{
	return spawn;
}

GTDWaypoint * GTDLevel::getWaypoint()
{
	return waypoint;
}