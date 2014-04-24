#include "GTDMap.h"

GTDMap::GTDMap()
{
	initialized = 0;
	lives = 50;
}

GTDMap::GTDMap(char *filename, SDL_Renderer *rend)
{	
	initialized = 0;
	init(filename, rend);
}

bool GTDMap::init(char *filename, SDL_Renderer *rend)
{
	renderer = rend;
	if (!initialized)
	{
		std::ifstream mapFile;
		mapFile.open(filename);
		if (!mapFile)
		{
			std::cout << "Could not open file: " << filename << std::endl;
			return 0;
		}
		int wCounter = 0;
		bool haveWidth = false;
		int hCounter = 0;

		//Initial read to count lines
		while (mapFile >> a)
		{
			if (!haveWidth)
			{
				wCounter++;
			}
			if (mapFile.peek() == '\n')
			{
				haveWidth = true;
				hCounter++;
			}
		}
		std::cout << "Width / height = " << wCounter << " / " << hCounter << std::endl;
		
		mapBoard = new int *[hCounter];

		//2d array based on counts from last read
		for (int i = 0; i < hCounter; i++)
		{
			mapBoard[i] = new int[wCounter];
		}
		mapFile.close();
		mapFile.open(filename);

		//Actually load in contents to int array
		for (int i = 0; i < hCounter; i++)
		{
			for (int j = 0; j < wCounter; j++)
			{
				mapFile >> a;
				mapBoard[i][j] = a;
			}
		}
		mapW = wCounter;
		mapH = hCounter;
		tileW = 32;
		tileH = 32;
		if (!loadTextures())
		{
			return 0;
		}
		initialized = 1;
		return 1;
	}
	else
	{
		std::cout << "Map has already been initialized" << std::endl;
	}
	return 0;
}


int GTDMap::getMapBoard(int m, int n)
{
	if (m >= mapH || n >= mapW)
	{
		return -1;
	}
	else
	{
		return mapBoard[m][n];
	}
}
void GTDMap::setMapBoard(int m, int n, unsigned int value)
{
	if (m > mapH || n > mapW)
	{
		return;
	}
	else
	{
		mapBoard[m][n] = value;
	}
}

void GTDMap::printMapBoard()
{
	if (!initialized)
	{
		std::cout << "Map has not been initialized" << std::endl;
	}
	else
	{
		for (int i = 0; i < mapH; i++)
		{
			for (int j = 0; j < mapW; j++)
			{
				std::cout << mapBoard[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
}

bool GTDMap::loadTextures()
{
	for (int i = 1; i < 355; i++)
	{
		char filepath[50];
		//Create filepath from i
		sprintf_s(filepath, "./assets/terrain/%d.bmp", i);
		//Load BMP from filepath
		SDL_Surface *BMP = SDL_LoadBMP(filepath);
		//Add BMP to textures vector
		textures.push_back(SDL_CreateTextureFromSurface(renderer, BMP));

		SDL_FreeSurface(BMP);
	}

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		if (!textures.at(i))
		{
			return false;
		}
	}
	return true;
}

void GTDMap::spawnLevel(GTDLevel *level)
{
	for (unsigned int i = 0; i < 5; i++)
	{
		GTDUnit::GTDWaveUnit wtype = static_cast<GTDUnit::GTDWaveUnit>(i);
		for (unsigned int j = 0; j < level->getNumTypes(wtype); j++)
		{
			int spawnX = level->getSpawn()->getX() + (rand() % level->getSpawn()->getW());
			int spawnY = level->getSpawn()->getY() + (rand() % level->getSpawn()->getH());
			addUnit(new GTDUnit(wtype, spawnX, spawnY, renderer, *(level->getWaypoint())));
		}
	}
}

void GTDMap::draw(int x, int y, SDL_Renderer *renderer)
{
	SDL_Rect tileRect;
	tileRect.x = 0;
	tileRect.y = 0;
	tileRect.w = tileW;
	tileRect.h = tileH;

	int tileX = x / tileRect.w;
	int tileY = y / tileRect.h;

	int tileDeltaX = x % tileRect.w;
	int tileDeltaY = y % tileRect.h;

	int textcount = 0;
	tileRect.y = -tileDeltaY;

	//Loops through specific position in 2D array to draw only the textures needed
	for (int i = std::max(tileY, 0); i < std::min(tileY + 24, getMapH()); i++) //Hardcoded tile length for now...
	{
		tileRect.x = -tileDeltaX;
		for (int j = std::max(tileX, 0); j < std::min(tileX + 41, getMapW()); j++) //Hardcoded tile length for now... should come up with better way
		{
			//Render texture based on index in array
			int textureI = getMapBoard(i, j) - 1;
			if (textureI < 1)
			{
				textureI = 1;
			}
			else if (textureI > 355)
			{
				textureI = 355;
			}
			SDL_RenderCopy(renderer, textures.at(textureI), NULL, &tileRect);
			tileRect.x += tileRect.w;
			textcount++;
		}
		tileRect.y += tileRect.h;
	}

	//Draw units on map
	for (unsigned int uindex = 0; uindex < units.size(); uindex++)
	{
		if (units.at(uindex).isOnMap())
		{
			GTDRect screenRect;
			screenRect.setX(x - units.at(uindex).getCollision() / 2);
			screenRect.setY(y - units.at(uindex).getCollision() / 2);
			screenRect.setW(x + 1280 + units.at(uindex).getCollision() / 2);
			screenRect.setH(y + 720 + units.at(uindex).getCollision() / 2);

			if (rectContainsUnit(screenRect, units.at(uindex))) //only draw if in screen bounds for performance
			{
				SDL_Rect unitRect;
				unitRect.x = units.at(uindex).getPosX() - x - units.at(uindex).getCollision() / 2;
				unitRect.y = units.at(uindex).getPosY() - y - units.at(uindex).getCollision() / 2;
				unitRect.w = units.at(uindex).getCollision();
				unitRect.h = units.at(uindex).getCollision();
				SDL_RenderCopyEx(renderer, units.at(uindex).getTexture(), NULL, &unitRect, (units.at(uindex).getFacingAngle() / (2 * M_PI)) * 360, NULL, SDL_FLIP_NONE); //converting radians to degrees. goddamnit sdl why you takin degrees

				if (units.at(uindex).isWaveUnit())
				{
					double healthPct = (double)units.at(uindex).getHealth() / units.at(uindex).getMaxHealth();
					SDL_Rect healthRect;
					healthRect.x = unitRect.x - 4;
					healthRect.y = unitRect.y - 10;
					healthRect.w = unitRect.w + 8; //Red = remaining
					healthRect.h = 6;
					SDL_SetRenderDrawColor(renderer, 255, 50, 50, 1);
					SDL_RenderFillRect(renderer, &healthRect);

					healthRect.w *= healthPct; //Green = remaining health
					SDL_SetRenderDrawColor(renderer, 50, 255, 50, 1);
					SDL_RenderFillRect(renderer, &healthRect);
				}

				if (units.at(uindex).isSelected())
				{
					SDL_Rect boxRect;
					boxRect.x = unitRect.x - 1;
					boxRect.y = unitRect.y - 1;
					boxRect.w = unitRect.w + 2;
					boxRect.h = unitRect.h + 2;
					if (units.at(uindex).isBuilding())
					{
						SDL_SetRenderDrawColor(renderer, 50, 50, 255, 1);
					}
					else
					{
						SDL_SetRenderDrawColor(renderer, 255, 50, 50, 1);
					}
					SDL_RenderDrawRect(renderer, &unitRect);
					SDL_RenderDrawRect(renderer, &boxRect); //Draw 2 boxes to make it easier to see....
					if (units.at(uindex).isBuilding())
					{
						int circX = units.at(uindex).getPosX() - x;
						int circY = units.at(uindex).getPosY() - y;
						int r = units.at(uindex).getAttackRange();
						double dt = .01;
						SDL_SetRenderDrawColor(renderer, 50, 255, 50, 1);
						for (double theta = 0; theta < 2 * M_PI; theta += dt)
						{
							int tX = circX + r*cos(theta);
							int tY = circY + r*sin(theta);
							int tXD = circX + r*cos(theta + dt);
							int tYD = circY + r*sin(theta + dt);
							SDL_RenderDrawLine(renderer, tX, tY, tXD, tYD);
						}
					}
				}

				/*
				SDL_Rect destRect;
				destRect.x = units.at(uindex).getCurrentDest().getX() - x;
				destRect.y= units.at(uindex).getCurrentDest().getY() - y;
				destRect.w = units.at(uindex).getCurrentDest().getW();
				destRect.h = units.at(uindex).getCurrentDest().getH();
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
				SDL_RenderDrawRect(renderer, &destRect);
				*/

				//for testing waypoint

			}
		}
	}
	for (unsigned int pindex = 0; pindex < projectiles.size(); pindex++)
	{
		if (projectiles.at(pindex).getIsOnMap())
		{
			SDL_Rect projRect;
			projRect.x = projectiles.at(pindex).getPosX() - x - 8; //all projectiles have 16 width/height... maybe change later
			projRect.y = projectiles.at(pindex).getPosY() - y - 8;
			projRect.w = 16;
			projRect.h = 16;
			SDL_RenderCopyEx(renderer, projectiles.at(pindex).getTexture(), NULL, &projRect, (projectiles.at(pindex).getFacingAngle() / (2 * M_PI)) * 360, NULL, SDL_FLIP_NONE);
		}
	}
}

void GTDMap::addUnit(GTDUnit *u)
{
	u->setOnMap(true);
	units.push_back(*u);
}

void GTDMap::stepUnits(int timeElapsed)
{
	for (unsigned int j = 0; j < projectiles.size(); j++)
	{
		projectiles.at(j).step(timeElapsed);
	}
	for (unsigned int i = 0; i < units.size(); i++)
	{
		if (units.at(i).isBuilding())
		{
			
			if (units.at(i).getBuildingType() == GTDUnit::GTDBuilding::SPEEDASSIST)
			{
				for (unsigned int j = 0; j < units.size(); j++)
				{
					if (units.at(j).isBuilding())
					{
						if (units.at(i).isWithinDistanceOfUnit(units.at(i).getAttackRange(), &units.at(j)))
						{
							units.at(j).setAttackCooldown(GTDUnit::getAttackCooldown(units.at(j).getBuildingType()) * 0.75);
						}
					}
				}
			}
			if (units.at(i).getBuildingType() == GTDUnit::GTDBuilding::DMGASSIST)
			{
				for (unsigned int j = 0; j < units.size(); j++)
				{
					if (units.at(j).isBuilding())
					{
						if (units.at(i).isWithinDistanceOfUnit(units.at(i).getAttackRange(), &units.at(j)))
						{
							units.at(j).setAttackDMG(GTDUnit::getAttackDMG(units.at(j).getBuildingType()) * 1.5);
						}
					}
				}
			}
			if (!units.at(i).hasTarget())
			{
				for (unsigned int j = 0; j < units.size(); j++)
				{
					if (units.at(j).isWaveUnit())
					{
						if (units.at(i).isWithinDistanceOfUnit(units.at(i).getAttackRange(), &units.at(j)) && !units.at(j).getInvuln())
						{
							units.at(i).setTarget(&units.at(j));
						}
					}
				}
			}
		}
		if (units.at(i).isOnMap())
		{
			units.at(i).step(timeElapsed);
		}
		if (units.at(i).isBuilding())
		{
			if (units.at(i).hasQueuedProjectile())
			{
				GTDProjectile::GTDProjectileType pType;
				switch (units.at(i).getBuildingType())
				{
				case GTDUnit::GTDBuilding::NORMAL:
					pType = GTDProjectile::GTDProjectileType::NORMAL;
					break;
				case GTDUnit::GTDBuilding::FIRE:
					pType = GTDProjectile::GTDProjectileType::FIRE;
					break;
				case GTDUnit::GTDBuilding::ICE:
					pType = GTDProjectile::GTDProjectileType::ICE;
					break;
				case GTDUnit::GTDBuilding::LIGHTNING:
					pType = GTDProjectile::GTDProjectileType::LIGHTNING;
					break;
				case GTDUnit::GTDBuilding::EARTH:
					pType = GTDProjectile::GTDProjectileType::EARTH;
					break;
				default:
					break;
				}
				int damage = units.at(i).getAttackDMG();
				if (units.at(i).getAttackDMGRange() != 0)
				{
					damage += rand() % units.at(i).getAttackDMGRange() - units.at(i).getAttackDMGRange() / 2;
				}
				addProj(new GTDProjectile(pType, units.at(i).getOwner(), units.at(i).getPosX(), units.at(i).getPosY(), damage, units.at(i).getTarget(), renderer));
				units.at(i).setQueuedProjectile(false);
			}
		}
		if (units.at(i).isDead())
		{
			units.at(i).setOnMap(false);
		}
		else if(units.at(i).didReachEnd() && units.at(i).isOnMap())
		{
			lives--;
			units.at(i).setOnMap(false);
			if (lives <= 0)
			{
				//gg
			}
		}
	}
}

void GTDMap::addProj(GTDProjectile *p)
{
	p->setOnMap(true);
	projectiles.push_back(*p);
}

void GTDMap::removeUnitsNotOnMap() //only to use at end of level to save memory
{
	for (unsigned int i = 0; i < units.size(); i++)
	{
		if (units.at(i).isBuilding()) //set building targets to null to prevent them attacking nonexistant units
		{
			units.at(i).setTarget(NULL);
		}
		if (!units.at(i).isOnMap())
		{
			units.erase(units.begin() + i);
		}
	}
	for (unsigned int j = 0; j < projectiles.size();j ++)
	{
		if (!projectiles.at(j).getIsOnMap()) //set building targets to null to prevent them attacking nonexistant units
		{
			projectiles.erase(projectiles.begin() + j);
		}
	}
}

int GTDMap::getNumWaveUnitsOnMap()
{
	int count = 0;
	for (unsigned int i = 0; i < units.size(); i++)
	{
		if (units.at(i).isOnMap() && units.at(i).isWaveUnit())
		{
			count++;
		}
	}
	return count;
}

void GTDMap::selectUnitsInRect(GTDRect *rect)
{
	for (unsigned int i = 0; i < units.size(); i++)
	{
		if (rectContainsUnit(*rect, units.at(i)))
		{
			units.at(i).select();
		}
		else
		{
			units.at(i).unselect();
		}
	}
}

void GTDMap::issueAttackOrder(GTDPlayer *p, GTDRect *rect)
{
	GTDUnit *target = NULL;
	for (unsigned int i = 0; i < units.size(); i++)
	{
		if (units.at(i).isWaveUnit() && rectContainsUnit(*rect, units.at(i)))
		{
			target = &units.at(i);
		}
	}
	if (target) //if target was found
	{
		for (unsigned int i = 0; i < units.size(); i++)
		{
			if (units.at(i).isSelected() && units.at(i).getOwner() == p && units.at(i).isBuilding() && units.at(i).isWithinDistanceOfUnit(units.at(i).getAttackRange(), target))
			{
				units.at(i).setTarget(target);
			}
		}
	}
}

GTDUnit * GTDMap::getSelectedUnit()
{
	int count = 0;
	int selectedIndex = 0;
	for (unsigned int i = 0; i < units.size(); i++)
	{
		if (units.at(i).isSelected())
		{
			selectedIndex = i;
			count++;
		}
	}
	if (count == 1) //only return a unit if there is 1 selected unit. otherwise return null since it is not needed
	{
		return &units.at(selectedIndex);
	}
	else
	{
		return NULL;
	}
}

int GTDMap::getMapW()
{
	return mapW;
}
int GTDMap::getMapH()
{
	return mapH;
}

int GTDMap::getTileW()
{
	return tileW;
}
int GTDMap::getTileH()
{
	return tileH;
}

int GTDMap::getLives()
{
	return lives;
}

bool GTDMap::spaceIsBuildable(int m, int n)
{
	if ((getMapBoard(m, n) >= 1 && getMapBoard(m, n) <= 164) ||
		(getMapBoard(m, n) >= 190 && getMapBoard(m, n) <= 221) ||
		(getMapBoard(m, n) >= 284 && getMapBoard(m, n) <= 317) ||
		getMapBoard(m, n) == 322 ||
		(getMapBoard(m, n) >= 332 && getMapBoard(m, n) <= 337) ||
		getMapBoard(m, n) == 345)
	{
		return false; //terrain is unbuildable
	}

	GTDRect buildingRect;
	buildingRect.setX(n * getTileH());
	buildingRect.setY(m * getTileW());
	buildingRect.setW(getTileW());
	buildingRect.setH(getTileH());
	for (unsigned int i = 0; i < units.size(); i++)
	{
		if (rectContainsUnit(buildingRect, units.at(i)))
		{
			return false;//building in the way
		}
	}
	return true;
}


bool GTDMap::rectContainsUnit(GTDRect rect, GTDUnit unit)
{
	if (unit.getPosX() >= rect.getX() &&
		unit.getPosX() <= (rect.getX() + rect.getW()) &&
		unit.getPosY() >= rect.getY() &&
		unit.getPosY() <= (rect.getY() + rect.getH()))
	{
		return true;
	}
	return false;
}

GTDMap::~GTDMap()
{
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		SDL_DestroyTexture(textures.at(i));
	}
	delete []mapBoard;
}
