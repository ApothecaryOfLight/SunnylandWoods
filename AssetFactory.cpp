#include "AssetFactory.hpp"

#include <iostream>
#include <string>
#include <sstream>

#include "CameraManager.hpp"

AssetFactory::AssetFactory ( SDL_Renderer * inRen, CameraManager * inCameraManager ) {
	myRen = inRen;
	myCameraManager = inCameraManager;
	doLoadImages();
}

//TODO: An asset file?
void AssetFactory::doLoadImages ( void ) {
	doLoadImage( 0, true, "media/SPRITES/player/idle/player-idle-", 0, 0, 90, 58, 8 );
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
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->myRect_src = doCreateRect ( 0, 0, inWidth, inHeight );
			myAnimatedAssets[inAssetID]->myStaticAssets[i-1]->myRect_dst = doCreateRect ( (myCameraManager->ScreenWidth/2)-((90*3)/2), (myCameraManager->ScreenHeight/2), inWidth*3, inHeight*3 );

			SDL_FreeSurface( myNewSurface );
		}
	} else if( !isAnimated ) {
		myStaticAssets[inAssetID] = new StaticAsset;

		SDL_Surface *myNewSurface = IMG_Load( inSrcFilename.c_str() );
		if( !myNewSurface ) { std::cout << IMG_GetError() << std::endl; }

		myStaticAssets[inAssetID]->myTexture = SDL_CreateTextureFromSurface( myRen, myNewSurface );
		myStaticAssets[inAssetID]->PixelWidth = inWidth;
		myStaticAssets[inAssetID]->PixelHeight = inHeight;
		myStaticAssets[inAssetID]->myRect_src = doCreateRect ( 0, 0, inWidth, inHeight );
		myStaticAssets[inAssetID]->myRect_dst = doCreateRect ( (myCameraManager->ScreenWidth/2)-((90*3)/2), (myCameraManager->ScreenHeight/2), inWidth*3, inHeight*3 );

		SDL_FreeSurface( myNewSurface );
	}
}

void AssetFactory::doResize ( void ) {
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

void AssetFactory::doAdjustPlayerDest ( int inDestX ) {
	for( int i=0; i<8; i++ ) {
		StaticAsset * StaticAssetPtr = myAnimatedAssets[0]->myStaticAssets[i];
		StaticAssetPtr->myRect_dst.x = inDestX;
	}
}
