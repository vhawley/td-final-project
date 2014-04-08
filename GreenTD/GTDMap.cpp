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

		//Initial read to count lines
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

		//2d array based on counts from last read
		for (int i = 0; i < hCounter; i++)
		{
			mapBoard[i] = new int[wCounter];
		}
		mapFile.close();
		mapFile.open(filename);

		//Actually load in contents to int array
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
		if (!loadTextures(renderer))
		{
			return 0;
		}
		initialized = 1;
		//printMapBoard();
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
	for (int i = 1; i < 355; i++)
	{
		char filepath[50];
		//Create filepath from i
		sprintf_s(filepath, "./assets/terrain/%d.bmp", i);
		//Load BMP from filepath
		SDL_Surface *BMP = SDL_LoadBMP(filepath);
		//Add BMP to textures vector
		textures.push_back(SDL_CreateTextureFromSurface(renderer, BMP));

		SDL_FreeSurface(BMP);
	}

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

	//Loops through specific position in 2D array to draw only the textures needed
	for (int i = std::max(tileY, 0); i < std::min(tileY + 24, getMapH()); i++) //Hardcoded tile length for now...
	{
		tileRect.x = -tileDeltaX;
		for (int j = std::max(tileX, 0); j < std::min(tileX + 41, getMapW()); j++) //Hardcoded tile length for now... should come up with better way
		{
			//Render texture based on index in array
			int textureI = getMapBoard(i, j) - 1;
			if (textureI < 1)
			{
				textureI = 1;
			}
			else if (textureI > 355)
			{
				textureI = 355;
			}
			SDL_RenderCopy(renderer, textures.at(textureI), NULL, &tileRect);
			tileRect.x += tileRect.w;
			textcount++;
		}
		tileRect.y += tileRect.h;
	}
	for (unsigned int uindex = 0; uindex < units.size(); uindex++)
	{
		if (units.at(uindex).getPosX() >(x - units.at(uindex).getCollision() / 2) &&
			units.at(uindex).getPosX() < (x + 1280 + units.at(uindex).getCollision() / 2) &&
			units.at(uindex).getPosY() > (y - units.at(uindex).getCollision() / 2) &&
			units.at(uindex).getPosY() < (y + 720 + units.at(uindex).getCollision() / 2)) //only draw if in screen bounds
		{
			SDL_Rect unitRect;
			unitRect.x = units.at(uindex).getPosX() - x;
			unitRect.y = units.at(uindex).getPosY() - y;
			unitRect.w = units.at(uindex).getCollision();
			unitRect.h = units.at(uindex).getCollision();
			SDL_RenderCopy(renderer, units.at(uindex).getTexture(), NULL, &unitRect);
		}
	}
}

void GTDMap::addUnit(GTDUnit *u, GTDPlayer *p)
{
	units.push_back(*u);
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
	delete []mapBoard;
}
