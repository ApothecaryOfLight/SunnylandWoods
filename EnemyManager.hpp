//EnemyManager.hpp

#ifndef _ENEMY_MANAGER_
#define _ENEMY_MANAGER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MapManager.hpp"

#include "CameraManager.hpp"

class EnemyManager {
public:
	EnemyManager ( SDL_Renderer * inRen, MapManager * inMapManager, CameraManager * inCameraManager );
	~EnemyManager ( void );

	void doClearMyEnemies ( void );
	void doAddEnemy ( int inAssetID, int inPosX, int inPosY );

	void doEnemyTick ( int inSectorID, int inSectorX, int inSectorY );

	//void doDrawEnemies
private:
	SDL_Renderer * myRen;
	MapManager * myMapManager;

	CameraManager * myCameraManager;

	//Make additional nesting array in order to represent sectors.
	int ** myEnemies; 	//Asset ID		//X Pos			//Y Pos		//Width		//height	//Facing direction
};

#endif
