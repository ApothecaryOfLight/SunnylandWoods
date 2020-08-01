#ifndef _TIMER_MANAGER_
#define _TIMER_MANAGER_

#include <SDL.h>

class TimeManager {
public:
	TimeManager();
	void doStart();
	Uint32 getTicks();
	bool isStarted;
private:
	Uint32 myStartTicks;
};
#endif
