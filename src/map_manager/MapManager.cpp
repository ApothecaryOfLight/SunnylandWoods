//MapManager.cpp
#include "MapManager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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

//TODO: This should take place in AssetFactory
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

	//Clean
	//std::cout << "Loading map objects..." << std::endl;
	int newID = myIDManager->getNewID();
	myActiveMapObjects.push_back( newID );
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
	myMapObjects[newID].XPos = 1000;
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
	myMapObjects[newID].myAssetID = 5;
}

void MapManager::doRenderFrame ( void ) {
	//1) Only draw objects in sector

	//2) Draw objects with x/y adjustment for camera panning.

	SDL_RenderCopy( myRen, texture_Clouds, &rect_Clouds, &rect_Clouds_dest );


	std::list<int>::iterator myIter = myActiveMapObjects.begin();
	std::list<int>::iterator myEnd = myActiveMapObjects.end();
	while( myIter != myEnd ) {
		int myGlobalID = (*myIter);
		//std::cout << "drawing object " << myGlobalID << std::endl;
		MapObject * myMapObjectPtr = &myMapObjects[myGlobalID];
		StaticAsset * myStaticAssetPtr = myAssetFactory->myStaticAssets[myMapObjectPtr->myAssetID];

		SDL_Rect myObjectPosition;
		//myCameraPosition = myStaticAssetPtr->myRect_dst;
		myObjectPosition.x = (myMapObjects[myGlobalID].XPos - myCameraManager->PlayerX_level) * myCameraManager->magnification;
		myObjectPosition.y = (myMapObjects[myGlobalID].YPos - myCameraManager->PlayerY_level) * myCameraManager->magnification;
		myObjectPosition.w = myStaticAssetPtr->PixelWidth * myCameraManager->magnification;
		myObjectPosition.h = myStaticAssetPtr->PixelHeight * myCameraManager->magnification;
		//myCameraPosition.y = myCameraManager->PlayerY_level;

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
