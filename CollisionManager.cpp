//CollisionManager.cpp

/*
TODO:
	Collision dimensions are stored in AssetFactory
	Object positions are stored in their respective Object Managers
	CollisionManager is not to be queried by other managers, but instead is to crunch collisions and run the simulation.
*/

#include <iostream>

#include <list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "CollisionManager.hpp"
#include "CameraManager.hpp"
#include "PlayerManager.hpp"
#include "EnemyManager.hpp"
#include "PlayerManager.hpp"
#include "AssetFactory.hpp"
#include "MapManager.hpp"

CollisionManager::CollisionManager ( SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory, MapManager * inMapManager, EnemyManager * inEnemyManager, PlayerManager * inPlayerManager ) {
	myRen = inRen;
	myCameraManager = inCameraManager;
	myAssetFactory = inAssetFactory;
	myEnemyManager = inEnemyManager;
	myPlayerManager = inPlayerManager;

	myCollisionBoxLookup = new std::list<int>[1000];
	myCollisionBoxCounter = 0;

	myCollisionBoxes = new int*[1000];//Series of stacked rectangles from top to bottom stored as contiguous ranges.
	for( int i=0; i<=999; i++ ) {
		myCollisionBoxes[i] = new int[4];
	}

	doInitializeCollisions(); //TODO: Run only after level is fully loaded
}

CollisionManager::~CollisionManager ( void ) {
	delete[] myCollisionBoxLookup;

	for( int i=0; i<=999; i++ ) {
		delete[] myCollisionBoxes[i];
	}
	delete[] myCollisionBoxes;
}


//1) Match each object in the managers with their asset IDs from AssetFactory
//2) Combine that data with information from CameraManager
//3) Get their coordinants from ObjectManagers
//4) Use that to create an x/y/w/h collision box for each object
	//FUTURE: Make collision boxes stack frame by frame for finer collisions.
void CollisionManager::doInitializeCollisions ( void ) {
	//doAddCollisionRectangle( 0, 0, 0, 90, 90 );
	StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0]; //Needs to reference myCameraManager for player coords, not AssetFactory (!!) but also not PlayerManager
	SDL_Rect * myRect = &myStaticAssetPtr->myRect_dst;
	//std::cout << myRect->x << "/" << myRect->y << "/" << myRect->w << "/" << myRect->h << std::endl;
	doAddCollisionRectangle(
		0,
		myRect->x,
		myRect->y,
		myRect->x + myRect->w,
		myRect->y + myRect->h
	);
}

void CollisionManager::doAddCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 ) {
	myCollisionBoxLookup[inID].push_back( myCollisionBoxCounter );
	myCollisionBoxes[myCollisionBoxCounter][0] = inX1;
	myCollisionBoxes[myCollisionBoxCounter][1] = inY1;
	myCollisionBoxes[myCollisionBoxCounter][2] = inX1+inX2;
	myCollisionBoxes[myCollisionBoxCounter][3] = inY1+inY2;
	myCollisionBoxCounter++;
}

void CollisionManager::doUpdateCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 ) {
	myCollisionBoxes[myCollisionBoxCounter][0] = inX1;
	myCollisionBoxes[myCollisionBoxCounter][1] = inY1;
	myCollisionBoxes[myCollisionBoxCounter][2] = inX1+inX2;
	myCollisionBoxes[myCollisionBoxCounter][3] = inY1+inY2;
}

//1) Match each on-screen object to its Asset ID
//2) Construct the rect for each onscreen object by applying the ObjectManager's positional values
//3) Draw the collision boxes.
void CollisionManager::doDrawCollisionBoxes ( void ) {
	StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0];
	SDL_Rect * myRect = &myStaticAssetPtr->myRect_dst;

	SDL_SetRenderDrawColor( myRen, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for( int i=0; i<myCollisionBoxCounter; i++ ) {
		int size = myCollisionBoxLookup[i].size();
		for( int b=0; b<size; b++ ) {
			SDL_RenderDrawLine(
				myRen,
				myRect->x,
				myRect->y,
				myRect->x+myRect->w,
				myRect->y+myRect->h
			); 
		}
	}
}

//1) Test enemies against map
//2) Test player against map
//3) Test player against enemies
void CollisionManager::doGameLogic ( void ) {
	//doEnemyLogicTick();
	//doCollectiblesLogicTick();
	//doClickablesLogicTick();
	//doInteractablesLogicTick();
	myPlayerManager->doGameLogic();
}
