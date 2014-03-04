#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>


class GTDPlayer
{
public:
	GTDPlayer();
	void processInput();
	int getOver();
private:
	SDL_Event e;
	const char *key;
	int over;
};