#include "AssetFactory.hpp"
#include "../logger/logger.hpp"

#include <iostream>
#include <string>
#include <sstream>

#include "../camera_manager/CameraManager.hpp"

AssetFactory::AssetFactory ( Logger * inLogger, SDL_Renderer * inRen, CameraManager * inCameraManager ) {
	myLogger = inLogger;
	myRen = inRen;
	myCameraManager = inCameraManager;
	doLoadImages();
}

void AssetFactory::doLoadImages ( void ) {
	doLoadPlayerImages();
	doLoadImage( 5, false, "media/ENVIRONMENT/tileset.png", 384, 96, 16, 16, 0 );
	doLoadEnemyImages();
}

SDL_Rect AssetFactory::doCreateRect ( int inX, int inY, int inW, int inH ) {
	SDL_Rect toRetByValue;
	toRetByValue.x = inX;
	toRetByValue.y = inY;
	toRetByValue.w = inW;
	toRetByValue.h = inH;
	return toRetByValue;
}

void AssetFactory::doLoadImage ( int inAssetID, bool isAnimated, std::string inSrcFilename, int inSrcX, int inSrcY, int inWidth, int inHeight, int inFrames ) {
	myLogger->log( "doLoadImage" );
	myLogger->log( inAssetID );
	if( isAnimated ) {
		myAnimatedAssets[inAssetID] = new AnimatedAsset;
		myAnimatedAssets[inAssetID]->frames = inFrames;
		myAnimatedAssets[inAssetID]->myStaticAssets = new StaticAsset*[inFrames];
		for( int i=1; i<=inFrames; i++ ) {
			std::stringstream myStringstreamConv;
			myStringstreamConv << i;
			std::string myConv = myStringstreamConv.str();

			SDL_Surface *myNewSurface = IMG_Load( std::string( inSrcFilename + myConv + ".png" ).c_str() );
			myLogger->log( std::string(inSrcFilename + myConv + ".png") );
			
			if(!myNewSurface) { myLogger->log(IMG_GetError()); }

			myAnimatedAssets[inAssetID]->myStaticAssets[i-1] = new StaticAsset;
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->myTexture = SDL_CreateTextureFromSurface( myRen, myNewSurface );
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->PixelWidth = inWidth;
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->PixelHeight = inHeight;
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->myRect_src = doCreateRect ( inSrcX, inSrcY, inWidth, inHeight );
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->myRect_dst = doCreateRect (
				myCameraManager->PlayerX_screen,
				myCameraManager->PlayerY_screen,
				inWidth * myCameraManager->magnification,
				inHeight * myCameraManager->magnification
			);

			SDL_FreeSurface( myNewSurface );
		}
	} else if( !isAnimated ) {
		std::cout << "Loading image 5." << std::endl;
		std::cout << inAssetID << std::endl;
		myStaticAssets[inAssetID] = new StaticAsset;

		SDL_Surface *myNewSurface = IMG_Load( inSrcFilename.c_str() );
		if( !myNewSurface ) { std::cout << IMG_GetError() << std::endl; }

		myStaticAssets[inAssetID]->myTexture = SDL_CreateTextureFromSurface( myRen, myNewSurface );
		myStaticAssets[inAssetID]->PixelWidth = inWidth;
		myStaticAssets[inAssetID]->PixelHeight = inHeight;
		myStaticAssets[inAssetID]->myRect_src = doCreateRect ( inSrcX, inSrcY, inWidth, inHeight );
		myStaticAssets[inAssetID]->myRect_dst = doCreateRect ( 0, 0, inWidth, inHeight );

		SDL_FreeSurface( myNewSurface );
	}
}

void AssetFactory::doResize (int screenWidth, int screenHeight) {
	//TODO: doSetPlayerSize
	for( int i=0; i<8; i++ ) {
		StaticAsset * myStaticAssetPtr = myAnimatedAssets[0]->myStaticAssets[i];
		int width = myStaticAssetPtr->PixelWidth;
		int multiplier = myCameraManager->magnification;//TODO: Tie into input
		myStaticAssetPtr->myRect_dst = doCreateRect (
			(myCameraManager->ScreenWidth/2)-( (width*multiplier)/2 ),	//screen position X
			(myCameraManager->ScreenHeight/2),	//screen position y
			myStaticAssetPtr->PixelWidth*multiplier,	//hoziontal scaling
			myStaticAssetPtr->PixelHeight*multiplier	//vertical scaling
		);
	}
}

void AssetFactory::doLoadPlayerImages ( void ) {
	myCameraManager->doSetPlayerSize( 90, 85 );
	doLoadImage( 0, true, "media/SPRITES/player/idle/player-idle-", 20, 0, 36, 48, 8 );
	doLoadImage( 1, true, "media/SPRITES/player/run/player-run-", 9, 0, 54, 48, 6 );
	doLoadImage( 2, true, "media/SPRITES/player/hurt/player-hurt-", 0, 0, 90, 58, 2 );
	doLoadImage( 3, true, "media/SPRITES/player/crouch/player-crouch-", 0, 0, 90, 58, 2 );
	doLoadImage( 4, true, "media/SPRITES/player/jump/player-jump-", 21, 0, 45, 58, 4 );

	myLogger->log("Player images loaded.");
}

void AssetFactory::doLoadEnemyImages(void) {
	doLoadImage(6, true, "media/SPRITES/enemies/ant/ant-", 0, 0, 37, 31, 7);
}