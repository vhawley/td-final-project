#include "GTDUnit.h"

GTDUnit::GTDUnit()
{


}

GTDUnit::GTDUnit(enum GTDBuilding b, GTDPlayer *own, int x, int y, SDL_Renderer *renderer)
{
	unitType = BUILDING;
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
			break;
		case FIRE:
			cout << "Creating FIRE building..." << endl;
			break;
		case ICE:
			cout << "Creating ICE building..." << endl;
				break;
		case LIGHTNING:
			cout << "Creating LIGHTNING building..." << endl;
				break;
		case EARTH:
			cout << "Creating EARTH building..." << endl;
				break;
		case SPEEDASSIST:
			cout << "Creating SPEEDASSIST building..." << endl;
				break;
		case DMGASSIST:
			cout << "Creating DMGASSIST building..." << endl;
				break;
		default:
				break;
	}


}


GTDUnit::GTDUnit(enum GTDWaveUnit w, int x, int y, SDL_Renderer *renderer)
{


}



int GTDUnit::getPosX()
{
	return posX;
}
int GTDUnit::getPosY()
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

void GTDUnit::setPosX(int x)
{
	posX = x;
}
void GTDUnit::setPosY(int y)
{
	posY = y;
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
