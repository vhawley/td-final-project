#include "GTDGame.h"

#define S_WIDTH 1280
#define S_HEIGHT 720
#define SYS_FPS 60

GTDGame::GTDGame()
{
	IsGameOver = 0;
}

bool GTDGame::init()
{
	//Create Window	
	window = SDL_CreateWindow(
		"Green TD",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		S_WIDTH,
		S_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED);

	//Create Renderer
	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);// | SDL_RENDERER_PRESENTVSYNC);

	if (!(renderer && window && map.init("assets/dat/greentd.dat", renderer)))
	{
		return false;
	}
	return true;
}

void GTDGame::run()
{
	//Default color = black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	screenX = (map.getMapW() * map.getTileW() - S_WIDTH) / 2; //initial screen position
	screenY = (map.getMapH() * map.getTileH() - S_HEIGHT) / 2 - 500;

	GTDRect *testRect = new GTDRect(1800, 100, 400, 150);
	map.spawnLevel(new GTDLevel("./assets/dat/level1.dat", testRect, NULL));


	GTDTimer tickTimer;
	tickTimer.start();
	int timeElapsed = 0;

	while (!IsGameOver)
	{
		//Get time elapsed 
		timeElapsed = tickTimer.getTicks();
		tickTimer.start();

		//clear renderer
		SDL_RenderClear(renderer);

		//handle input
		player.processInput();

		//Step units
		map.stepUnits(timeElapsed);

		//Add building to map if queued
		buildPlayerBuilding();

		//get mouse state
		SDL_GetMouseState(&mouseX, &mouseY);

		//Draw map
		map.draw(screenX, screenY, renderer);

		//Draw mouse rect if necessary
		drawBoxSelection();

		//Draw blue rect on current tile if building
		drawBuildBox();

		//Draw blue rect on current tile if building
		drawBuildBox();

		//Present Contents to screen
		SDL_RenderPresent(renderer);

		//Moves screen position based on mouse position
		moveScreen();

		if (player.getOver())
		{
			IsGameOver = 1;
		}

		if (debug)
		{
			if (timeElapsed == 0)
			{
				std::cout << "timeelapsed = 0" << std::endl;
			}
			else
			{
				double fps = 1000 / (double)timeElapsed;
				std::cout << fps << " fps." << std::endl;
			}
		}
	}
	tickTimer.stop();
	quit();
}

void GTDGame::quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void GTDGame::moveScreen()
{
	if (mouseX < 5 || player.isHolding("Left"))
	{
		screenX = std::max(0, screenX - 2);
	}
	if (mouseY < 5 || player.isHolding("Up"))
	{
		screenY = std::max(0, screenY - 2);
	}
	if (mouseX > S_WIDTH - 5 || player.isHolding("Right"))
	{
		screenX = std::min(map.getTileW() * map.getMapW() - S_WIDTH, screenX + 2);
	}
	if (mouseY > S_HEIGHT - 5 || player.isHolding("Down"))
	{
		screenY = std::min(map.getTileH() * map.getMapH() - S_HEIGHT, screenY + 2);
	}
}

void GTDGame::drawBoxSelection()
{
	SDL_Rect mouseRect;
	if (player.isHolding("MOUSE1") && !player.isSelectingBuildLocation())
	{
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 1);
		mouseRect.x = std::min(mouseX, player.getOldMouseX()); //min to allow box selection in all directions
		mouseRect.y = std::min(mouseY, player.getOldMouseY());//min to allow box selection in all directions 
		mouseRect.w = std::abs(mouseX - player.getOldMouseX());
		mouseRect.h = std::abs(mouseY - player.getOldMouseY());
		if (debug)
		{
			std::cout << "Should be drawing mouse rect..." << std::endl;
			std::cout << mouseRect.x << " " << mouseRect.y << " " << mouseRect.w << " " << mouseRect.h << std::endl;
		}
		if (mouseRect.w > 0 && mouseRect.h > 0)
		{
			if (SDL_RenderDrawRect(renderer, &mouseRect))
			{
				std::cout << "ERROR drawing mouserect: " << SDL_GetError() << std::endl;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	}
}

void GTDGame::drawBuildBox()
{
	if (player.isSelectingBuildLocation())
	{
		int btileX = ( mouseX / map.getTileW() ) * map.getTileW();
		int btileY = ( mouseY / map.getTileH() ) * map.getTileH();

		int btileDeltaX = screenX % map.getTileW();
		int btileDeltaY = screenY % map.getTileH();

		SDL_Rect buildingRect;
		buildingRect.x = btileX - btileDeltaX;
		buildingRect.y = btileY - btileDeltaY;
		buildingRect.w = map.getTileW(); //Times building width when building is implemented
		buildingRect.h = map.getTileH(); //times building height when building is implemented

		int bX = btileX - btileDeltaX + screenX;
		int bY = btileY - btileDeltaY + screenY;

		int mapIndexX = (btileX + screenX) / map.getTileW();
		int mapIndexY = (btileY + screenY) / map.getTileH();
		
		if (map.spaceIsBuildable(mapIndexY, mapIndexX))
			{
			GTDUnit::GTDBuilding btype = static_cast<GTDUnit::GTDBuilding>(player.getCurrentlySelectedBuilding());
			int cost = GTDUnit::getCost(btype);
			if (player.getMoney() >= cost)
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 1);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 1);
			}
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
		}

		if (debug)
		{
			std::cout << "(" << mapIndexX << ", " << mapIndexY << ")" << std::endl;
		}
		
		SDL_RenderFillRect(renderer, &buildingRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	}
}

void GTDGame::drawBoxOnSelectedUnits()
{


}

void GTDGame::buildPlayerBuilding()
{
	if (player.hasBuildingQueued())
	{
		GTDUnit::GTDBuilding btype = static_cast<GTDUnit::GTDBuilding>(player.getCurrentlySelectedBuilding());
		int cost = GTDUnit::getCost(btype);
		if (player.getMoney() >= cost)
		{
			int btileX = (mouseX / map.getTileW()) * map.getTileW();
			int btileY = (mouseY / map.getTileH()) * map.getTileH();

			int btileDeltaX = screenX % map.getTileW();
			int btileDeltaY = screenY % map.getTileH();

			int bX = btileX - btileDeltaX + screenX + GTDUnit::getCollision(btype) / 2;
			int bY = btileY - btileDeltaY + screenY + GTDUnit::getCollision(btype) / 2;

			int mapIndexX = (btileX + screenX) / map.getTileW();
			int mapIndexY = (btileY + screenY) / map.getTileH();

			if (map.spaceIsBuildable(mapIndexY, mapIndexX))
			{
				player.spend(cost);
				std::cout << "You spent " << cost << " money on a " << btype << " tower.  You now have " << player.getMoney() << " money." << std::endl;
				map.addUnit(new GTDUnit(btype, &player, bX, bY, renderer));
			}
			else
			{
				std::cout << "You only have " << player.getMoney() << " money. A " << btype << " tower costs " << cost << " money.";
			}
			player.endQueueBuilding();
		}
	}
}

