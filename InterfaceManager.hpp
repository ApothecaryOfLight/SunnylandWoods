//myInterfaceManager.hpp

#ifndef _INTERFACE_MANAGER_
#define _INTERFACE_MANAGER_

#include <SDL2/SDL.h>

#include "MainMenuInterface.hpp"
#include "GameInterface.hpp"

class InterfaceManager {
public:
	InterfaceManager ( SDL_Renderer * inRenderer );
	~InterfaceManager ( void );
	void doProcessInput ( SDL_Event * inEvent );
	void doGameLogic ( void );
	void doRenderFrame ( void );
	bool isQuit ( void );
private:
	SDL_Renderer * myRen;
	MainMenuInterface * myMainMenuInterface;
	GameInterface * myGameInterface;
};

#endif
