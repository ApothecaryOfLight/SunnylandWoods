//EnemyManager.cpp
#include "EnemyManager.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "../logger/logger.hpp"
#include "../map_manager/MapManager.hpp"
#include "../camera_manager/CameraManager.hpp"
#include "../id_manager/id_manager.hpp"

Enemy::Enemy() {

}

Enemy::Enemy(int XPos, int YPos, int inAssetID) {
	EnemyGameCoordX = XPos;
	EnemyGameCoordY = YPos;
	AssetID = inAssetID;
	isFacingLeft = false;
	Frame = 0;
}


EnemyManager::EnemyManager ( SDL_Renderer * inRen, Logger* inLogger, AssetFactory* inAssetFactory, MapManager * inMapManager, CameraManager * inCameraManager, IDManager * inIDManager ) {
	myRen = inRen;
	myLogger = inLogger;
	myAssetFactory = inAssetFactory;
	myMapManager = inMapManager;
	myCameraManager = inCameraManager;
	myIDManager = inIDManager;

	doAddEnemy(5, 114, -298);
	doAddEnemy(5, -14, -298);
	doAddEnemy(5, -114, -298);
	doAddEnemy(5, -14, -398);
}

EnemyManager::~EnemyManager ( void ) {

}

void EnemyManager::doAddEnemy ( int inAssetID, int inPosX, int inPosY ) {
	myEnemies.push_back(Enemy(inPosX, inPosY, inAssetID));
}

void EnemyManager::doDeleteEnemy(std::list<Enemy>::iterator inEnemyListPos) {
	myEnemies.erase(inEnemyListPos);
}

void EnemyManager::doAnimateEnemy () {
	std::list<Enemy>::iterator myIter = myEnemies.begin(), myEnd = myEnemies.end();
	tick_counter++;
	if (tick_counter > 2) {
		while (myIter != myEnd) {
			Enemy* myEnemy = &(*myIter);
			int max_frames = myAssetFactory->myAnimatedAssets[myEnemy->AssetID]->frames;
			myEnemy->Frame++;
			if (myEnemy->Frame >= max_frames) {
				myEnemy->Frame = 0;
				if (myEnemy->AssetID == 6) {
					myEnemies.erase(myIter);
				}
			}
			++myIter;
		}
		tick_counter = 0;
	}
}

void EnemyManager::doRenderFrame() {
	std::list<Enemy>::iterator myIter = myEnemies.begin(), myEnd = myEnemies.end();
	while (myIter != myEnd) {
		Enemy* myEnemy = &(*myIter);
		int myAssetID = myEnemy->AssetID;
		StaticAsset* myStaticAssetPtr = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[myEnemy->Frame];
		SDL_Rect enemy_dst;
		enemy_dst.x = (myEnemy->EnemyGameCoordX * myCameraManager->zoom) - myCameraManager->CameraX;
		enemy_dst.y = (myEnemy->EnemyGameCoordY * myCameraManager->zoom) - myCameraManager->CameraY;
		enemy_dst.w = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[0]->myRect_dst.w;
		enemy_dst.h = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[0]->myRect_dst.h;
		if (!myEnemy->isFacingLeft) {
			SDL_RenderCopyEx(
				myRen,
				myStaticAssetPtr->myTexture,
				&(myStaticAssetPtr->myRect_src),
				&(enemy_dst),
				0,
				NULL,
				SDL_FLIP_HORIZONTAL
			);
		}
		else {
			SDL_RenderCopy(
				myRen,
				myStaticAssetPtr->myTexture,
				&(myStaticAssetPtr->myRect_src),
				&(enemy_dst)
			);
		}
		++myIter;
	}
}
