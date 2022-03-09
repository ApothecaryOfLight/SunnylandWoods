//MainMenuInterface.cpp

#include <SDL.h>
#include <SDL_image.h>

#include "MainMenuInterface.hpp"

#include "../logger/logger.hpp"
/*
TODO: Subsume this within main game loop
*/

MainMenuInterface::MainMenuInterface( SDL_Window * inWin, SDL_Renderer * inRendererHandle, Logger * inLogger ) {
	myLogger = inLogger;
	isQuit = false;
	GameInterfaceStart = false;
	GameInterfaceStarted = false;

	ren = inRendererHandle;

	doSetSizes();

	SDL_Surface *myClouds = IMG_Load( "media/ENVIRONMENT/bg-clouds.png" );
	myTClouds = SDL_CreateTextureFromSurface( ren, myClouds );
	SDL_FreeSurface( myClouds );

	SDL_Surface *myMountains = IMG_Load( "media/ENVIRONMENT/bg-mountains.png" );
	myTMountains = SDL_CreateTextureFromSurface( ren, myMountains );
	SDL_FreeSurface( myMountains );

	SDL_Surface *myTrees = IMG_Load( "media/ENVIRONMENT/bg-trees.png" );
	myTTrees = SDL_CreateTextureFromSurface( ren, myTrees );
	SDL_FreeSurface( myTrees );

	SDL_Surface *myTitleCard = IMG_Load( "media/INTERFACE/title-screen.png" );
	myTTitleCard = SDL_CreateTextureFromSurface( ren, myTitleCard );
	SDL_FreeSurface( myTitleCard );

	SDL_Surface *myPressEnterText = IMG_Load( "media/INTERFACE/press-enter-text.png" );
	myTPressEnterText = SDL_CreateTextureFromSurface( ren, myPressEnterText );
	SDL_FreeSurface( myPressEnterText );
}

MainMenuInterface::~MainMenuInterface() {
	SDL_DestroyTexture( myTClouds );
	SDL_DestroyTexture( myTMountains );
	SDL_DestroyTexture( myTTrees );
	SDL_DestroyTexture( myTTitleCard );
	SDL_DestroyTexture( myTPressEnterText );
}

void MainMenuInterface::doSetSizes() {
	int screenWidth, screenHeight;
	int return_code = SDL_GetRendererOutputSize(ren, &screenWidth, &screenHeight);

	AnimationCounterMountains = screenWidth / 2;
	AnimationCounterTrees = screenWidth / 2;
	AnimationCounterPressEnterText = 0;

	BackgroundTileSrc = doMakeRect(0, 0, 160, 208);

	BackgroundCloudsLeftDest = doMakeRect(0, 0, screenWidth / 2, screenHeight);
	BackgroundCloudsRightDest = doMakeRect(screenWidth / 2, 0, screenWidth / 2, screenHeight);

	BackgroundMountainsLeftestDest = doMakeRect(-1 * (screenWidth / 2), 0, screenWidth / 2, screenHeight);
	BackgroundMountainsLeftDest = doMakeRect(0, 0, screenWidth / 2, screenHeight);
	BackgroundMountainsRightDest = doMakeRect(screenWidth / 2, 0, screenWidth / 2, screenHeight);

	BackgroundTreesLeftestDest = doMakeRect(-1 * (screenWidth / 2), 0, screenWidth / 2, screenHeight);
	BackgroundTreesLeftDest = doMakeRect(0, 0, screenWidth / 2, screenHeight);
	BackgroundTreesRightDest = doMakeRect(screenWidth / 2, 0, screenWidth / 2, screenHeight);

	TitleCardSrc = doMakeRect(0, 0, 194, 65);
	TitleCardDest = doMakeRect((screenWidth / 2) - ((194 * 3.25) / 2), (screenHeight / 2) - ((65 * 2) / 2.7) - (screenHeight / 4), (194 * 3.25), (65 * 3.25));

	PressEnterTextSrc = doMakeRect(0, 0, 194, 65);
	PressEnterTextDest = doMakeRect((screenWidth / 2) - ((194 * 2.4) / 2), (screenHeight / 6) * 4.5, (194 * 2.4), 65);
}

void MainMenuInterface::doComposeFrameMainMenuInterface() {
	int screenWidth, screenHeight;
	int return_code = SDL_GetRendererOutputSize(ren, &screenWidth, &screenHeight);

	if (screenWidth & 1) {
		screenWidth--;
	}
	int half_screen = ceil(screenWidth / 2);

	AnimationCounterMountains--;
	if( AnimationCounterMountains <= 0 ) {
		AnimationCounterMountains = half_screen;
	}
	BackgroundMountainsLeftDest.x = (half_screen) - AnimationCounterMountains;
	BackgroundMountainsLeftestDest.x = ((screenWidth)*-1) + (screenWidth-AnimationCounterMountains);
	BackgroundMountainsRightDest.x = (screenWidth)-AnimationCounterMountains;

	AnimationCounterTrees = AnimationCounterTrees-2;
	if( AnimationCounterTrees <= 0 ) {
		AnimationCounterTrees = half_screen;
	}
	BackgroundTreesLeftDest.x = (half_screen) - AnimationCounterTrees;
	BackgroundTreesLeftestDest.x = ((screenWidth)*-1) + (screenWidth-AnimationCounterTrees);
	BackgroundTreesRightDest.x = (screenWidth)-AnimationCounterTrees;

	AnimationCounterPressEnterText++;
}

void MainMenuInterface::doRenderMainMenuInterface() {
	SDL_RenderClear( ren );

	SDL_RenderCopy( ren, myTClouds, &BackgroundTileSrc, &BackgroundCloudsLeftDest );
	SDL_RenderCopy( ren, myTClouds, &BackgroundTileSrc, &BackgroundCloudsRightDest );

	SDL_RenderCopy( ren, myTMountains, &BackgroundTileSrc, &BackgroundMountainsLeftestDest );
	SDL_RenderCopy( ren, myTMountains, &BackgroundTileSrc, &BackgroundMountainsLeftDest );
	SDL_RenderCopy( ren, myTMountains, &BackgroundTileSrc, &BackgroundMountainsRightDest );

	SDL_RenderCopy( ren, myTTrees, &BackgroundTileSrc, &BackgroundTreesLeftestDest );
	SDL_RenderCopy( ren, myTTrees, &BackgroundTileSrc, &BackgroundTreesLeftDest );
	SDL_RenderCopy( ren, myTTrees, &BackgroundTileSrc, &BackgroundTreesRightDest );

	SDL_RenderCopy( ren, myTTitleCard, &TitleCardSrc, &TitleCardDest );

	if( AnimationCounterPressEnterText <= 20 ) {
		SDL_RenderCopy( ren, myTPressEnterText, &PressEnterTextSrc, &PressEnterTextDest );
	}
	if( AnimationCounterPressEnterText >= 35 ) { AnimationCounterPressEnterText = 0; }

	SDL_RenderPresent(ren);
}

SDL_Rect MainMenuInterface::doMakeRect ( int inX, int inY, int inW, int inH ) {
	SDL_Rect toReturn;
	toReturn.x = inX;
	toReturn.y = inY;
	toReturn.w = inW;
	toReturn.h = inH;
	return toReturn;
}

SDL_Texture * MainMenuInterface::doMakeTexture ( std::string inImageLocation ) {
	SDL_Surface * mySurface = IMG_Load( inImageLocation.c_str() );
	SDL_Texture  * toReturn = SDL_CreateTextureFromSurface( ren, mySurface );
	SDL_FreeSurface( mySurface );
	return toReturn;
}

void MainMenuInterface::doProcessInput ( SDL_Event * inEvent ) {
	while (SDL_PollEvent(inEvent)){
		if (inEvent->type == SDL_QUIT){
			isQuit = true;
		}
		else if (inEvent->type == SDL_WINDOWEVENT) {
			switch (inEvent->window.event) {
			case SDL_WINDOWEVENT_RESIZED:
				doSetSizes();
				doComposeFrameMainMenuInterface();
				break;
			}
		}
		switch( inEvent->key.keysym.sym ) {
			case SDLK_ESCAPE:
				isQuit = true;
				break;
			case SDLK_RETURN:
				GameInterfaceStart = true;
				break;
		}
	}
}
