//myInterfaceManager.hpp

#ifndef _INTERFACE_MANAGER_
#define _INTERFACE_MANAGER_

#include <SDL.h>

#include "../main_menu_interface/MainMenuInterface.hpp"
#include "../game_interface/GameInterface.hpp"

class InterfaceManager {
public:
	InterfaceManager ( SDL_Window * inWin, SDL_Renderer * inRenderer, Logger * inLogger );
	~InterfaceManager ( void );
	void doProcessInput ( SDL_Event * inEvent );
	void doGameLogic ( void );
	void doRenderFrame ( void );
	bool isQuit ( void );
private:
//	SDL_Renderer * myRen;
	MainMenuInterface * myMainMenuInterface;
	GameInterface * myGameInterface;
	SDL_Window* myWin;
	Logger* myLogger;
};

#endif
