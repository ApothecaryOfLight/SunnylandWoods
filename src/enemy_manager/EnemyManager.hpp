//EnemyManager.hpp

#ifndef _ENEMY_MANAGER_
#define _ENEMY_MANAGER_

#include <SDL.h>
#include <SDL_image.h>

#include "../logger/logger.hpp"
#include "../map_manager/MapManager.hpp"
#include "../camera_manager/CameraManager.hpp"
#include "../id_manager/id_manager.hpp"

class Enemy {
public:
	Enemy();
	Enemy(int XPos, int YPos, int inAssetID);
	int EnemyGameCoordX, EnemyGameCoordY;
	int AssetID;
	int Frame;
	bool isFacingLeft;
};

class EnemyManager {
public:
	EnemyManager ( SDL_Renderer * inRen, Logger* inLogger, AssetFactory* myAssetFactory, MapManager * inMapManager, CameraManager * inCameraManager, IDManager * inIDManager );
	~EnemyManager ( void );

	void doAddEnemy ( int inAssetID, int inPosX, int inPosY );

	void doEnemyTick ();

	void doRenderFrame();

	std::list<Enemy> myEnemies;
private:
	SDL_Renderer * myRen;
	Logger* myLogger;
	MapManager * myMapManager;
	CameraManager * myCameraManager;
	IDManager * myIDManager;
	AssetFactory* myAssetFactory;


	int tick_counter;
};

#endif
