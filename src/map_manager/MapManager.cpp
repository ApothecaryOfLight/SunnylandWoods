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

MapManager::MapManager ( SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory, IDManager * inIDManager ) {
	std::cout << "MapManager constructor called." << std::endl;
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

void MapManager::doLoadMapObjects ( void ) {
	doCreateRect( 0, 320, 320, 16, 16 );

	//Clean
	//std::cout << "Loading map objects..." << std::endl;
	int newID = myIDManager->getNewID();
	/*myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 320;
	myMapObjects[newID].YPos = -720;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 300;
	myMapObjects[newID].YPos = -600;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 200;
	myMapObjects[newID].YPos = -500;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 100;
	myMapObjects[newID].YPos = -400;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;



	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = -100;
	myMapObjects[newID].YPos = -300;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = -200;
	myMapObjects[newID].YPos = -200;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 400;
	myMapObjects[newID].YPos = -100;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 500;
	myMapObjects[newID].YPos = 100;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 600;
	myMapObjects[newID].YPos = 200;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 800;
	myMapObjects[newID].YPos = 300;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 700;
	myMapObjects[newID].YPos = 400;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 800;
	myMapObjects[newID].YPos = 500;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;

	newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
	myMapObjects[newID].XPos = 900;
	myMapObjects[newID].YPos = 600;
	myMapObjects[newID].myGlobalID = newID;
	myMapObjects[newID].myAssetID = 5;*/


	/*for (int i = 20; i < 60; i++) {
		newID = myIDManager->getNewID();
		myActiveMapObjects.push_back(newID);
		myMapObjects[newID].XPos = -30;
		myMapObjects[newID].YPos = i*16;
		myMapObjects[newID].myGlobalID = newID;
		myMapObjects[newID].myAssetID = 5;
		myMapObjects[newID].has_collided = false;
		myMapObjects[newID].has_collided_counter = 0;
	}*/
	for (int i = -60; i < 60; i++) {
		newID = myIDManager->getNewID();
		myActiveMapObjects.push_back(newID);
		myMapObjects[newID].XPos = 90;
		myMapObjects[newID].YPos = i * 16;
		myMapObjects[newID].myGlobalID = newID;
		myMapObjects[newID].myAssetID = 5;
		myMapObjects[newID].has_collided = false;
		myMapObjects[newID].has_collided_counter = 0;
	}

	for (int i = -100; i < 100; i++) {
		newID = myIDManager->getNewID();
		myActiveMapObjects.push_back(newID);
		myMapObjects[newID].XPos = i * 16;
		myMapObjects[newID].YPos = 1000;
		myMapObjects[newID].myGlobalID = newID;
		myMapObjects[newID].myAssetID = 5;
		myMapObjects[newID].has_collided = false;
		myMapObjects[newID].has_collided_counter = 0;
	}

	for (int i = -100; i < 100; i++) {
		newID = myIDManager->getNewID();
		myActiveMapObjects.push_back(newID);
		myMapObjects[newID].XPos = i * 16;
		myMapObjects[newID].YPos = 50;
		myMapObjects[newID].myGlobalID = newID;
		myMapObjects[newID].myAssetID = 5;
		myMapObjects[newID].has_collided = false;
		myMapObjects[newID].has_collided_counter = 0;
	}
	for (int i = -50; i < 50; i++) {
		newID = myIDManager->getNewID();
		myActiveMapObjects.push_back(newID);
		myMapObjects[newID].XPos = i * 16;
		myMapObjects[newID].YPos = -25;
		myMapObjects[newID].myGlobalID = newID;
		myMapObjects[newID].myAssetID = 5;
		myMapObjects[newID].has_collided = false;
		myMapObjects[newID].has_collided_counter = 0;
	}
	for (int i = -25; i < 25; i++) {
		newID = myIDManager->getNewID();
		myActiveMapObjects.push_back(newID);
		myMapObjects[newID].XPos = i * 16;
		myMapObjects[newID].YPos = -100;
		myMapObjects[newID].myGlobalID = newID;
		myMapObjects[newID].myAssetID = 5;
		myMapObjects[newID].has_collided = false;
		myMapObjects[newID].has_collided_counter = 0;
	}
	for (int i = -10; i < 10; i++) {
		newID = myIDManager->getNewID();
		myActiveMapObjects.push_back(newID);
		myMapObjects[newID].XPos = i * 16;
		myMapObjects[newID].YPos = -175;
		myMapObjects[newID].myGlobalID = newID;
		myMapObjects[newID].myAssetID = 5;
		myMapObjects[newID].has_collided = false;
		myMapObjects[newID].has_collided_counter = 0;
	}
	for (int i = -5; i < 5; i++) {
		newID = myIDManager->getNewID();
		myActiveMapObjects.push_back(newID);
		myMapObjects[newID].XPos = i * 16;
		myMapObjects[newID].YPos = -250;
		myMapObjects[newID].myGlobalID = newID;
		myMapObjects[newID].myAssetID = 5;
		myMapObjects[newID].has_collided = false;
		myMapObjects[newID].has_collided_counter = 0;
	}
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
		myObjectPosition.x = myMapObjects[myGlobalID].XPos - myCameraManager->CameraX;
		myObjectPosition.y = myMapObjects[myGlobalID].YPos - myCameraManager->CameraY;
		myObjectPosition.w = myStaticAssetPtr->myRect_src.w;
		myObjectPosition.h = myStaticAssetPtr->myRect_src.h;

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