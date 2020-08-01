#include "TimeManager.hpp"

#include <SDL.h>

TimeManager::TimeManager() {
	isStarted = false;
	myStartTicks = 0;
}

void TimeManager::doStart() {
	myStartTicks = SDL_GetTicks();
	isStarted = true;
}

Uint32 TimeManager::getTicks() {
	return (Uint32)(SDL_GetTicks()-myStartTicks);
}
