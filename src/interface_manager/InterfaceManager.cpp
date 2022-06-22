//InterfaceManager.cpp
#include "InterfaceManager.hpp"

#include <SDL.h>

#include "../main_menu_interface/MainMenuInterface.hpp"
#include "../game_interface/GameInterface.hpp"

InterfaceManager::InterfaceManager ( SDL_Window * inWin, SDL_Renderer * inRenderer, Logger * inLogger ) {
	myLogger = inLogger;
	myWin = inWin;
	myMainMenuInterface = new MainMenuInterface( inWin, inRenderer, inLogger, &interfaceSwitched, &interfaceID );
	myGameInterface = new GameInterface( inWin, inRenderer, inLogger, &interfaceSwitched, &interfaceID );

	interfaceSwitched = false;
	interfaceID = 0;
}

InterfaceManager::~InterfaceManager ( void ) {
	delete myMainMenuInterface;
	delete myGameInterface;
}

void InterfaceManager::doProcessInput ( SDL_Event * inEvent ) {
	if( interfaceID == 0 ) {
		myMainMenuInterface->doProcessInput( inEvent );
	}
	else if( interfaceID == 1 ) {
		myGameInterface->doProcessInput( inEvent );
	}
}

void InterfaceManager::doGameLogic ( void ) {
	if( interfaceID == 0 ) {
		myMainMenuInterface->doComposeFrameMainMenuInterface();
	}
	else if( interfaceID == 1 ) {
		myGameInterface->doGameLogic();
	}
}

void InterfaceManager::doRenderFrame ( void ) {
	if (interfaceSwitched) {
		if (interfaceID == 1) {
			myGameInterface->doGameStart();
		}
		interfaceSwitched = false;
	}
	if (interfaceID == 0) {
		myMainMenuInterface->doRenderMainMenuInterface();
	}
	else if (interfaceID == 1) {
		myGameInterface->doRenderFrame();
	}
}


bool InterfaceManager::isQuit ( void ) {
	if( interfaceID == 0 ) {
		return myMainMenuInterface->isQuit;
	}
	else if( interfaceID == 1 ) {
		return myGameInterface->isQuit();
	}
}
