//CollisionManager.cpp

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

//Build a list of all collision boxes for all entities in the sector, bound to their frames, with x/y adjustments for each collision set.
//Store all collision boxes as SDL_Rects and use SDL is intersecting to test.
//Lookup by: world ID, Asset ID, xpos, ypos, frame, adjX, adjY
//Will test all the collision boxes of that entity against all collision boxes in the sector and in adjacent sectors
void CollisionManager::doInitializeCollisions ( void ) {
	//doAddCollisionRectangle( 0, 0, 0, 90, 90 );
	SDL_Rect * myRect = &myPlayerManager->rect_PlayerDest;
	std::cout << myRect->x << "/" << myRect->y << "/" << myRect->w << "/" << myRect->h << std::endl;
	doAddCollisionRectangle( 0, myRect->x, myRect->y, myRect->w, myRect->h );
}

void CollisionManager::doAddCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 ) {
	myCollisionBoxLookup[inID].push_back( myCollisionBoxCounter );
	myCollisionBoxes[myCollisionBoxCounter][0] = inX1;
	myCollisionBoxes[myCollisionBoxCounter][1] = inY1;
	myCollisionBoxes[myCollisionBoxCounter][2] = inX1+inX2;
	myCollisionBoxes[myCollisionBoxCounter][3] = inY1+inY2;
	myCollisionBoxCounter++;
}

void CollisionManager::doDrawCollisionBoxes ( void ) {
	SDL_Rect * myRect = &myPlayerManager->rect_PlayerDest;
	std::cout << myRect->x << "/" << myRect->y << "/" << myRect->w << "/" << myRect->h << std::endl;
	

	SDL_SetRenderDrawColor( myRen, 255, 0, 0, SDL_ALPHA_OPAQUE);
	for( int i=0; i<myCollisionBoxCounter; i++ ) {
		int size = myCollisionBoxLookup[i].size();
		for( int b=0; b<size; b++ ) {
			SDL_RenderDrawLine( myRen, myRect->x, myRect->y, myRect->x+myRect->w, myRect->y+myRect->h  ); 
		}
	}
}

void CollisionManager::doGameLogicTick ( void ) {
	
}
