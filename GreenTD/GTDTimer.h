#pragma once

#include <iostream>
#ifdef TARGET_OS_MAC
#include <SDL2/SDL.h>
#elif _WIN64
#include <SDL.h>
#elif _WIN32
#include <SDL.h>
#endif

class GTDTimer {
public:
	GTDTimer();

	//self explanatory!!!!!!!!???????????????????????????????????????
	void start();
	void stop();
	void pause();
	void unpause();

	int getTicks();

	bool isStarted();
	bool isPaused();

private:
	int startTicks;
	int pauseTicks;

	bool started;
	bool paused;

};