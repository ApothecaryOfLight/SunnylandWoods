#include <list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "CameraManager.hpp"
#include "PlayerManager.hpp"
#include "EnemyManager.hpp"
#include "PlayerManager.hpp"
#include "AssetFactory.hpp"

#ifndef _COLLISION_MANAGER_
#define _COLLISION_MANAGER_

class CollisionManager {
public:
	CollisionManager ( SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory, MapManager * inMapManager, EnemyManager * inEnemyManager, PlayerManager * myPlayerManager );
	~CollisionManager ( void );

	void doInitializeCollisions ( void );

	void doAddCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 );
	void doUpdateCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 );

	void doGameLogic ( void );

	void doDrawCollisionBoxes ( void );
private:
	SDL_Renderer * myRen;

	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
	EnemyManager * myEnemyManager;
	PlayerManager * myPlayerManager;

	int myCollisionBoxCounter;
	std::list<int> retiredCollisionBoxIDs;
	int ** myCollisionBoxes;
	std::list<int> * myCollisionBoxLookup;
};

#endif
