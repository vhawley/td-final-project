#include "GTDUnit.h"

GTDUnit::GTDUnit()
{


}

GTDUnit::GTDUnit(enum GTDBuilding b, GTDPlayer *own, double x, double y, SDL_Renderer *rend)
{
	unitType = BUILDING;
	renderer = rend;
	switch (b)
	{
		case NORMAL:
			cout << "Creating NORMAL building..." << endl;
			setPosX(x);
			setPosY(y);
			collision = 32;
			if (!loadUnitTexture("./assets/towers/NORMAL.bmp", renderer))
			{
				cout << "NORMAL tower texture load failed" << endl;
			}
			name = "Normal Tower";
			owner = own;
			maxhealth = 100;
			health = 100;
			armor = 2;
			invuln = false;
			movespeed = 0;
			bounty = 50;
			bountyrange = 5;
			attackDMG = 18;
			attackDMGRange = 3;
			cost = getCost(b);
			break;
		case FIRE:
			cout << "Creating FIRE building..." << endl;
			cost = getCost(b);
			break;
		case ICE:
			cout << "Creating ICE building..." << endl;
			cost = getCost(b);
			break;
		case LIGHTNING:
			cout << "Creating LIGHTNING building..." << endl;
			cost = getCost(b);
				break;
		case EARTH:
			cout << "Creating EARTH building..." << endl;
			cost = getCost(b);
				break;
		case SPEEDASSIST:
			cout << "Creating SPEEDASSIST building..." << endl;
			cost = getCost(b);
				break;
		case DMGASSIST:
			cout << "Creating DMGASSIST building..." << endl;
			cost = getCost(b);
				break;
		default:
				break;
	}


}


GTDUnit::GTDUnit(enum GTDWaveUnit w, double x, double y, SDL_Renderer *rend)
{
	renderer = rend;
	unitType = WAVEUNIT;
	switch (w)
	{
	case VILLAGER:
		cout << "Creating VILLAGER unit..." << endl;
		setPosX(x);
		setPosY(y);
		collision = 28;
		if (!loadUnitTexture("./assets/creeps/VILLAGER.bmp", renderer))
		{
			cout << "NORMAL tower texture load failed" << endl;
		}
		name = "Villager";
		owner = NULL;
		maxhealth = 50;
		health = 50;
		armor = 2;
		invuln = false;
		movespeed = 200;
		bounty = 10;
		bountyrange = 2;
		attackDMG = 18;
		attackDMGRange = 3;
		cost = 0;
		currentDest = new GTDRect(2000, 500, 100, 100);
		break;
	case SWORDSMAN:
		cout << "Creating SWORDSMAN unit..." << endl;
		break;
	case KNIGHT:
		cout << "Creating KNIGHT unit..." << endl;
		break;
	case KING:
		cout << "Creating KING unit..." << endl;
		break;
	default:
		break;
	}
}



double GTDUnit::getPosX()
{
	return posX;
}
double GTDUnit::getPosY()
{
	return posY;
}

int GTDUnit::getCollision()
{
	return collision;
}

SDL_Texture * GTDUnit::getTexture()
{
	return text;
}

GTDPlayer * GTDUnit::getOwner()
{
	return owner;
}

void GTDUnit::step(int timeElapsed) //time elapsed in milliseconds
{
	switch (unitType)
	{
	case BUILDING:
		//what to do as a building every game tick. check for attack.  attack if possible

		break;
	case WAVEUNIT:
		//what to do as a unit every game tick. 
		if (atDestination())
		{
			//set next Destination
			health = .50 * maxhealth;
		}
		else
		{
			//move towards center of next waypoint
			double xC = currentDest->getX() + (double) currentDest->getW() / 2;
			double yC = currentDest->getY() + (double) currentDest->getH() / 2;

			double dx = xC - posX;
			double dy = yC - posY;

			double walkangle = atan(dy / dx);

			posX += cos(walkangle) * ((double)timeElapsed / 1000) * movespeed;
			posY += sin(walkangle) * ((double)timeElapsed / 1000) * movespeed;
		}
		break;
	default:
		break;
	}

}

void GTDUnit::issueMoveToPoint(int x, int y)
{


}
void GTDUnit::issueMoveToRect(GTDRect rect)
{

}

bool GTDUnit::isBuilding()
{
	if (unitType == BUILDING)
	{
		return true;
	}
	return false;
}
bool GTDUnit::isWaveUnit()
{
	if (unitType == WAVEUNIT)
	{
		return true;
	}
	return false;
}

bool GTDUnit::isSelected()
{
	return selected;
}

int GTDUnit::getMaxHealth()
{
	return maxhealth;
}

int GTDUnit::getHealth()
{ 
	return health;
}
int GTDUnit::getArmor()
{ 
	return armor;
}
bool GTDUnit::getInvuln()
{ 
	return invuln;
}
int GTDUnit::getMovespeed()
{ 
	return movespeed;
}
int GTDUnit::getBounty()
{
	return bounty;
}
int GTDUnit::getBountyrange()
{ 
	return bountyrange;
}
int GTDUnit::getAttackDMG()
{ 
	return attackDMG;
}
int GTDUnit::getAttackDMGRange()
{
	return attackDMG;
}
int GTDUnit::getAttackRange()
{ 
	return attackRange;
}


int GTDUnit::getCollision(enum GTDBuilding b)
{
	switch (b)
	{
		case NORMAL:
			return 32;
		case FIRE:
			return 32;
		case ICE:
			return 32;
		case LIGHTNING:
			return 32;
		case EARTH:
			return 32;
		case SPEEDASSIST:
			return 32;
		case DMGASSIST:
			return 32;
		default:
			return 0;
	}
}

int GTDUnit::getCost(enum GTDBuilding b)
{
	switch (b)
	{
		case NORMAL:
			return 100;
		case FIRE:
			return 150;
		case ICE:
			return 200;
		case LIGHTNING:
			return 250;
		case EARTH:
			return 225;
		case SPEEDASSIST:
			return 300;
		case DMGASSIST:
			return 300;
		default:
			return 0;
	}
}


void GTDUnit::setPosX(double x)
{
	posX = x;
}
void GTDUnit::setPosY(double y)
{
	posY = y;
}

bool GTDUnit::atDestination()
{
	if (posX > currentDest->getX() &&
		posX < (currentDest->getX() + currentDest->getW()) &&
		posY > currentDest->getY() &&
		posY < (currentDest->getY() + currentDest->getH()))
	{
		return true;
	}
	return false;
}


bool GTDUnit::loadUnitTexture(string fn, SDL_Renderer *renderer)
{
	//Load BMP from filepath
	SDL_Surface *BMP = SDL_LoadBMP(fn.c_str());
	text = SDL_CreateTextureFromSurface(renderer, BMP);

	SDL_FreeSurface(BMP);

	if (!text)
	{
			return false;
	}
	
	return true;
}
