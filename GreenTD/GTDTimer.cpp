#include "GTDTimer.h"

GTDTimer::GTDTimer()
{
	paused = false;
	started = false;

	startTicks = 0;
	pauseTicks = 0;

}

void GTDTimer::start()
{
	started = true;
	paused = false;
	startTicks = SDL_GetTicks();
}

void GTDTimer::stop()
{
	started = false;
	paused = false;
	startTicks = 0;
}

void GTDTimer::pause()
{
	if (started)
	{
		if (paused)
		{
			std::cout << "Timer is already paused" << std::endl;
		}
		else
		{
			pauseTicks = SDL_GetTicks() - startTicks;
			paused = true;
		}
	}
	else
	{
		std::cout << "Timer has not been started." << std::endl;
	}
}

void GTDTimer::unpause()
{
	if (started)
	{
		if (paused)
		{
			pauseTicks = SDL_GetTicks() - startTicks;
			paused = false;
		}
		else
		{
			std::cout << "Timer is not paused" << std::endl;
		}
	}
	else
	{
		std::cout << "Timer has not been started." << std::endl;
	}
}

int GTDTimer::getTicks()
{
	if (started)
	{
		if (paused)
		{
			return pauseTicks;
		}
		else
		{
			return SDL_GetTicks() - startTicks;
		}
	}
	return 0; // not running
}

bool GTDTimer::isStarted()
{
	return started;
}
bool GTDTimer::isPaused()
{
	return paused;
}
