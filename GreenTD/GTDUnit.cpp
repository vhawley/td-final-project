#include "GTDUnit.h"

GTDUnit::GTDUnit()
{


}

GTDUnit::GTDUnit(enum GTDBuilding b, GTDPlayer *own, double x, double y, SDL_Renderer *rend)
{
	unitType = BUILDING;
	bType = b;
	renderer = rend;
	setPosX(x);
	setPosY(y);
	cost = getCost(b);
	name = getName(b);
	collision = getCollision(b);
	attackDMG = getAttackDMG(b);
	attackDMGRange = getAttackDMGRange(b);
	attackRange = getAttackRange(b);
	attackCooldown = getAttackCooldown(b);
	owner = own;
	atkCooldownTimer = 0;
	armor = 0;
	invuln = false;
	bounty = 0;
	bountyrange = 0;
	movespeed = 0;
	maxhealth = 100;
	health = maxhealth;
	facingAngle = 0;
	queuedProjectile = false;
	switch (b)
	{
		case NORMAL:
			cout << "Creating NORMAL building..." << endl;
			if (!loadUnitTexture("./assets/towers/NORMAL.bmp", renderer))
			{
				cout << "NORMAL tower texture load failed" << endl;
			}
			break;
		case FIRE:
			cout << "Creating FIRE building..." << endl;
			if (!loadUnitTexture("./assets/towers/FIRE.bmp", renderer))
			{
				cout << "FIRE tower texture load failed" << endl;
			}
			break;
		case ICE:
			cout << "Creating ICE building..." << endl;
			if (!loadUnitTexture("./assets/towers/ICE.bmp", renderer))
			{
				cout << "ICE tower texture load failed" << endl;
			}
			break;
		case LIGHTNING:
			cout << "Creating LIGHTNING building..." << endl;
			if (!loadUnitTexture("./assets/towers/LIGHTNING.bmp", renderer))
			{
				cout << "LIGHTNING tower texture load failed" << endl;
			}
				break;
		case EARTH:
			cout << "Creating EARTH building..." << endl;
			if (!loadUnitTexture("./assets/towers/EARTH.bmp", renderer))
			{
				cout << "EARTH tower texture load failed" << endl;
			}
				break;
		case SPEEDASSIST:
			cout << "Creating SPEEDASSIST building..." << endl;
			if (!loadUnitTexture("./assets/towers/SPEEDASSIST.bmp", renderer))
			{
				cout << "SPEEDASSIST tower texture load failed" << endl;
			}
				break;
		case DMGASSIST:
			cout << "Creating DMGASSIST building..." << endl;
			if (!loadUnitTexture("./assets/towers/DMGASSIST.bmp", renderer))
			{
				cout << "DMGASSIST tower texture load failed" << endl;
			}
				break;
		default:
				break;
	}


}


GTDUnit::GTDUnit(enum GTDWaveUnit w, double x, double y, SDL_Renderer *rend, GTDWaypoint way)
{
	unitType = WAVEUNIT;
	wType = w;
	renderer = rend;
	waypoint = way;
	reachedEnd = false;
	setPosX(x);
	setPosY(y);
	name = getName(w);
	owner = NULL;
	cost = 0;
	attackRange = 0;
	maxhealth = getMaxHealth(w);
	health = maxhealth;
	armor = getBaseArmor(w);
	invuln = false;
	movespeed = getMoveSpeed(w);
	bounty = getBounty(w);
	bountyrange = getBountyRange(w);
	attackDMG = 0;
	attackDMGRange = 0;
	facingAngle = 0;
	queuedProjectile = false;
	if (waypoint.first)
	{
		issueMoveToRect(waypoint.first->rect);
	}
	switch (w)
	{
	case VILLAGER:
		cout << "Creating VILLAGER unit..." << endl;
		
		collision = 26;
		if (!loadUnitTexture("./assets/creeps/VILLAGER.png", renderer))
		{
			cout << "VILLAGER texture load failed" << endl;
		}
		break;
	case SWORDSMAN:
		cout << "Creating SWORDSMAN unit..." << endl;
		collision = 28;
		if (!loadUnitTexture("./assets/creeps/SWORDSMAN.png", renderer))
		{
			cout << "SWORDSMAN texture load failed" << endl;
		}
		break;
	case KNIGHT:
		cout << "Creating KNIGHT unit..." << endl;
		collision = 32;
		if (!loadUnitTexture("./assets/creeps/KNIGHT.png", renderer))
		{
			cout << "KNIGHT texture load failed" << endl;
		}
		break;
	case KING:
		cout << "Creating KING unit..." << endl;
		collision = 40;
		if (!loadUnitTexture("./assets/creeps/KING.png", renderer))
		{
			cout << "KING texture load failed" << endl;
		}
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

string GTDUnit::getName()
{
	return name;
}

GTDUnit::GTDBuilding GTDUnit::getBuildingType()
{
	return bType;
}
GTDUnit::GTDWaveUnit GTDUnit::getWaveUnitType()
{
	return wType;
}

GTDUnit * GTDUnit::getTarget()
{
	return target;
}

bool GTDUnit::hasQueuedProjectile()
{
	return queuedProjectile;
}

void GTDUnit::setQueuedProjectile(bool t)
{
	queuedProjectile = false;
}

void GTDUnit::setTarget(GTDUnit *u)
{
	if (!u)
	{
		target = NULL;
	}
	else
	{
		target = u;
	}
}

bool GTDUnit::hasTarget()
{
	if (target == NULL)
	{
		return false;
	}
	return true;
}

bool GTDUnit::isDead()
{
	if (health <= 0)
	{
		return true;
	}
	return false;
}

void GTDUnit::setOnMap(bool on)
{
	onMap = on;
}

void GTDUnit::step(int timeElapsed) //time elapsed in milliseconds
{
	switch (unitType)
	{
	case BUILDING:
		//what to do as a building every game tick. check for attack.  attack if possible
		switch (bType)
		{
		case SPEEDASSIST:
			//map provides speed buff to nearby towers
			break;
		case DMGASSIST:
			//map provides dmg buff to nearby towers
			break;
		default:
			if (atkCooldownTimer <= 0 && hasTarget())
			{
				if (target->isDead() || target->didReachEnd() || target->getInvuln())
				{
					setTarget(NULL);
				}
				else if (isWithinDistanceOfUnit(attackRange, target))
				{
					attackTarget();
					atkCooldownTimer = attackCooldown;
				}
			}
			else if (!hasTarget() && atkCooldownTimer <= 0)
			{

			}
			else if (atkCooldownTimer > 0)
			{
				atkCooldownTimer -= timeElapsed;
			}
			break;
		}
		break;
	case WAVEUNIT:
		//what to do as a unit every game tick. 
		if (isOnMap())
		{
			if (atDestination())
			{
				//set next Destination
				if (waypoint.first->next != NULL)
				{
					waypoint.advance();
					issueMoveToRect(waypoint.first->rect);
				}
				else
				{
					reachedEnd = true;
				}

			}
			else //move if it has a waypoint
			{
				//move towards center of next waypoint
				double xC = currentDest->getX() + (double)currentDest->getW() / 2;
				double yC = currentDest->getY() + (double)currentDest->getH() / 2;

				double dx = xC - posX;
				double dy = yC - posY;

				facingAngle = atan(dy / dx);
				if (dx < 0) //only dx < 0 because cos is always positive. sin can return - values!!
				{
					posX -= cos(facingAngle) * ((double)timeElapsed / 1000) * movespeed;
					posY -= sin(facingAngle) * ((double)timeElapsed / 1000) * movespeed;
				}
				else
				{
					posX += cos(facingAngle) * ((double)timeElapsed / 1000) * movespeed;
					posY += sin(facingAngle) * ((double)timeElapsed / 1000) * movespeed;
				}
			}
		}
		break;
	default:
		break;
	}

}

void GTDUnit::issueMoveToPoint(int x, int y)
{
	GTDRect *tempDest = new GTDRect(x - 2, y - 2, 4, 4);
	currentDest = tempDest;
}
void GTDUnit::issueMoveToRect(GTDRect *rect)
{
	currentDest = rect;
}

bool GTDUnit::isWithinDistanceOfUnit(double d, GTDUnit *u)
{
	if (!u->isOnMap())
	{
		return false;
	}
	double dx = u->getPosX() - posX;
	double dy = u->getPosY() - posY;

	double distance = sqrt(dx*dx + dy*dy);
	if (distance <= d)
	{
		return true;
	}
	return false;
}

bool GTDUnit::isOnMap()
{
	return onMap;
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

GTDRect GTDUnit::getCurrentDest()
{
	return *currentDest;
}

void GTDUnit::select()
{
	selected = true;
}


void GTDUnit::unselect()
{
	selected = false;
}


bool GTDUnit::isSelected()
{
	return selected;
}

bool GTDUnit::didReachEnd()
{
	return reachedEnd;
}

double GTDUnit::getFacingAngle()
{
	return facingAngle;
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
	return attackDMGRange;
}
int GTDUnit::getAttackRange()
{ 
	return attackRange;
}

void GTDUnit::setHealth(int h)
{
	if (h > 0)
	{
		health = std::min(maxhealth, h);
	}
	else
	{
		health = 0;
	}
	
}

void GTDUnit::attackTarget()
{
	queuedProjectile = true;
}

string GTDUnit::getName(enum GTDBuilding b)
{
	switch (b)
	{
		case NORMAL:
			return "Normal Tower";
		case FIRE:
			return "Fire Tower";
		case ICE:
			return "Ice Tower";
		case LIGHTNING:
			return "Lightning Tower";
		case EARTH:
			return "Earth Tower";
		case SPEEDASSIST:
			return "Speed Assist Tower";
		case DMGASSIST:
			return "Damage Assist Tower";
		default:
			return " ";
	}
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
			return 250;
		case SPEEDASSIST:
			return 300;
		case DMGASSIST:
			return 300;
		default:
			return 0;
	}
}


int GTDUnit::getAttackDMG(enum GTDBuilding b)
{
	switch (b)
	{
		case NORMAL:
			return 18;
		case FIRE:
			return 30;
		case ICE:
			return 22;
		case LIGHTNING:
			return 35;
		case EARTH:
			return 50;
		case SPEEDASSIST:
			return 0;
		case DMGASSIST:
			return 0;
		default:
			return 0;
	}
}
int GTDUnit::getAttackDMGRange(enum GTDBuilding b)
{
	switch (b)
	{
		case NORMAL:
			return 3;
		case FIRE:
			return 10;
		case ICE:
			return 2;
		case LIGHTNING:
			return 15;
		case EARTH:
			return 0;
		case SPEEDASSIST:
			return 0;
		case DMGASSIST:
			return 0;
		default:
			return 0;
	}
}
int GTDUnit::getAttackRange(enum GTDBuilding b)
{
	switch (b)
	{
		case NORMAL:
			return 200;
		case FIRE:
			return 300;
		case ICE:
			return 300;
		case LIGHTNING:
			return 150;
		case EARTH:
			return 300;
		case SPEEDASSIST:
			return 250;
		case DMGASSIST:
			return 250;
		default:
			return 0;
	}
}
int GTDUnit::getAttackCooldown(enum GTDBuilding b)
{
	switch (b)
	{
		case NORMAL:
			return 1000;
		case FIRE:
			return 1250;
		case ICE:
			return 1100;
		case LIGHTNING:
			return 500;
		case EARTH:
			return 2000;
		case SPEEDASSIST:
			return 0;
		case DMGASSIST:
			return 0;
		default:
			return 0;
	}
}

string GTDUnit::getName(enum GTDWaveUnit w)
{
	switch (w)
	{
		case VILLAGER:
			return "Villager";
		case SWORDSMAN:
			return "Swordsman";
		case KNIGHT:
			return "Knight";
		case KING:
			return "King";
		default:
			return " ";
	}
}

int GTDUnit::getMaxHealth(enum GTDWaveUnit w)
{
	switch (w)
	{
		case VILLAGER:
			return 50;
		case SWORDSMAN:
			return 200;
		case KNIGHT:
			return 400;
		case KING:
			return 1000;
		default:
			return 50;
	}
}
int GTDUnit::getBaseArmor(enum GTDWaveUnit w)
{
	switch (w)
	{
		case VILLAGER:
			return 2;
		case SWORDSMAN:
			return 6;
		case KNIGHT:
			return 12;
		case KING:
			return 8;
		default:
			return 0;
	}
}
int GTDUnit::getMoveSpeed(enum GTDWaveUnit w)
{
	switch (w)
	{
		case VILLAGER:
			return 75;
		case SWORDSMAN:
			return 90;
		case KNIGHT:
			return 100;
		case KING:
			return 50;
		default:
			return 50;
	}
}
int GTDUnit::getBounty(enum GTDWaveUnit w)
{
	switch (w)
	{
		case VILLAGER:
			return 15;
		case SWORDSMAN:
			return 50;
		case KNIGHT:
			return 100;
		case KING:
			return 200;
		default:
			return 50;
	}
}
int GTDUnit::getBountyRange(enum GTDWaveUnit w)
{
	switch (w)
	{
		case VILLAGER:
			return 3;
		case SWORDSMAN:
			return 5;
		case KNIGHT:
			return 10;
		case KING:
			return 0;
		default:
			return 50;
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
	if (currentDest == NULL)
	{
		return true;
	}
	else
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
}


bool GTDUnit::loadUnitTexture(string fn, SDL_Renderer *renderer)
{
	//Load BMP from filepath
	SDL_Surface *image = IMG_Load(fn.c_str());
	text = SDL_CreateTextureFromSurface(renderer, image);

	SDL_FreeSurface(image);

	if (!text)
	{
			return false;
	}
	
	return true;
}

GTDUnit::~GTDUnit()
{

}

