//MapManager.cpp
#include "MapManager.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "../camera_manager/CameraManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../id_manager/id_manager.hpp"

#include <iostream>
#include <list>

MapObject::MapObject ( void ) {

}


MapObject::MapObject( int GlobalID, int AssetID ) {

}

MapManager::MapManager ( SDL_Renderer * inRen, Logger* inLogger, CameraManager * inCameraManager, AssetFactory * inAssetFactory, IDManager * inIDManager ) {
	std::cout << "MapManager constructor called." << std::endl;
	myLogger = inLogger;
	myAssetFactory = inAssetFactory;
	myCameraManager = inCameraManager;
	myRen = inRen;
	myIDManager = inIDManager;

	//myObjectCounter = 0;

	/*myObjects = new int*[1000];
	for( int i=0; i<=999; i++ ) {
		myObjects[i] = new int[6];
	}

	//TODO: For load level function in LevelManager
	myMapObjects[0][0] = 1;
	myObjects[0][1] = 1;
	myObjects[0][2] = 1;
	myObjects[0][3] = 1;
	myObjects[0][4] = 1;
	myObjects[0][5] = 1;
	myObjectCounter++;*/

	myMapObjects = new MapObject[5000];

	myCollisionBoxes = new SDL_Rect*[1000];
	for( int i=0; i<=999; i++ ) {
		myCollisionBoxes[i] = new SDL_Rect;
	}
	std::cout << &myCollisionBoxes << std::endl;

	doLoadMapTextures();

	doLoadMapObjects();
	std::cout << "MapManager constructor finished." << std::endl;
}

MapManager::~MapManager ( void ) {
	SDL_DestroyTexture( texture_Clouds );
}

inline void MapManager::doCreateRect ( int inMapObjectID, int inX, int inY, int inW, int inH ) {
	std::cout << "doCreateRect: " << inMapObjectID << " = " << inX << "/" << inY << "/" << inW << "/" << inH << std::endl;
	std::cout << &myCollisionBoxes << std::endl;
	int myMagnification = myCameraManager->magnification;
	myCollisionBoxes[ inMapObjectID ]->x = inX;
	myCollisionBoxes[ inMapObjectID ]->y = inY;
	myCollisionBoxes[ inMapObjectID ]->w = inW * myMagnification;
	myCollisionBoxes[ inMapObjectID ]->h = inH * myMagnification;
}

//TODO: This should take place in AssetFactory
//TODO: This should be called on resize.
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

void MapManager::doResize( int screenWidth, int screenHeight ) {
	rect_Clouds.x = 0;//TODO: Move to background manager.
	rect_Clouds.y = 0;
	rect_Clouds.w = 160;
	rect_Clouds.h = 208;

	rect_Clouds_dest.x = 0;
	rect_Clouds_dest.y = 0;
	rect_Clouds_dest.w = myCameraManager->ScreenWidth;
	rect_Clouds_dest.h = myCameraManager->ScreenHeight;
}

void MapManager::doCreateMapObject(int XPos, int YPos, int AssetID) {
	int newID = myIDManager->getNewID();
	myActiveMapObjects.push_back(newID);
	myMapObjects[newID].XPos = XPos;
	myMapObjects[newID].YPos = YPos;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = AssetID;
	myMapObjects[newID].has_collided = false;
	myMapObjects[newID].has_collided_counter = 0;
}

void MapManager::doLoadMapObjects ( void ) {
	doCreateRect( 0, 320, 320, 16, 16 );

	int newID = myIDManager->getNewID();

	for (int i = -60; i < 60; i++) {
		doCreateMapObject( 90, i*16, 0 );
		doCreateMapObject(340, i * 16, 0);
	}

	for (int i = -100; i < 100; i++) {
		doCreateMapObject(i * 16, 1000, 0);
		doCreateMapObject( i*16, 50, 0 );
	}

	for (int i = -50; i < 50; i++) {
		doCreateMapObject(i * 16, -25, 0);
	}
	for (int i = -25; i < 25; i++) {
		doCreateMapObject(i * 16, -100, 0);
	}
	for (int i = -10; i < 10; i++) {
		doCreateMapObject(i * 16, -175, 0);
	}
	for (int i = -5; i < 5; i++) {
		doCreateMapObject(i * 16, -250, 0);
	}

	doCreateMapObject(220, -300, 1);

	myLogger->log("Map objects initialised. Total: " + std::to_string(myIDManager->getIDcounter()));
}

void MapManager::doRenderFrame ( void ) {
	//1) Only draw objects in sector

	//2) Draw objects with x/y adjustment for camera panning.

	SDL_RenderCopy( myRen, texture_Clouds, &rect_Clouds, &rect_Clouds_dest );


	std::list<int>::iterator myIter = myActiveMapObjects.begin();
	std::list<int>::iterator myEnd = myActiveMapObjects.end();
	while( myIter != myEnd ) {
		int myGlobalID = (*myIter);
		MapObject * myMapObjectPtr = &myMapObjects[myGlobalID];
		StaticAsset * myStaticAssetPtr = myAssetFactory->myStaticAssets[myMapObjectPtr->myAssetID];

		SDL_Rect myObjectPosition;
		myObjectPosition.x = static_cast<int>(myMapObjects[myGlobalID].XPos*myCameraManager->zoom) - myCameraManager->CameraX;
		myObjectPosition.y = static_cast<int>(myMapObjects[myGlobalID].YPos*myCameraManager->zoom) - myCameraManager->CameraY;
		myObjectPosition.w = myStaticAssetPtr->myRect_dst.w;
		myObjectPosition.h = myStaticAssetPtr->myRect_dst.h;

		SDL_RenderCopy(
			myRen,
			myStaticAssetPtr->myTexture,
			&(myStaticAssetPtr->myRect_src),
			&myObjectPosition
		);

		++myIter;
	}
}

int MapManager::getAssetID( int inGlobalID ) {
	return myMapObjects[inGlobalID].myAssetID;
}
MapObject * MapManager::getMapObject ( int inGlobalID ) {
	return &myMapObjects[inGlobalID];
}

void MapManager::mark_collided(int inMapObjectID) {
	myMapObjects[inMapObjectID].has_collided = true;
	myMapObjects[inMapObjectID].has_collided_counter = 5;
}

void MapManager::decrement_collided(int inMapObjectID) {
	if (myMapObjects[inMapObjectID].has_collided_counter <= 0) {
		myMapObjects[inMapObjectID].has_collided = false;
	}
	else {
		myMapObjects[inMapObjectID].has_collided_counter--;
	}
}