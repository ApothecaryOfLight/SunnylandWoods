//EnemyManager.hpp

#ifndef _ENEMY_MANAGER_
#define _ENEMY_MANAGER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../map_manager/MapManager.hpp"
#include "../camera_manager/CameraManager.hpp"
#include "../id_manager/id_manager.hpp"

class EnemyManager {
public:
	EnemyManager ( SDL_Renderer * inRen, MapManager * inMapManager, CameraManager * inCameraManager, IDManager * inIDManager );
	~EnemyManager ( void );

	void doClearMyEnemies ( void );
	void doAddEnemy ( int inAssetID, int inPosX, int inPosY );

	void doEnemyTick ( int inSectorID, int inSectorX, int inSectorY );

	//void doDrawEnemies
private:
	SDL_Renderer * myRen;
	MapManager * myMapManager;

	CameraManager * myCameraManager;
	IDManager * myIDManager;

	//Make additional nesting array in order to represent sectors.
	int ** myEnemies; 	//Asset ID		//X Pos			//Y Pos		//Width		//height	//Facing direction
};

#endif
