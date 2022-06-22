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

	bool interfaceSwitched;
	/*
	* interfaceID: Determines which interface should be displayed to the user.
	* 0: Main menu
	* 1: Game interface
	* 2: Editor interface
	* 3: Level selector interface
	*/
	int interfaceID;
private:
	MainMenuInterface * myMainMenuInterface;
	GameInterface * myGameInterface;
	SDL_Window* myWin;
	Logger* myLogger;
};

#endif
