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

	screenX = 800; //initial screen position
	screenY = 400;

	SDL_Rect mouseRect;
	mouseRect.w = 8;
	mouseRect.h = 8;

	GTDTimer fps;
	fps.start();
	int frames = 0;

	while (!IsGameOver)
	{
		//draw some stuff
		SDL_RenderClear(renderer);

		
		map.draw(screenX, screenY, renderer);

		SDL_GetMouseState(&mouseRect.x, &mouseRect.y);

		mouseRect.x -= (mouseRect.w / 2); //Offset for center of square
		mouseRect.y -= (mouseRect.h / 2); //Offset for center of square

		//SDL_RenderCopy(renderer, textures.at(2), NULL, &mouseRect);
		SDL_RenderPresent(renderer);

		//handle input
		player.processInput();

		//recalculate
		
		SDL_GetMouseState(&mouseX, &mouseY);
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