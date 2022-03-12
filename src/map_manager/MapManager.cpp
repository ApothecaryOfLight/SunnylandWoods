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
	myCollisionBoxes[ inMapObjectID ]->x = inX;
	myCollisionBoxes[ inMapObjectID ]->y = inY;
	myCollisionBoxes[ inMapObjectID ]->w = inW;
	myCollisionBoxes[ inMapObjectID ]->h = inH;
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

	for (int i = -50; i < 100; i++) {
		doCreateMapObject( i*16, 100, 0 );
	}
	for (int i = -50; i < -35; i++) {
		doCreateMapObject(i * 16, 0, 0);
	}

	//-475 -> -350 @ -70
	for (int i = -30; i < -21; i++) {
		doCreateMapObject(i * 16, -70, 0);
	}

	//-475 -> -350 @ -70
	for (int i = -16; i < -11; i++) {
		doCreateMapObject(i * 16, -95, 0);
	}

	//Ant platform #1
	for (int i = -8; i < 22; i++) {
		doCreateMapObject(i * 16, -100, 0);
	}
	doCreateMapObject(-128, -116, 0);

	//Branch #1
	doCreateMapObject(127, -210, 7);
	doCreateMapObject(175, -210, 8);
	doCreateMapObject(207, -210, 9);
	doCreateMapObject(239, -210, 8);
	doCreateMapObject(271, -210, 9);
	doCreateMapObject(303, -210, 10);

	//Boundary trees
	for (int log = -20; log < 20; log+=4) {
		doCreateMapObject(-895, -200 + ((log) * 48), 3);
		doCreateMapObject(-895, -200 + ((log+1) * 48), 4);
		doCreateMapObject(-895, -200 + ((log+2) * 48), 5);
		doCreateMapObject(-895, -200 + ((log + 3) * 48), 6);
	}
	for (int log = -20; log < 20; log += 4) {
		doCreateMapObject(350, -200 + ((log) * 48), 3);
		doCreateMapObject(350, -200 + ((log + 1) * 48), 4);
		doCreateMapObject(350, -200 + ((log + 2) * 48), 5);
		doCreateMapObject(350, -200 + ((log + 3) * 48), 6);
	}

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
		coord myObjectCoords = myCameraManager->translate_coords(myMapObjects[myGlobalID].XPos, myMapObjects[myGlobalID].YPos);
		myObjectPosition.x = myObjectCoords.x;
		myObjectPosition.y = myObjectCoords.y;
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