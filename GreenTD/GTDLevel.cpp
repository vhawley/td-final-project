#include "GTDLevel.h"


GTDLevel::GTDLevel(char *filename, GTDRect *s)
{
	//set spawn region
	spawn = s;
	
	//set units waypoint
	waypoint = new GTDWaypoint(new GTDWaypointNode(new GTDRect(212, 305, 20, 20)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(220, 768, 20, 20)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(484, 750, 20, 30)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(460, 490, 45, 20)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(900, 493, 20, 38)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(882, 202, 34, 24)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(1286, 200, 22, 38)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(1260, 1220, 40, 20)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(970, 1208, 20, 28)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(850, 1064, 30, 30)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(850, 930, 40, 20)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(1574, 946, 22, 38)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(1553, 1188, 40, 20)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(1766, 1166, 22, 34)));
	waypoint->addNode(new GTDWaypointNode(new GTDRect(1746, 192, 34, 28)));
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

	//read file to enter units into stl map
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