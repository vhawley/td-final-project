#include "GTDMap.h"

GTDMap::GTDMap()
{
	initialized = 0;
}

GTDMap::GTDMap(char *filename, SDL_Renderer *renderer)
{	
	initialized = 0;
	init(filename, renderer);
}

bool GTDMap::init(char *filename, SDL_Renderer *renderer)
{
	if (!initialized)
	{
		std::ifstream mapFile;
		mapFile.open(filename);
		if (!mapFile)
		{
			std::cout << "Could not open file: " << filename << std::endl;
			return 0;
		}
		int wCounter = 0;
		bool haveWidth = false;
		int hCounter = 0;
		while (mapFile >> a)
		{
			if (!haveWidth)
			{
				wCounter++;
			}
			if (mapFile.peek() == '\n')
			{
				haveWidth = true;
				hCounter++;
			}
		}
		std::cout << "Width / height = " << wCounter << " / " << hCounter << std::endl;
		mapBoard = new int *[hCounter];
		for (int i = 0; i < hCounter; i++)
		{
			mapBoard[i] = new int[wCounter];
		}
		mapFile.close();
		mapFile.open(filename);
		for (int i = 0; i < hCounter; i++)
		{
			for (int j = 0; j < wCounter; j++)
			{
				mapFile >> a;
				mapBoard[i][j] = a;
			}
		}
		mapW = wCounter;
		mapH = hCounter;
		tileW = 32;
		tileH = 32;
		loadTextures(renderer);
		initialized = 1;
		printMapBoard();
		return 1;
	}
	else
	{
		std::cout << "Map has already been initialized" << std::endl;
	}
	return 0;
}


int GTDMap::getMapBoard(int m, int n)
{
	if (m > mapH || n > mapW)
	{
		return -1;
	}
	else
	{
		return mapBoard[m][n];
	}
}
void GTDMap::setMapBoard(int m, int n, unsigned int value)
{
	if (m > mapH || n > mapW)
	{
		return;
	}
	else
	{
		mapBoard[m][n] = value;
	}
}

void GTDMap::printMapBoard()
{
	if (!initialized)
	{
		std::cout << "Map has not been initialized" << std::endl;
	}
	else
	{
		for (int i = 0; i < mapH; i++)
		{
			for (int j = 0; j < mapW; j++)
			{
				std::cout << mapBoard[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
}

bool GTDMap::loadTextures(SDL_Renderer *renderer)
{
	//Load grass bitmap
	SDL_Surface *grassBMP = SDL_LoadBMP("./assets/bmp/grass.bmp");

	//Create grass texture
	textures.push_back(SDL_CreateTextureFromSurface(renderer, grassBMP));

	SDL_FreeSurface(grassBMP);

	//Load sidewalk bitmap
	SDL_Surface *sidewalk = SDL_LoadBMP("./assets/bmp/sidewalk.bmp");

	//Create rorange texture
	textures.push_back(SDL_CreateTextureFromSurface(renderer, sidewalk));

	SDL_FreeSurface(sidewalk);

	//Load blue bitmap
	SDL_Surface *blueBMP = SDL_LoadBMP("./assets/bmp/blue.bmp");

	//Create blue texture
	textures.push_back(SDL_CreateTextureFromSurface(renderer, blueBMP));

	SDL_FreeSurface(blueBMP);

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		if (!textures.at(i))
		{
			return false;
		}
	}
	return true;
}

void GTDMap::draw(int x, int y, SDL_Renderer *renderer)
{
	SDL_Rect tileRect;
	tileRect.x = 0;
	tileRect.y = 0;
	tileRect.w = tileW;
	tileRect.h = tileH;

	int tileX = x / tileRect.w;
	int tileY = y / tileRect.h;

	int tileDeltaX = x % tileRect.w;
	int tileDeltaY = y % tileRect.h;

	int textcount = 0;
	tileRect.y = -tileDeltaY;
	for (int i = std::max(tileY, 0); i < std::min(tileY + 48, getMapH()); i++)
	{
		tileRect.x = -tileDeltaX;
		for (int j = std::max(tileX, 0); j < std::min(tileX + 82, getMapW()); j++)
		{
			SDL_RenderCopy(renderer, textures.at(getMapBoard(i, j)), NULL, &tileRect);
			tileRect.x += tileRect.w;
			textcount++;
		}
		tileRect.y += tileRect.h;
	}
}

int GTDMap::getMapW()
{
	return mapW;
}
int GTDMap::getMapH()
{
	return mapH;
}

int GTDMap::getTileW()
{
	return tileW;
}
int GTDMap::getTileH()
{
	return tileH;
}

bool rectContainsUnit(GTDRect rect, GTDUnit unit)
{
	if (unit.getPosX() >= rect.getX() &&
		unit.getPosX() <= (rect.getX() + rect.getW()) &&
		unit.getPosY() >= rect.getY() &&
		unit.getPosY() >= (rect.getY() + rect.getH()))
	{
		return true;
	}
	return false;
}

GTDMap::~GTDMap()
{
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		SDL_DestroyTexture(textures.at(i));
	}
	free(mapBoard);
}
