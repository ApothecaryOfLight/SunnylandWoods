//InterfaceManager.cpp
#include "InterfaceManager.hpp"

#include <SDL.h>

#include "../main_menu_interface/MainMenuInterface.hpp"
#include "../game_interface/GameInterface.hpp"

InterfaceManager::InterfaceManager ( SDL_Window * inWin, SDL_Renderer * inRenderer, Logger * inLogger ) {
	myLogger = inLogger;
	myWin = inWin;
	myMainMenuInterface = new MainMenuInterface( inWin, inRenderer, inLogger );
	myGameInterface = new GameInterface( inWin, inRenderer, inLogger );
}

InterfaceManager::~InterfaceManager ( void ) {
	delete myMainMenuInterface;
	delete myGameInterface;
}

void InterfaceManager::doProcessInput ( SDL_Event * inEvent ) {
	if( myMainMenuInterface->GameInterfaceStarted == false ) {
		myMainMenuInterface->doProcessInput( inEvent );
	}
	else if( myMainMenuInterface->GameInterfaceStarted == true ) {
		myGameInterface->doProcessInput( inEvent );
	}
}

void InterfaceManager::doGameLogic ( void ) {
	if( myMainMenuInterface->GameInterfaceStarted == false ) {
		myMainMenuInterface->doComposeFrameMainMenuInterface();
	}
	else if( myMainMenuInterface->GameInterfaceStarted == true ) {
		myGameInterface->doGameLogic();
	}
}

void InterfaceManager::doRenderFrame ( void ) {
	if( myMainMenuInterface->GameInterfaceStarted == false ) {
		myMainMenuInterface->doRenderMainMenuInterface();
	}
	else if( myMainMenuInterface->GameInterfaceStarted == true ) {
		myGameInterface->doRenderFrame();
	}
}


bool InterfaceManager::isQuit ( void ) {
	if( myMainMenuInterface->GameInterfaceStarted == false ) {
		return myMainMenuInterface->isQuit;
	}
	else if( myMainMenuInterface->GameInterfaceStarted == true ) {
		return myGameInterface->isQuit();
	}
}
