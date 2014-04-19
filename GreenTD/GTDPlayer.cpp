#include "GTDPlayer.h"

GTDPlayer::GTDPlayer()
{
	money = 750;
	over = 0;
	lastkey = "NULL";
}

int GTDPlayer::getOver()
{
	return over;
}

void GTDPlayer::processInput()
{
	while (SDL_PollEvent(&e)) {
		switch (e.type)
		{
			case SDL_KEYDOWN:
				key = SDL_GetKeyName(e.key.keysym.sym);
				keyDown[key] = true;
				if (!strcmp(key, lastkey))
				{
					std::cout << "Holding: " << key << std::endl;
				}
				else
				{
					std::cout << "Key pressed: " << key << std::endl;
					if (!strcmp(key, "B"))
					{
						std::cout << "Build mode..." << std::endl;
						building = 1;
					}
					if (!strcmp(key, "C"))
					{
						std::cout << "Build mode off..." << std::endl;
					}
					if (!strcmp(key, "1") && building == 0)
					{
						currentlySelectedBuilding = 0;
						std::cout << "Selected building = NORMAL..." << std::endl;
					}
					if (!strcmp(key, "2") && building == 0)
					{
						currentlySelectedBuilding = 1;
						std::cout << "Selected building = FIRE..." << std::endl;
					}
					if (!strcmp(key, "3") && building == 0)
					{
						currentlySelectedBuilding = 2;
						std::cout << "Selected building = ICE..." << std::endl;
					}
					if (!strcmp(key, "4") && building == 0)
					{
						currentlySelectedBuilding = 3;
						std::cout << "Selected building = LIGHTNING..." << std::endl;
					}
					if (!strcmp(key, "5") && building == 0)
					{
						currentlySelectedBuilding = 4;
						std::cout << "Selected building = EARTH..." << std::endl;
					}
					if (!strcmp(key, "6") && building == 0)
					{
						currentlySelectedBuilding = 5;
						std::cout << "Selected building = SPEEDASSIST..." << std::endl;
					}
					if (!strcmp(key, "7") && building == 0)
					{
						currentlySelectedBuilding = 6;
						std::cout << "Selected building = DMGASSIST..." << std::endl;
					}
					lastkey = key;
				}
				
				break;
			case SDL_KEYUP:
				key = SDL_GetKeyName(e.key.keysym.sym);
				std::cout << "Key released: " << key << std::endl;
				lastkey = "NULL";
				keyDown[key] = false;
				if (!strcmp(key, "Q"))
				{
					over = 1;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch (e.button.button)
				{
					case SDL_BUTTON_LEFT:
						std::cout << "Mouse1 Pressed" << std::endl;
						SDL_GetMouseState(&oldMouseX, &oldMouseY);
						keyDown["MOUSE1"] = true;
						break;
					case SDL_BUTTON_RIGHT:
						std::cout << "Mouse2 Pressed" << std::endl;
						keyDown["MOUSE2"] = true;
						break;
					case SDL_BUTTON_MIDDLE:
						std::cout << "Mouse3 Pressed" << std::endl;
						keyDown["MOUSE3"] = true;
						break;
					case SDL_BUTTON_X1:
						std::cout << "Mouse4 Pressed" << std::endl;
						keyDown["MOUSE4"] = true;
						break;
					case SDL_BUTTON_X2:
						std::cout << "Mouse5 Pressed" << std::endl;
						keyDown["MOUSE5"] = true;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					std::cout << "Mouse1 Released" << std::endl;
					keyDown["MOUSE1"] = false;
					if (isSelectingBuildLocation())
					{
						queueBuilding();
					}
					else
					{
						queueSelection();
					}
					break;
				case SDL_BUTTON_RIGHT:
					std::cout << "Mouse2 Released" << std::endl;
					keyDown["MOUSE2"] = false;
					building = 0;
					//Issue attack?
					break;
				case SDL_BUTTON_MIDDLE:
					std::cout << "Mouse3 Released" << std::endl;
					keyDown["MOUSE3"] = false;
					break;
				case SDL_BUTTON_X1:
					std::cout << "Mouse4 Released" << std::endl;
					keyDown["MOUSE4"] = false;
					break;
				case SDL_BUTTON_X2:
					std::cout << "Mouse5 Released" << std::endl;
					keyDown["MOUSE5"] = false;
					break;
				}
				break;
			default:
				break;
		}
	}
}

bool GTDPlayer::isHolding(string key)
{
	return keyDown[key];
}

void GTDPlayer::queueSelection()
{
	selecting = true;
}

bool GTDPlayer::isSelecting()
{
	return selecting;
}


void GTDPlayer::endQueueSelection()
{
	selecting = false;

}

void GTDPlayer::queueBuilding()
{
	buildingQueued = true;
	building = false;
}

int GTDPlayer::getCurrentlySelectedBuilding()
{
	return currentlySelectedBuilding;
}


bool GTDPlayer::hasBuildingQueued()
{
	return buildingQueued;
}

void GTDPlayer::endQueueBuilding()
{
	buildingQueued = false;

}

bool GTDPlayer::isSelectingBuildLocation()
{
	return building;
}

int GTDPlayer::getOldMouseX()
{
	return oldMouseX;
}
int GTDPlayer::getOldMouseY()
{
	return oldMouseY;
}

int GTDPlayer::getMoney()
{
	return money;
}

void GTDPlayer::spend(int cost)
{
	money -= cost;
}

void GTDPlayer::earn(int bounty)
{
	money += bounty;
}