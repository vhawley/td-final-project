#include "GTDPlayer.h"

GTDPlayer::GTDPlayer()
{
	over = 0;
	lastkey = "NULL";
	mouseHold = false;
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
					lastkey = key;
				}
				
				break;
			case SDL_KEYUP:
				key = SDL_GetKeyName(e.key.keysym.sym);
				std::cout << "Key released: " << key << std::endl;
				lastkey = "NULL";
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
						mouseHold = true;
						break;
					case SDL_BUTTON_RIGHT:
						std::cout << "Mouse2 Pressed" << std::endl;
						break;
					case SDL_BUTTON_MIDDLE:
						std::cout << "Mouse3 Pressed" << std::endl;
						break;
					case SDL_BUTTON_X1:
						std::cout << "Mouse4 Pressed" << std::endl;
						break;
					case SDL_BUTTON_X2:
						std::cout << "Mouse5 Pressed" << std::endl;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
				case SDL_BUTTON_LEFT:
					std::cout << "Mouse1 Released" << std::endl;
					mouseHold = false;
					break;
				case SDL_BUTTON_RIGHT:
					std::cout << "Mouse2 Released" << std::endl;
					break;
				case SDL_BUTTON_MIDDLE:
					std::cout << "Mouse3 Released" << std::endl;
					break;
				case SDL_BUTTON_X1:
					std::cout << "Mouse4 Released" << std::endl;
					break;
				case SDL_BUTTON_X2:
					std::cout << "Mouse5 Released" << std::endl;
					break;
				}
				break;
			default:
				break;
		}
	}
}

bool GTDPlayer::isHoldingMouse()
{
	return mouseHold;
}

int GTDPlayer::getOldMouseX()
{
	return oldMouseX;
}
int GTDPlayer::getOldMouseY()
{
	return oldMouseY;
}