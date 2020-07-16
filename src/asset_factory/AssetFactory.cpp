#include "AssetFactory.hpp"

#include <iostream>
#include <string>
#include <sstream>

#include "../camera_manager/CameraManager.hpp"

AssetFactory::AssetFactory ( SDL_Renderer * inRen, CameraManager * inCameraManager ) {
	myRen = inRen;
	myCameraManager = inCameraManager;
	doLoadImages();
}

//TODO: An asset file?
void AssetFactory::doLoadImages ( void ) {
	doLoadPlayerImages();
	doLoadImage( 5, false, "media/ENVIRONMENT/tileset.png", 384, 96, 16, 16, 0 );
}

//TODO: Inline?
SDL_Rect AssetFactory::doCreateRect ( int inX, int inY, int inW, int inH ) {
	SDL_Rect toRetByValue; toRetByValue.x = inX; toRetByValue.y = inY; toRetByValue.w = inW; toRetByValue.h = inH; return toRetByValue;
}

void AssetFactory::doLoadImage ( int inAssetID, bool isAnimated, std::string inSrcFilename, int inSrcX, int inSrcY, int inWidth, int inHeight, int inFrames ) {
	if( isAnimated ) {
		myAnimatedAssets[inAssetID] = new AnimatedAsset;
		myAnimatedAssets[inAssetID]->frames = inFrames;
		myAnimatedAssets[inAssetID]->myStaticAssets = new StaticAsset*[inFrames];
		for( int i=1; i<=inFrames; i++ ) {
			std::stringstream myStringstreamConv;
			myStringstreamConv << i;
			std::string myConv = myStringstreamConv.str();

			SDL_Surface *myNewSurface = IMG_Load( std::string( inSrcFilename + myConv + ".png" ).c_str() );
			if(!myNewSurface) { std::cout << IMG_GetError() << std::endl; }

			myAnimatedAssets[inAssetID]->myStaticAssets[i-1] = new StaticAsset;
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->myTexture = SDL_CreateTextureFromSurface( myRen, myNewSurface );
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->PixelWidth = inWidth;
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->PixelHeight = inHeight;
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->myRect_src = doCreateRect ( inSrcX, inSrcY, inWidth, inHeight );
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->myRect_dst = doCreateRect ( myCameraManager->PlayerX_screen, myCameraManager->PlayerY_screen, inWidth*3, inHeight*3 );

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

void AssetFactory::doResize ( void ) {
	//TODO: doSetPlayerSize
	for( int i=0; i<8; i++ ) {
		StaticAsset * myStaticAssetPtr = myAnimatedAssets[0]->myStaticAssets[i];
		int width = myStaticAssetPtr->PixelWidth;
		int multiplier = 3;//TODO: Tie into input
		myStaticAssetPtr->myRect_dst = doCreateRect (
			(myCameraManager->ScreenWidth/2)-( (width*multiplier)/2 ),	//screen position X
			(myCameraManager->ScreenHeight/2),	//screen position y
			myStaticAssetPtr->PixelWidth*multiplier,	//hoziontal scaling
			myStaticAssetPtr->PixelHeight*multiplier	//vertical scaling
		);
	}
}

//TODO: Remove, AssetFactory shouldn't track object positions, they're not the same thing.
void AssetFactory::doAdjustPlayerDest ( int inDestX, int inDestY ) { //TODO: Each object needs a positional location and associated metadata outside of the Asset.
	for( int i=0; i<8; i++ ) {
		StaticAsset * StaticAssetPtr = myAnimatedAssets[0]->myStaticAssets[i];
		StaticAssetPtr->myRect_dst.x = inDestX;
		StaticAssetPtr->myRect_dst.y = inDestY;
	}
	for( int i=0; i<6; i++ ) {
		StaticAsset * StaticAssetPtr = myAnimatedAssets[1]->myStaticAssets[i];
		StaticAssetPtr->myRect_dst.x = inDestX;
		StaticAssetPtr->myRect_dst.y = inDestY;
	}
	for( int i=0; i<4; i++ ) {
		StaticAsset * StaticAssetPtr = myAnimatedAssets[4]->myStaticAssets[i];
		StaticAssetPtr->myRect_dst.x = inDestX;
		StaticAssetPtr->myRect_dst.y = inDestY;
	}
	for( int i=0; i<2; i++ ) {
		StaticAsset * StaticAssetPtr = myAnimatedAssets[2]->myStaticAssets[i];
		StaticAssetPtr->myRect_dst.x = inDestX;
		StaticAssetPtr->myRect_dst.y = inDestY;
	}
	for( int i=0; i<2; i++ ) {
		StaticAsset * StaticAssetPtr = myAnimatedAssets[3]->myStaticAssets[i];
		StaticAssetPtr->myRect_dst.x = inDestX;
		StaticAssetPtr->myRect_dst.y = inDestY;
	}
}

void AssetFactory::doLoadPlayerImages ( void ) {
	myCameraManager->doSetPlayerSize( 90, 85 );
	doLoadImage( 0, true, "media/SPRITES/player/idle/player-idle-", 0, 0, 90, 58, 8 );
	doLoadImage( 1, true, "media/SPRITES/player/run/player-run-", 0, 0, 90, 58, 6 );
	doLoadImage( 2, true, "media/SPRITES/player/hurt/player-hurt-", 0, 0, 90, 58, 2 );
	doLoadImage( 3, true, "media/SPRITES/player/crouch/player-crouch-", 0, 0, 90, 58, 2 );
	doLoadImage( 4, true, "media/SPRITES/player/jump/player-jump-", 0, 0, 90, 58, 4 );	


}
