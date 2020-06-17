//MainMenuInterface.hpp

#ifndef _MAIN_MENU_MANAGER_
#define _MAIN_MENU_MANAGER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#define screenWIDTH 960
#define screenHEIGHT 624

class MainMenuInterface {
public:
	MainMenuInterface( SDL_Renderer * inRendererHandle );
	~MainMenuInterface();

	//Animation Functions
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
};

#endif

