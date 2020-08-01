//EnemyManager.cpp
#include "EnemyManager.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "../map_manager/MapManager.hpp"
#include "../camera_manager/CameraManager.hpp"
#include "../id_manager/id_manager.hpp"

EnemyManager::EnemyManager ( SDL_Renderer * inRen, MapManager * inMapManager, CameraManager * inCameraManager, IDManager * inIDManager ) {
	myRen = inRen;
	myMapManager = inMapManager;
	myCameraManager = inCameraManager;
	myIDManager = inIDManager;

	myEnemies = new int*[100];
	for( int i=0; i++; i<100 ) {
		myEnemies[i] = new int[4];
	}
}

EnemyManager::~EnemyManager ( void ) {
	for( int i=0; i++; i<100 ) {
		delete[] myEnemies[i];
	}
	delete[] myEnemies;
}

void EnemyManager::doClearMyEnemies ( void ) {
	for( int x=0; x<100; x++ ) {
		myEnemies[x][0] = -1;
	}
}

void EnemyManager::doAddEnemy ( int inAssetID, int inPosX, int inPosY ) {
	for( int x=0; x<100; x++ ) {
		if( myEnemies[x][0] != -1 ) {
			myEnemies[x][0] = inAssetID;
			myEnemies[x][1] = inPosX;
			myEnemies[x][2] = inPosY;
			myEnemies[x][3] = 0;//TODO: Randomize
		}
	}
}

void doEnemyTick ( int inSectorID, int inSectorX, int inSectorY ) {
	int *mySectorsToCheck[9];
	//mySectorsToCheck[0] = 
	//int **myLocalEnemies = myEnemies[inSectorID];
}

