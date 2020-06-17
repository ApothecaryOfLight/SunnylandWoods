//MapManager.cpp
#include "MapManager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../camera_manager/CameraManager.hpp"
#include "../asset_factory/AssetFactory.hpp"

#include <iostream>

MapManager::MapManager ( SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory ) {
	std::cout << "MapManager constructor called." << std::endl;
	myAssetFactory = inAssetFactory;
	myCameraManager = inCameraManager;
	myRen = inRen;


	myObjectCounter = 0;


	myObjects = new int*[1000];
	for( int i=0; i<=999; i++ ) {
		myObjects[i] = new int[6];
	}

	//TODO: For load level function in LevelManager
	myObjects[0][0] = 1;
	myObjects[0][1] = 1;
	myObjects[0][2] = 1;
	myObjects[0][3] = 1;
	myObjects[0][4] = 1;
	myObjects[0][5] = 1;
	myObjectCounter++;

	myCollisionBoxes = new SDL_Rect*[1000];
	for( int i=0; i<=999; i++ ) {
		myCollisionBoxes[i] = new SDL_Rect;
	}
	std::cout << &myCollisionBoxes << std::endl;

	doLoadMapTextures();

	//doLoadMapObjects();
	std::cout << "MapManager constructor finished." << std::endl;
}

MapManager::~MapManager ( void ) {
	SDL_DestroyTexture( texture_Clouds );
}

/*SDL_Rect MapManager::doCreateRect ( int inX, int inY, int inW, int inH ) {
	SDL_Rect toRetByValue;
	//SDL_Rect * toRetByValue = new SDL_Rect;
	toRetByValue.x = inX; toRetByValue.y = inY; toRetByValue.w = inW; toRetByValue.h = inH;
	return toRetByValue;
}*/

inline void MapManager::doCreateRect ( int inMapObjectID, int inX, int inY, int inW, int inH ) {
	//myCollisionBoxes[ inMapObjectID ] = new SDL_Rect;
	std::cout << "doCreateRect: " << inMapObjectID << " = " << inX << "/" << inY << "/" << inW << "/" << inH << std::endl;
	std::cout << &myCollisionBoxes << std::endl;
	int myMagnification = myCameraManager->magnification;
	myCollisionBoxes[ inMapObjectID ]->x = inX;
	myCollisionBoxes[ inMapObjectID ]->y = inY;
	myCollisionBoxes[ inMapObjectID ]->w = inW * myMagnification;
	myCollisionBoxes[ inMapObjectID ]->h = inH * myMagnification;
}

void MapManager::doLoadMapTextures ( void ) {
	rect_Clouds.x = 0;//TODO: Move to background manager.
	rect_Clouds.y = 0;
	rect_Clouds.w = 160;
	rect_Clouds.h = 208;

	rect_Clouds_dest.x = 0;
	rect_Clouds_dest.y = 0;
	rect_Clouds_dest.w = myCameraManager->ScreenWidth;
	rect_Clouds_dest.h = myCameraManager->ScreenHeight;

	SDL_Surface *myNewSurface = IMG_Load( "media/ENVIRONMENT/bg-clouds.png" );//TODO: Background manager.
	if(!myNewSurface) { std::cout << IMG_GetError() << std::endl; }
	texture_Clouds = SDL_CreateTextureFromSurface( myRen, myNewSurface );
	SDL_FreeSurface( myNewSurface );
}

void MapManager::doLoadMapObjects ( void ) {
	doCreateRect( 0, 320, 320, 16, 16 );	
}

void MapManager::doRenderFrame ( void ) {
	//1) Only draw objects in sector

	//2) Draw objects with x/y adjustment for camera panning.

	SDL_RenderCopy( myRen, texture_Clouds, &rect_Clouds, &rect_Clouds_dest );

	//for( int i=0; i<myObjectCounter; i++ ) {
		//if( myObjects[i][3] == 1 ) { //type = block
			StaticAsset * myStaticAssetPtr = myAssetFactory->myStaticAssets[5]; //Asset ID for block TODO: Make it dynamic, not just the block.
			SDL_RenderCopy(
				myRen,
				myStaticAssetPtr->myTexture,
				&(myStaticAssetPtr->myRect_src),
				&(myStaticAssetPtr->myRect_dst)
			);
		//}
	//}
}
