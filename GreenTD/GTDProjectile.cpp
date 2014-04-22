#include "GTDProjectile.h"

GTDProjectile::GTDProjectile(GTDProjectileType p, GTDPlayer *o, int x, int y, int d, GTDUnit *t, SDL_Renderer *renderer)
{
	owner = o;
	posX = x;
	posY = y;
	damage = d;
	target = t;
	isOnMap = false;
	movespeed = getMoveSpeed(p);
	switch (p)
	{
	case NORMAL:
		if (!loadProjTexture("./assets/projectiles/NORMAL.png", renderer))
		{
			cout << "NORMAL tower projectile texture load failed" << endl;
		}
		break;
	case FIRE:
		if (!loadProjTexture("./assets/projectiles/FIRE.png", renderer))
		{
			cout << "FIRE tower projectile texture load failed" << endl;
		}
		break;
	case ICE:
		if (!loadProjTexture("./assets/projectiles/ICE.png", renderer))
		{
			cout << "ICE tower projectile texture load failed" << endl;
		}
		break;
	case LIGHTNING:
		if (!loadProjTexture("./assets/projectiles/LIGHTNING.png", renderer))
		{
			cout << "LIGHTNING tower projectile texture load failed" << endl;
		}
		break;
	case EARTH:
		if (!loadProjTexture("./assets/projectiles/EARTH.png", renderer))
		{
			cout << "EARTH tower projectile texture load failed" << endl;
		}
		break;
	default:
		texture = NULL;
		break;
	}
}

GTDPlayer * GTDProjectile::getOwner()
{
	return owner;
}

GTDUnit * GTDProjectile::getTarget()
{
	return target;
}
SDL_Texture * GTDProjectile::getTexture()
{
	return texture;
}

double GTDProjectile::getPosX()
{
	return posX;
}

double GTDProjectile::getPosY()
{
	return posY;
}

double GTDProjectile::getFacingAngle()
{
	return facingAngle;
}

void GTDProjectile::step(int timeElapsed)
{
	if (!target || target->isDead() || target->getInvuln() || !target->isOnMap())
	{
		die();
	}
	else
	{
		if (hasReachedTarget())
		{
			int damageDealt = damage * ((100 - (double)target->getArmor()) / 100);
			target->setHealth(target->getHealth() - damageDealt);
			std::cout << "target hit for " << damageDealt << " damage.  He now has " << target->getHealth() << " health." << std::endl;
			if (target->isDead())
			{
				int moneyEarned;
				if (target->getBountyrange() == 0)
				{
					moneyEarned = target->getBounty();
				}
				else
				{
					moneyEarned = target->getBounty() + ((rand() % target->getBountyrange()) - (target->getBountyrange() / 2));
				}
				owner->earn(moneyEarned);
				owner->addKill();
				std::cout << "Player earned " << moneyEarned << " money.  He now has " << owner->getMoney() << " money." << std::endl;
			}
			die();
		}
		else
		{
			//move towards center of unit
			double xC = target->getPosX();
			double yC = target->getPosY();

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
}

void GTDProjectile::die()
{
	setOnMap(false);
}

void GTDProjectile::setOnMap(bool t)
{
	isOnMap = t;
}

bool GTDProjectile::getIsOnMap()
{
	return isOnMap;
}

bool GTDProjectile::hasReachedTarget()
{
	if (!isOnMap || !target->isOnMap())
	{
		return false;
	}
	double dx = target->getPosX() - posX;
	double dy = target->getPosY() - posY;

	double distance = sqrt(dx*dx + dy*dy);
	if (distance <= target->getCollision()/4) //if projetile is in a box half width and half height of target, it has reached
	{
		return true;
	}
	return false;
}

bool GTDProjectile::loadProjTexture(string fn, SDL_Renderer *renderer)
{
	//Load BMP from filepath
	SDL_Surface *PNG = IMG_Load(fn.c_str());
	texture = SDL_CreateTextureFromSurface(renderer, PNG);

	SDL_FreeSurface(PNG);

	if (!texture)
	{
		return false;
	}

	return true;
}

int GTDProjectile::getMoveSpeed(GTDProjectileType p)
{
	switch (p)
	{
	case NORMAL:
		return 750;
	case FIRE:
		return 550;
	case ICE:
		return 800;
	case LIGHTNING:
		return 1000;
	case EARTH:
		return 650;
	default:
		return 0;
	}
}