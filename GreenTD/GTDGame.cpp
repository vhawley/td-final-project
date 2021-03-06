#include "GTDGame.h"

#define S_WIDTH 1280
#define S_HEIGHT 720
#define SYS_FPS 60

GTDGame::GTDGame()
{
	IsGameOver = 0;
	UIheight = 24;
	defColor.r = 0;
	defColor.g = 0;
	defColor.b = 0;
	defColor.a = 1;
	totalLevels = 10;
	currentLevel = 0;
}
GTDGame::~GTDGame()
{
	TTF_CloseFont(font);
	SDL_Quit();
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
		SDL_WINDOW_FULLSCREEN | SDL_WINDOW_INPUT_GRABBED);

	//Create Renderer
	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);// | SDL_RENDERER_PRESENTVSYNC);

	if (!(renderer && window && map.init("assets/dat/greentd.dat", renderer)))
	{
		return false;
	}
	if (TTF_Init() == -1)
	{
		std::cout << "TTF_Init error: " << TTF_GetError() << std::endl;
		return false;
	}
	font = TTF_OpenFont("HARNGTON.TTF", 19);
	if (!font)
	{
		std::cout << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
		return false;
	}
	if (!IMG_Init(IMG_INIT_PNG))
	{
		printf( "IMG_Init Error: %s\n", IMG_GetError() );
	}
	return true;
}

void GTDGame::run()
{
	screenX = 0;
	screenY = 0;
	//screenX = (map.getMapW() * map.getTileW() - S_WIDTH) / 2; //initial screen position = middle. commented it because units now start in the top left
	//screenY = (map.getMapH() * map.getTileH() - S_HEIGHT) / 2 - 500;
	currentState = PREGAME;

	if (debug)
	{
		timeTilSpawn = 10;
		player.earn(1000);
	}
	else
	{
		timeTilSpawn = 30;
	}

	GTDTimer tickTimer;
	tickTimer.start();
	int timeElapsed = 0;

	while (!IsGameOver)
	{
		//Set default color
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		//Get time elapsed 
		timeElapsed = tickTimer.getTicks();
		tickTimer.start();

		//clear renderer
		SDL_RenderClear(renderer);

		//update game state
		updateGameState(timeElapsed);

		//update status message
		updateStatusMessage();

		//get mouse state
		SDL_GetMouseState(&mouseX, &mouseY);

		//handle input
		player.processInput();

		//select units
		performSelection();

		//queue attacks
		queueAttacks();

		//Step units
		map.stepUnits(timeElapsed);

		//Add building to map if queued
		buildPlayerBuilding();

		//Draw map
		map.draw(screenX, screenY, renderer);

		//Draw mouse rect if necessary
		drawBoxSelection();

		//Draw blue rect on current tile if building
		drawBuildBox();

		//Draw UI
		drawUI();

		//Present Contents to screen
		SDL_RenderPresent(renderer);

		//Moves screen position based on mouse position
		moveScreen(timeElapsed);

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
}

void GTDGame::moveScreen(int timeElapsed) //move screen independent of frame rate
{
	if (mouseX < 5 || player.isHolding("Left"))
	{
		screenX = std::max(0, screenX - (2 * timeElapsed)); //checks if game is at left most possible
	}
	if (mouseY < 5 || player.isHolding("Up"))
	{
		screenY = std::max(0, screenY - (2 * timeElapsed)); //checks if game is at top most possible
	}
	if (mouseX > S_WIDTH - 5 || player.isHolding("Right")) 
	{
		screenX = std::min(map.getTileW() * map.getMapW() - S_WIDTH, screenX + (2 * timeElapsed));//checks if game is at right most possible
	}
	if (mouseY > S_HEIGHT - 5 || player.isHolding("Down")) 
	{
		screenY = std::min(map.getTileH() * map.getMapH() - S_HEIGHT + UIheight, screenY + (2 * timeElapsed));//checks if game is at right most possible
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
			std::cout << "Should be drawing mouse rect at GTD coordinates:" << std::endl;
			std::cout << mouseRect.x + screenX << " " << mouseRect.y + screenY << " " << mouseRect.w << " " << mouseRect.h << std::endl;
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
		GTDUnit::GTDBuilding btype = static_cast<GTDUnit::GTDBuilding>(player.getCurrentlySelectedBuilding());
		int btileX = ( mouseX / map.getTileW() ) * map.getTileW(); //X of tile mouse is on with respect to screen location
		int btileY = ( mouseY / map.getTileH() ) * map.getTileH();//Y of tile mouse is on with respect to screen location

		int btileDeltaX = screenX % map.getTileW(); //X tile mouse offset
		int btileDeltaY = screenY % map.getTileH(); //Y tile mouse offset

		SDL_Rect buildingRect;
		buildingRect.x = btileX - btileDeltaX; //subtract from delta to get X of top left corner ot tile
		buildingRect.y = btileY - btileDeltaY; //subtract from delta to get Y of top left corner ot tile
		buildingRect.w = GTDUnit::getCollision(btype); //building width
		buildingRect.h = GTDUnit::getCollision(btype); //building height THEY ARE SQUARES

		int bX = btileX - btileDeltaX + screenX; //X of building in game terms
		int bY = btileY - btileDeltaY + screenY; //Y of building in game terms

		int mapIndexX = (btileX + screenX) / map.getTileW(); //map tile X to check
		int mapIndexY = (btileY + screenY) / map.getTileH(); //map tile Y to check
		
		if (map.spaceIsBuildable(mapIndexY, mapIndexX)) //checks terrain and existance of other buildings at location
			{
			int cost = GTDUnit::getCost(btype);
			if (player.getMoney() >= cost)
			{
				SDL_SetRenderDrawColor(renderer, 0, 0, 255, 1); //blue if enough money + space is buildable
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 255, 0, 1); //yellow if not enough money but space is buildable
			}
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1); //red if space isn't buildable regardless of cost
		}

		if (debug)
		{
			std::cout << "(" << mapIndexX << ", " << mapIndexY << ")" << std::endl;
		}
		
		SDL_RenderFillRect(renderer, &buildingRect);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
	}
}

void GTDGame::buildPlayerBuilding()
{
	if (player.hasBuildingQueued())
	{
		GTDUnit::GTDBuilding btype = static_cast<GTDUnit::GTDBuilding>(player.getCurrentlySelectedBuilding());
		int cost = GTDUnit::getCost(btype);
		if (player.getMoney() >= cost)
		{
			int btileX = (mouseX / map.getTileW()) * map.getTileW(); //next 12 lines: same as last function but actually build the tower this time
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
		}
		player.endQueueBuilding();
	}
}

void GTDGame::performSelection()
{
	if (player.isSelecting())
	{
		std::cout << "Player is selecting... " << std::endl;
		GTDRect *selectRect = new GTDRect(); //select region in game coordinates, not screen coordinates
		int selectX = screenX + std::min(mouseX, player.getOldMouseX());
		int selectY = screenY + std::min(mouseY, player.getOldMouseY());
		int selectW = std::abs(mouseX - player.getOldMouseX());
		int selectH = std::abs(mouseY - player.getOldMouseY());
		if (selectW <= 32 && selectH <= 32) //adjust select rect when small box (click)
		{
			selectX -= map.getTileW() / 2;
			selectY -= map.getTileH() / 2;
			selectW = map.getTileW();
			selectH = map.getTileH();
		}
		selectRect->setX(selectX); //min to allow box selection in all directions
		selectRect->setY(selectY);//min to allow box selection in all directions 
		selectRect->setW(selectW);
		selectRect->setH(selectH);
		map.selectUnitsInRect(selectRect);
		player.endQueueSelection();
	}
}

void GTDGame::drawUI()
{
	SDL_Rect UIRect; //rect along bottom of game for UI
	UIRect.x = 0;
	UIRect.y = S_HEIGHT - UIheight;
	UIRect.w = S_WIDTH;
	UIRect.h = UIheight;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &UIRect);
	SDL_Color textColor; //white text
	textColor.r = 255;
	textColor.g = 255;
	textColor.b = 255;
	textColor.a = 1;
	char kills[8] = "Kills: ";
	char killCount[5];
	char lives[8] = "Lives: ";
	char livesCount[5];
	char level[8] = "Level: ";
	char levelCount[5];
	char shop[75];
	char timer[19] = "Time Until Spawn: ";
	char timerCount[6];
	char money[8] = "Money: ";
	char moneyCount[10];
	sprintf_s(killCount, "%d", player.getKills());
	sprintf_s(livesCount, "%d", map.getLives());
	sprintf_s(levelCount, "%d", currentLevel);
	GTDUnit::GTDBuilding btype = static_cast<GTDUnit::GTDBuilding>(player.getCurrentlySelectedBuilding());
	string bname = GTDUnit::getName(btype);
	int cost = GTDUnit::getCost(btype);
	sprintf_s(shop, "Queued: %s. Costs: %d", bname.c_str(), cost);
	sprintf_s(timerCount, "%.1lf", timeTilSpawn);
	sprintf_s(moneyCount, "%d", player.getMoney());

	//create surfaces from text
	SDL_Surface *killsTextSurface = TTF_RenderText_Solid(font, kills, textColor);
	SDL_Surface *killsCountSurface = TTF_RenderText_Solid(font, killCount, textColor);
	SDL_Surface *livesTextSurface = TTF_RenderText_Solid(font, lives, textColor);
	SDL_Surface *livesCountSurface = TTF_RenderText_Solid(font, livesCount, textColor);
	SDL_Surface *levelTextSurface = TTF_RenderText_Solid(font, level, textColor);
	SDL_Surface *levelCountSurface = TTF_RenderText_Solid(font, levelCount, textColor);
	SDL_Surface *timerTextSurface = TTF_RenderText_Solid(font, timer, textColor);
	SDL_Surface *timerCountSurface = TTF_RenderText_Solid(font, timerCount, textColor);

	SDL_Surface *queuedTextSurface = TTF_RenderText_Solid(font, shop, textColor);

	SDL_Surface *statusTextSurface = TTF_RenderText_Solid(font, statusMessage.c_str(), textColor);
	SDL_Surface *statusAuxSurface = TTF_RenderText_Solid(font, statusAux.c_str(), textColor);
	SDL_Surface *moneyTextSurface = TTF_RenderText_Solid(font, money, textColor);
	SDL_Surface *moneyCountSurface = TTF_RenderText_Solid(font, moneyCount, textColor);

	//check for valid surfaces
	if (!killsTextSurface || !killsCountSurface || !livesTextSurface || !livesCountSurface || !levelTextSurface || !levelCountSurface || !timerTextSurface || !timerCountSurface || !queuedTextSurface || !moneyTextSurface || !moneyCountSurface || !statusTextSurface || !statusAuxSurface)
	{
		std::cout << "Can't create surface: " << TTF_GetError() << std::endl;
	}
	else
	{
		//create textures from surface
		SDL_Texture *killsTextTexture = SDL_CreateTextureFromSurface(renderer, killsTextSurface);
		SDL_Texture *killCountTexture = SDL_CreateTextureFromSurface(renderer, killsCountSurface);
		SDL_Texture *livesTextTexture = SDL_CreateTextureFromSurface(renderer, livesTextSurface);
		SDL_Texture *livesCountTexture = SDL_CreateTextureFromSurface(renderer, livesCountSurface);
		SDL_Texture *levelTextTexture = SDL_CreateTextureFromSurface(renderer, levelTextSurface);
		SDL_Texture *levelCountTexture = SDL_CreateTextureFromSurface(renderer, levelCountSurface);
		SDL_Texture *timerTextTexture = SDL_CreateTextureFromSurface(renderer, timerTextSurface);
		SDL_Texture *timerCountTexture = SDL_CreateTextureFromSurface(renderer, timerCountSurface);

		SDL_Texture *queuedTextTexture = SDL_CreateTextureFromSurface(renderer, queuedTextSurface);

		SDL_Texture *statusTextTexture = SDL_CreateTextureFromSurface(renderer, statusTextSurface);
		SDL_Texture *statusAuxTexture = SDL_CreateTextureFromSurface(renderer, statusAuxSurface);
		SDL_Texture *moneyTextTexture = SDL_CreateTextureFromSurface(renderer, moneyTextSurface);
		SDL_Texture *moneyCountTexture = SDL_CreateTextureFromSurface(renderer, moneyCountSurface);

		SDL_FreeSurface(killsTextSurface);
		SDL_FreeSurface(killsCountSurface);
		SDL_FreeSurface(livesTextSurface);
		SDL_FreeSurface(livesCountSurface);
		SDL_FreeSurface(levelTextSurface);
		SDL_FreeSurface(levelCountSurface);
		SDL_FreeSurface(timerTextSurface);
		SDL_FreeSurface(timerCountSurface);

		SDL_FreeSurface(queuedTextSurface);

		SDL_FreeSurface(statusTextSurface);
		SDL_FreeSurface(statusAuxSurface);
		SDL_FreeSurface(moneyTextSurface);
		SDL_FreeSurface(moneyCountSurface);

		//define text regions
		SDL_Rect killsTextRect;
		killsTextRect.x = 4;
		killsTextRect.y = S_HEIGHT - UIheight;
		killsTextRect.w = strlen(kills) * 8;
		killsTextRect.h = UIheight;
		SDL_Rect killCountRect;
		killCountRect.x = killsTextRect.x + killsTextRect.w + 2;
		killCountRect.y = S_HEIGHT - UIheight;
		killCountRect.w = strlen(killCount) * 8;
		killCountRect.h = UIheight;
		SDL_Rect livesTextRect;
		livesTextRect.x = killCountRect.x + killCountRect.w + 20;
		livesTextRect.y = S_HEIGHT - UIheight;
		livesTextRect.w = strlen(lives) * 9;
		livesTextRect.h = UIheight;
		SDL_Rect livesCountRect;
		livesCountRect.x = livesTextRect.x + livesTextRect.w + 2;
		livesCountRect.y = S_HEIGHT - UIheight;
		livesCountRect.w = strlen(livesCount) * 9;
		livesCountRect.h = UIheight;
		SDL_Rect levelTextRect;
		levelTextRect.x = livesCountRect.x + livesCountRect.w + 20;
		levelTextRect.y = S_HEIGHT - UIheight;
		levelTextRect.w = strlen(level) * 9;
		levelTextRect.h = UIheight;
		SDL_Rect levelCountRect;
		levelCountRect.x = levelTextRect.x + levelTextRect.w + 2;
		levelCountRect.y = S_HEIGHT - UIheight;
		levelCountRect.w = strlen(levelCount) * 9;
		levelCountRect.h = UIheight;
		SDL_Rect timerTextRect;
		timerTextRect.x = levelCountRect.x + levelCountRect.w + 20;
		timerTextRect.y = S_HEIGHT - UIheight;
		timerTextRect.w = strlen(timer) * 9;
		timerTextRect.h = UIheight;
		SDL_Rect timerCountRect;
		timerCountRect.x = timerTextRect.x + timerTextRect.w + 2;
		timerCountRect.y = S_HEIGHT - UIheight;
		timerCountRect.w = strlen(timerCount) * 9;
		timerCountRect.h = UIheight;
		SDL_Rect queuedTextRect;
		queuedTextRect.x = timerCountRect.x + timerCountRect.w + 20;
		queuedTextRect.y = S_HEIGHT - UIheight;
		queuedTextRect.w = strlen(shop) * 9;
		queuedTextRect.h = UIheight;


		SDL_Rect moneyCountRect;
		moneyCountRect.w = strlen(moneyCount) * 10;
		moneyCountRect.x = S_WIDTH - moneyCountRect.w - 4;
		moneyCountRect.y = S_HEIGHT - UIheight;
		moneyCountRect.h = UIheight;
		SDL_Rect moneyTextRect;
		moneyTextRect.w = strlen(money) * 10;
		moneyTextRect.x = moneyCountRect.x - moneyTextRect.w - 4;
		moneyTextRect.y = S_HEIGHT - UIheight;
		moneyTextRect.h = UIheight;
		SDL_Rect statusAuxRect;
		statusAuxRect.w = strlen(statusAux.c_str()) * 8;
		statusAuxRect.x = moneyTextRect.x - statusAuxRect.w - 24;
		statusAuxRect.y = S_HEIGHT - UIheight;
		statusAuxRect.h = UIheight;
		SDL_Rect statusTextRect;
		statusTextRect.w = strlen(statusMessage.c_str()) * 10;
		statusTextRect.x = statusAuxRect.x - statusTextRect.w - 4;
		statusTextRect.y = S_HEIGHT - UIheight;
		statusTextRect.h = UIheight;

		SDL_RenderCopy(renderer, killsTextTexture, NULL, &killsTextRect);
		SDL_RenderCopy(renderer, killCountTexture, NULL, &killCountRect);
		SDL_RenderCopy(renderer, livesTextTexture, NULL, &livesTextRect);
		SDL_RenderCopy(renderer, livesCountTexture, NULL, &livesCountRect);
		SDL_RenderCopy(renderer, levelTextTexture, NULL, &levelTextRect);
		SDL_RenderCopy(renderer, levelCountTexture, NULL, &levelCountRect);
		SDL_RenderCopy(renderer, timerTextTexture, NULL, &timerTextRect);
		SDL_RenderCopy(renderer, timerCountTexture, NULL, &timerCountRect);

		SDL_RenderCopy(renderer, queuedTextTexture, NULL, &queuedTextRect);

		SDL_RenderCopy(renderer, statusTextTexture, NULL, &statusTextRect);
		SDL_RenderCopy(renderer, statusAuxTexture, NULL, &statusAuxRect);
		SDL_RenderCopy(renderer, moneyTextTexture, NULL, &moneyTextRect);
		SDL_RenderCopy(renderer, moneyCountTexture, NULL, &moneyCountRect);

		//kill textures when rendererd
		SDL_DestroyTexture(killsTextTexture);
		SDL_DestroyTexture(killCountTexture);
		SDL_DestroyTexture(livesTextTexture);
		SDL_DestroyTexture(livesCountTexture);
		SDL_DestroyTexture(levelTextTexture);
		SDL_DestroyTexture(levelCountTexture);
		SDL_DestroyTexture(timerTextTexture);
		SDL_DestroyTexture(timerCountTexture);
		SDL_DestroyTexture(queuedTextTexture);
		SDL_DestroyTexture(statusTextTexture);
		SDL_DestroyTexture(statusAuxTexture);
		SDL_DestroyTexture(moneyTextTexture);
		SDL_DestroyTexture(moneyCountTexture);
	}
}

void GTDGame::updateGameState(int timeElapsed)
{
	switch (currentState)
	{
	case PREGAME:
		if (timeTilSpawn <= 0)
		{
			timeTilSpawn = 0;
			currentLevel++;
			GTDRect *spawnRect = new GTDRect(128, 112, 190, 180);
			char levelfile[40];
			sprintf_s(levelfile, "./assets/dat/level%d.dat", currentLevel);
			map.spawnLevel(new GTDLevel(levelfile, spawnRect));
			currentState = WAVEINPROGRESS;
		}
		else
		{
			timeTilSpawn -= (double)timeElapsed / 1000;
		}
		break;
	case WAVEINPROGRESS:
		numRemaining = map.getNumWaveUnitsOnMap();
		if (map.getLives() <= 0)
		{
			currentState = GAMELOST;
		}
		else if (numRemaining <= 0)
		{
			map.removeUnitsNotOnMap(); //for memory purposes
			if (currentLevel >= totalLevels)
			{
				currentState = GAMEWON;
			}
			else
			{
				timeTilSpawn = 20;
				player.earn(75 + 50 * currentLevel);
				currentState = WAVECOMPLETE;
			}
		}
		
		break;
	case WAVECOMPLETE: //does the same thing as PREGAME right now.... but if i want to change what happens in between levels later i will want this
		if (timeTilSpawn <= 0)
		{
			timeTilSpawn = 0;
			currentLevel++;
			GTDRect *spawnRect = new GTDRect(116, 110, 210, 170);
			char levelfile[40];
			sprintf_s(levelfile, "./assets/dat/level%d.dat", currentLevel);
			map.spawnLevel(new GTDLevel(levelfile, spawnRect));
			currentState = WAVEINPROGRESS;
		}
		else
		{
			timeTilSpawn -= (double)timeElapsed / 1000;
		}
		break;
	case GAMELOST:
		//gg
		break;
	case GAMEWON:
		//gg
		break;
	default:
		break;
	}
}

void GTDGame::updateStatusMessage() //updates dynamic text based on current player situation
{
	switch (currentState)
	{
	case GAMELOST:
		statusMessage = "Game Over! You lost!";
		statusAux = " ";
		break;
	case GAMEWON:
		statusMessage = "You won!";
		statusAux = " ";
		break;
	default:
		GTDUnit *selectedUnit = map.getSelectedUnit();
		if (selectedUnit != NULL) //returns null if no or multiple units selected.... cant show text for multiple units
		{
			statusMessage = selectedUnit->getName();
			if (selectedUnit->isWaveUnit()) //show health if wave unit
			{
				int health = selectedUnit->getHealth();
				int maxHealth = selectedUnit->getMaxHealth();
				char temp[24];
				sprintf_s(temp, "health: %d / %d", health, maxHealth);
				statusAux = temp;
			}
			else if (selectedUnit->isBuilding()) //show building description
			{
				
				switch (selectedUnit->getBuildingType())
				{
				case GTDUnit::GTDBuilding::SPEEDASSIST: //describe effects of speed assist
					statusAux = ": +25% speed for nearby towers";
					break;
				case GTDUnit::GTDBuilding::DMGASSIST: //describe effects of damage assist
					statusAux = ": +50% damage for nearby towers";
					break;
				default: //show attack damage range
					char temp[40];
					int lowatk = selectedUnit->getAttackDMG() - selectedUnit->getAttackDMGRange();
					int highatk = selectedUnit->getAttackDMG() + selectedUnit->getAttackDMGRange();
					sprintf_s(temp, "attack: %d - %d", lowatk, highatk);
					statusAux = temp;
					break;
				}
			}
			else
			{
				//shouldnt get here units are either buildings or wave units
				statusAux = " ";
			}
		}
		else
		{
			statusMessage = " ";
			statusAux = " ";
		}
		break;
	}
	
}

void GTDGame::queueAttacks()
{
	if (player.hasAttackQueued())
	{
		GTDRect *attackRect = new GTDRect();
		int selectX = screenX + mouseX;
		int selectY = screenY + mouseY;
		int selectW = map.getTileW() / 1.5;
		int selectH = map.getTileH() / 1.5;
		selectX -= map.getTileW() / 3; //arbitrary box size to detect enemy units within a right click
		selectY -= map.getTileH() / 3;
		attackRect->setX(selectX); //min to allow box selection in all directions
		attackRect->setY(selectY);//min to allow box selection in all directions 
		attackRect->setW(selectW);
		attackRect->setH(selectH);
		map.issueAttackOrder(&player, attackRect);
		player.setAttackQueued(false);
	}
}