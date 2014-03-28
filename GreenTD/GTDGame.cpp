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
	screenY = (map.getMapH() * map.getTileH() - S_HEIGHT) / 2;

	SDL_Rect mouseRect;


	GTDTimer fps;
	fps.start();
	int frames = 0;

	while (!IsGameOver)
	{
		//clear renderer
		SDL_RenderClear(renderer);

		//handle input
		player.processInput();

		//get mouse state
		SDL_GetMouseState(&mouseX, &mouseY);

		//Draw map and mouse rect if necessary
		map.draw(screenX, screenY, renderer);
		if (player.isHoldingMouse() && !player.isBuilding())
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 1);
			std::cout << "Should be drawing mouse rect..." << std::endl;
			mouseRect.x = std::min(mouseX,player.getOldMouseX()); //min to allow box selection in all directions
			mouseRect.y = std::min(mouseY,player.getOldMouseY());//min to allow box selection in all directions 
			mouseRect.w = std::abs(mouseX - player.getOldMouseX());
			mouseRect.h = std::abs(mouseY - player.getOldMouseY());
			std::cout << mouseRect.x << " " << mouseRect.y << " " << mouseRect.w << " " << mouseRect.h << std::endl;
			if (mouseRect.w > 0 && mouseRect.h > 0)
			{
				if (SDL_RenderDrawRect(renderer, &mouseRect))
				{
					std::cout << "ERROR drawing mouserect: " << SDL_GetError() << std::endl;
				}
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		}
		//Draw blue rect on current tile if building
		if (player.isBuilding())
		{
			int btileX = std::floor(mouseX / map.getTileW()) * map.getTileW();
			int btileY = std::floor(mouseY / map.getTileH()) * map.getTileH();
			int btileDeltaX = screenX % map.getTileW();
			int btileDeltaY = screenY % map.getTileH();
			SDL_Rect buildingRect;
			buildingRect.x = btileX-btileDeltaX;
			buildingRect.y = btileY-btileDeltaY;
			buildingRect.w = map.getTileW(); //Times building width when building is implemented
			buildingRect.h = map.getTileH(); //times building height when building is implemented
			SDL_SetRenderDrawColor(renderer, 0, 0, 255, 1);
			SDL_RenderFillRect(renderer, &buildingRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
		}

		//Present Contents to screen
		SDL_RenderPresent(renderer);

		//Moves screen position based on mouse position
		if (mouseX < 5)
		{
			screenX = std::max(0, screenX - 2);
		}
		if (mouseY < 5)
		{
			screenY = std::max(0, screenY - 2);
		}
		if (mouseX > S_WIDTH - 5)
		{
			screenX = std::min(map.getTileW() * map.getMapW() - S_WIDTH, screenX + 2);
		}
		if (mouseY > S_HEIGHT - 5)
		{
			screenY = std::min(map.getTileH() * map.getMapH() - S_HEIGHT, screenY + 2);
		}
		if (player.getOver())
		{
			IsGameOver = 1;
		}
		if (debug)
		{
			frames++;
			if (fps.getTicks() > 1000)
			{
				std::cout << "FPS : " << frames << std::endl;
				std::cout << "screen: (" << screenX << ", " << screenY << ")" << std::endl;
				frames = 0;
				fps.start();
			}
		}
	}
	quit();
}

void GTDGame::quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}