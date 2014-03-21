#include <iostream>
#include <string.h>
#include <SDL.h>

using namespace std;

class GTDUnit
{
public:
	GTDUnit();
private:
	int x;
	int y;
	SDL_Texture *text;
	string name;

};