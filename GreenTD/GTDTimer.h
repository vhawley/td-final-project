#include <iostream>
#include <SDL.h>

#ifndef GTDTIMER_H
#define GTDTIMER_H

class GTDTimer {
public:
	GTDTimer();

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

#endif