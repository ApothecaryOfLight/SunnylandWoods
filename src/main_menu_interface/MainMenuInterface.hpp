//MainMenuInterface.hpp

#ifndef _MAIN_MENU_MANAGER_
#define _MAIN_MENU_MANAGER_

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "../logger/logger.hpp"

#define screenWIDTH 960
#define screenHEIGHT 624

class MainMenuInterface {
public:
	MainMenuInterface( SDL_Window * inWin, SDL_Renderer * inRendererHandle, Logger * inLogger );
	~MainMenuInterface();

	//Animation Functions
	void doSetSizes();
	void doComposeFrameMainMenuInterface();
	void doRenderMainMenuInterface();

	//Factory functions
	SDL_Rect doMakeRect ( int inX, int inY, int inW, int inH );
	SDL_Texture * doMakeTexture ( std::string inImageLocation );

	void doProcessInput ( SDL_Event * inEvent );

	//Textures
	SDL_Texture *myTClouds, *myTMountains, *myTTrees, *myTTitleCard, *myTPressEnterText;

	//Rects for drawing
	SDL_Rect BackgroundTileSrc;
	SDL_Rect BackgroundMountainsLeftestDest, BackgroundMountainsLeftDest, BackgroundMountainsRightDest;
	SDL_Rect BackgroundTreesLeftestDest, BackgroundTreesLeftDest, BackgroundTreesRightDest;
	SDL_Rect TitleCardSrc, TitleCardDest, PressEnterTextSrc, PressEnterTextDest;
	SDL_Rect BackgroundCloudsLeftDest, BackgroundCloudsRightDest;

	//Animation Variables
	int AnimationCounterMountains, AnimationCounterTrees, AnimationCounterPressEnterText;

	//Interface-flow variables.
	bool isQuit;
	bool GameInterfaceStarted;
private:
	SDL_Renderer *ren;
	SDL_Window* myWin;
	Logger* myLogger;
};

#endif

