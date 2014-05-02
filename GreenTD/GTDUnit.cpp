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

	//set building attributes 
	cost = getCost(b);
	name = getName(b);
	collision = getCollision(b);
	attackDMG = getAttackDMG(b);
	attackDMGRange = getAttackDMGRange(b);
	attackRange = getAttackRange(b);
	attackCooldown = getAttackCooldown(b);

	//set owner (always the player from GTDGame... but if this ever becomes multiplayer this will mean something)
	owner = own;

	//ready to attack the moment from creation
	atkCooldownTimer = 0;

	//buildings dont use these
	armor = 0;
	invuln = false;
	bounty = 0;
	bountyrange = 0;
	movespeed = 0;
	maxhealth = 100;
	health = maxhealth;
	facingAngle = 0;
	queuedProjectile = false;

	//load textures and attack launch sounds depending on tower type
	switch (b)
	{
		case NORMAL:
			cout << "Creating NORMAL building..." << endl;
			if (!loadUnitTexture("./assets/towers/NORMAL.png", renderer))
			{
				cout << "NORMAL tower texture load failed" << endl;
			}
			attackLaunch = Mix_LoadWAV("./assets/sound/NORMAL.wav");
			break;
		case FIRE:
			cout << "Creating FIRE building..." << endl;
			if (!loadUnitTexture("./assets/towers/FIRE.png", renderer))
			{
				cout << "FIRE tower texture load failed" << endl;
			}
			attackLaunch = Mix_LoadWAV("./assets/sound/FIRE.wav");
			break;
		case ICE:
			cout << "Creating ICE building..." << endl;
			if (!loadUnitTexture("./assets/towers/ICE.png", renderer))
			{
				cout << "ICE tower texture load failed" << endl;
			}
			attackLaunch = Mix_LoadWAV("./assets/sound/ICE.wav");
			break;
		case LIGHTNING:
			cout << "Creating LIGHTNING building..." << endl;
			if (!loadUnitTexture("./assets/towers/LIGHTNING.png", renderer))
			{
				cout << "LIGHTNING tower texture load failed" << endl;
			}
			attackLaunch = Mix_LoadWAV("./assets/sound/LIGHTNING.wav");
				break;
		case EARTH:
			cout << "Creating EARTH building..." << endl;
			if (!loadUnitTexture("./assets/towers/EARTH.png", renderer))
			{
				cout << "EARTH tower texture load failed" << endl;
			}
			attackLaunch = Mix_LoadWAV("./assets/sound/EARTH.wav");
				break;
		case SPEEDASSIST:
			cout << "Creating SPEEDASSIST building..." << endl;
			if (!loadUnitTexture("./assets/towers/SPEEDASSIST.png", renderer))
			{
				cout << "SPEEDASSIST tower texture load failed" << endl;
			}
				break;
		case DMGASSIST:
			cout << "Creating DMGASSIST building..." << endl;
			if (!loadUnitTexture("./assets/towers/DMGASSIST.png", renderer))
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
	facingAngle = 0;

	//set waveunit attributes
	name = getName(w);
	maxhealth = getMaxHealth(w);
	health = maxhealth;
	armor = getBaseArmor(w);
	invuln = false;
	movespeed = getMoveSpeed(w);
	bounty = getBounty(w);
	bountyrange = getBountyRange(w);

	//no owner
	owner = NULL;

	//waveunits dont use these
	cost = 0;
	attackRange = 0;
	attackDMG = 0;
	attackDMGRange = 0;
	queuedProjectile = false;
	if (waypoint.first)
	{
		issueMoveToRect(waypoint.first->rect);
	}
	switch (w) //load texture based on waveunit type
	{
	case VILLAGER:
		cout << "Creating VILLAGER unit..." << endl;
		
		collision = 30;
		if (!loadUnitTexture("./assets/creeps/VILLAGER.png", renderer))
		{
			cout << "VILLAGER texture load failed" << endl;
		}
		break;
	case SWORDSMAN:
		cout << "Creating SWORDSMAN unit..." << endl;
		collision = 34;
		if (!loadUnitTexture("./assets/creeps/SWORDSMAN.png", renderer))
		{
			cout << "SWORDSMAN texture load failed" << endl;
		}
		break;
	case KNIGHT:
		cout << "Creating KNIGHT unit..." << endl;
		collision = 38;
		if (!loadUnitTexture("./assets/creeps/KNIGHT.png", renderer))
		{
			cout << "KNIGHT texture load failed" << endl;
		}
		break;
	case KING:
		cout << "Creating KING unit..." << endl;
		collision = 48;
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
				if (target->isDead() || target->didReachEnd() || target->getInvuln() || !isWithinDistanceOfUnit(attackRange, target))
				{
					//reset target if target isn't attackable for any reason when the unit is able to attack (atkcooldown reaches 0)
					setTarget(NULL);
				}
				else
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
				//set next Destination if at waypoint checkpoint or set reached end flag if it has no more waypoints
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

void GTDUnit::setAttackCooldown(int c)
{
	attackCooldown = c;
}

void GTDUnit::setAttackDMG(int d)
{
	attackDMG = d;
}

void GTDUnit::setMoveSpeed(int s)
{
	movespeed = s;
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
	Mix_PlayChannel(-1, attackLaunch, 0);
	queuedProjectile = true;
}

//static functions for unit attributes
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
			return 32;
	}
}

int GTDUnit::getCost(enum GTDBuilding b)
{
	switch (b)
	{
		case NORMAL:
			return 50;
		case FIRE:
			return 150;
		case ICE:
			return 225;
		case LIGHTNING:
			return 315;
		case EARTH:
			return 500;
		case SPEEDASSIST:
			return 400;
		case DMGASSIST:
			return 400;
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
			return 45;
		case ICE:
			return 60;
		case LIGHTNING:
			return 50;
		case EARTH:
			return 300;
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
			return 8;
		case ICE:
			return 4;
		case LIGHTNING:
			return 10;
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
			return 130;
		case FIRE:
			return 175;
		case ICE:
			return 250;
		case LIGHTNING:
			return 100;
		case EARTH:
			return 300;
		case SPEEDASSIST:
			return 200;
		case DMGASSIST:
			return 200;
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
			return 400;
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
			return 51;
		case SWORDSMAN:
			return 180;
		case KNIGHT:
			return 450;
		case KING:
			return 1200;
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
			return 20;
		default:
			return 0;
	}
}
int GTDUnit::getMoveSpeed(enum GTDWaveUnit w)
{
	switch (w)
	{
		case VILLAGER:
			return 60;
		case SWORDSMAN:
			return 77;
		case KNIGHT:
			return 87;
		case KING:
			return 82;
		default:
			return 50;
	}
}
int GTDUnit::getBounty(enum GTDWaveUnit w)
{
	switch (w)
	{
		case VILLAGER:
			return 8;
		case SWORDSMAN:
			return 18;
		case KNIGHT:
			return 34;
		case KING:
			return 58;
		default:
			return 10;
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

