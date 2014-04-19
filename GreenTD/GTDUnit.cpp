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
			attackRange = 300;
			attackCooldown = 1000;
			atkCooldownTimer = 0;
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


GTDUnit::GTDUnit(enum GTDWaveUnit w, double x, double y, SDL_Renderer *rend, GTDWaypoint way)
{
	unitType = WAVEUNIT;
	renderer = rend;
	waypoint = way;
	setPosX(x);
	setPosY(y);
	owner = NULL;
	cost = 0;
	attackRange = 0;
	if (waypoint.first)
	{
		issueMoveToRect(waypoint.first->rect);
	}
	switch (w)
	{
	case VILLAGER:
		cout << "Creating VILLAGER unit..." << endl;
		
		collision = 28;
		if (!loadUnitTexture("./assets/creeps/VILLAGER.bmp", renderer))
		{
			cout << "VILLAGER texture load failed" << endl;
		}
		name = "Villager";
		
		maxhealth = 50;
		health = 50;
		armor = 0;
		invuln = false;
		movespeed = 100;
		bounty = 10;
		bountyrange = 2;
		attackDMG = 18;
		attackDMGRange = 3;
		break;
	case SWORDSMAN:
		cout << "Creating SWORDSMAN unit..." << endl;
		collision = 28;
		if (!loadUnitTexture("./assets/creeps/SWORDSMAN.bmp", renderer))
		{
			cout << "SWORDSMAN texture load failed" << endl;
		}
		name = "Swordsman";
		maxhealth = 75;
		health = 75;
		armor = 2;
		invuln = false;
		movespeed = 100;
		bounty = 10;
		bountyrange = 2;
		attackDMG = 18;
		attackDMGRange = 3;
		break;
	case KNIGHT:
		cout << "Creating KNIGHT unit..." << endl;
		collision = 28;
		if (!loadUnitTexture("./assets/creeps/KNIGHT.bmp", renderer))
		{
			cout << "KNIGHT texture load failed" << endl;
		}
		name = "Knight";
		maxhealth = 95;
		health = 95;
		armor = 3;
		invuln = false;
		movespeed = 100;
		bounty = 10;
		bountyrange = 2;
		attackDMG = 18;
		attackDMGRange = 3;
		break;
	case KING:
		cout << "Creating KING unit..." << endl;
		collision = 28;
		if (!loadUnitTexture("./assets/creeps/KING.bmp", renderer))
		{
			cout << "KING texture load failed" << endl;
		}
		name = "Knight";
		maxhealth = 120;
		health = 120;
		armor = 5;
		invuln = false;
		movespeed = 100;
		bounty = 10;
		bountyrange = 2;
		attackDMG = 18;
		attackDMGRange = 3;
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

void GTDUnit::setTarget(GTDUnit *u)
{
	target = u;
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



void GTDUnit::step(int timeElapsed) //time elapsed in milliseconds
{
	switch (unitType)
	{
	case BUILDING:
		//what to do as a building every game tick. check for attack.  attack if possible
		if (atkCooldownTimer <= 0 && hasTarget())
		{
			if (isWithinDistanceOfUnit(attackRange, target) && !target->isDead() && !target->getInvuln())
			{
				//issue attack
				attackTarget();
				atkCooldownTimer = attackCooldown;
			}
			else
			{
				setTarget(NULL); //cannot attack unit anymore
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
	case WAVEUNIT:
		//what to do as a unit every game tick. 
		if (!isDead())
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
					//reached the end
				}

			}
			else //move if it has a waypoint
			{
				//move towards center of next waypoint
				double xC = currentDest->getX() + (double)currentDest->getW() / 2;
				double yC = currentDest->getY() + (double)currentDest->getH() / 2;

				double dx = xC - posX;
				double dy = yC - posY;

				double walkangle = atan(dy / dx);

				if (dx < 0) //only dx < 0 because cos is always positive. sin can return - values!!
				{
					posX -= cos(walkangle) * ((double)timeElapsed / 1000) * movespeed;
					posY -= sin(walkangle) * ((double)timeElapsed / 1000) * movespeed;
				}
				else
				{
					posX += cos(walkangle) * ((double)timeElapsed / 1000) * movespeed;
					posY += sin(walkangle) * ((double)timeElapsed / 1000) * movespeed;
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


}
void GTDUnit::issueMoveToRect(GTDRect *rect)
{
	currentDest = rect;
}

bool GTDUnit::isWithinDistanceOfUnit(double d, GTDUnit *u)
{
	double dx = abs(u->getPosX() - posX);
	double dy = abs(u->getPosY() - posY);

	double distance = sqrt(dx*dx + dy*dy);
	if (distance <= d)
	{
		return true;
	}
	return false;
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
	int damageDealt = (attackDMG + (rand() % attackDMGRange) - (attackDMGRange / 2)) * (((double)100 - target->getArmor()) / 100);
	target->setHealth(target->getHealth()-std::max(damageDealt,0));
	if (target->isDead())
	{
		int moneyEarned = (target->getBounty() + (rand() % target->getBountyrange()) - (target->getBountyrange() / 2));
		owner->earn(moneyEarned);
		owner->addKill();
		std::cout << "Player earned " << moneyEarned << " money.  He now has " << owner->getMoney() << " money." << std::endl;
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
	SDL_Surface *BMP = SDL_LoadBMP(fn.c_str());
	text = SDL_CreateTextureFromSurface(renderer, BMP);

	SDL_FreeSurface(BMP);

	if (!text)
	{
			return false;
	}
	
	return true;
}
