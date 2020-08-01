//MainMenuInterface.cpp

#include <SDL.h>
#include <SDL_image.h>

#include "MainMenuInterface.hpp"
/*
TODO: Subsume this within main game loop
*/

MainMenuInterface::MainMenuInterface( SDL_Renderer * inRendererHandle ) {
	isQuit = false;
	GameInterfaceStarted = false;

	ren = inRendererHandle;

	AnimationCounterMountains = screenWIDTH/2;
	AnimationCounterTrees = screenWIDTH/2;
	AnimationCounterPressEnterText = 0;

	BackgroundTileSrc = doMakeRect( 0, 0, 160, 208 );

	BackgroundCloudsLeftDest = doMakeRect( 0, 0, screenWIDTH/2, screenHEIGHT );
	BackgroundCloudsRightDest = doMakeRect( screenWIDTH/2, 0, screenWIDTH/2, screenHEIGHT );

	BackgroundMountainsLeftestDest = doMakeRect( -1*(screenWIDTH/2), 0, screenWIDTH/2, screenHEIGHT );
	BackgroundMountainsLeftDest = doMakeRect( 0, 0, screenWIDTH/2, screenHEIGHT );
	BackgroundMountainsRightDest = doMakeRect( screenWIDTH/2, 0, screenWIDTH/2, screenHEIGHT );

	BackgroundTreesLeftestDest = doMakeRect( -1*(screenWIDTH/2), 0, screenWIDTH/2, screenHEIGHT );
	BackgroundTreesLeftDest = doMakeRect( 0, 0, screenWIDTH/2, screenHEIGHT );
	BackgroundTreesRightDest = doMakeRect( screenWIDTH/2, 0, screenWIDTH/2, screenHEIGHT );

	TitleCardSrc = doMakeRect( 0, 0, 194, 65 );
	TitleCardDest = doMakeRect( (screenWIDTH/2)-((194*3.25)/2), (screenHEIGHT/2)-((65*2)/2.7)-(screenHEIGHT/4), (194*3.25), (65*3.25) );

	PressEnterTextSrc = doMakeRect( 0, 0, 194, 65 );
	PressEnterTextDest = doMakeRect( (screenWIDTH/2)-((194*2.4)/2), (screenHEIGHT/6)*4.5, (194*2.4), 65 );

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

void MainMenuInterface::doComposeFrameMainMenuInterface() {
	AnimationCounterMountains--;
	if( AnimationCounterMountains <= 0 ) {
		AnimationCounterMountains = screenWIDTH/2;
	}
	BackgroundMountainsLeftDest.x = (screenWIDTH/2) - AnimationCounterMountains;
	BackgroundMountainsLeftestDest.x = ((screenWIDTH)*-1) + (screenWIDTH-AnimationCounterMountains);
	BackgroundMountainsRightDest.x = (screenWIDTH)-AnimationCounterMountains;

	AnimationCounterTrees = AnimationCounterTrees-2;
	if( AnimationCounterTrees <= 0 ) {
		AnimationCounterTrees = screenWIDTH/2;
	}
	BackgroundTreesLeftDest.x = (screenWIDTH/2) - AnimationCounterTrees;
	BackgroundTreesLeftestDest.x = ((screenWIDTH)*-1) + (screenWIDTH-AnimationCounterTrees);
	BackgroundTreesRightDest.x = (screenWIDTH)-AnimationCounterTrees;

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
		switch( inEvent->key.keysym.sym ) {
			case SDLK_ESCAPE: isQuit = true; break;
			case SDLK_RETURN: GameInterfaceStarted = true; break;
		}
	}
}
