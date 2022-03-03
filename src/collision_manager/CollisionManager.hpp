#include <list>

#include <SDL.h>
#include <SDL_image.h>

#include "../camera_manager/CameraManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../map_manager/MapManager.hpp"
#include "../input_manager/InputManager.hpp"
#include "../id_manager/id_manager.hpp"

#include "../logger/logger.hpp"

#ifndef _COLLISION_MANAGER_
#define _COLLISION_MANAGER_

class vector2d {
public:
	vector2d(int x, int y);
	int x, y;
	int magnitude();
};

class CollisionBox {
public:
	CollisionBox(int x, int y, int w, int h);
	int x, y, w, h;
};

class CollisionManager {
public:
	CollisionManager ( Logger * inLogger, SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory, MapManager * inMapManager, EnemyManager * inEnemyManager, PlayerManager * myPlayerManager, InputManager * inInputManager, IDManager * inIDManager );
	~CollisionManager ( void );

	void doGameLogic ( void );

	void doPlayerCollisions ( void );
	void doEnemyCollisions ( void );

	void doDrawCollisionBoxes ( void );

	inline int isWalkingPlayerCollidingLeftMapObject(int MapObjectID);
	inline int isWalkingPlayerCollidingRightMapObject(int MapObjectID);
	inline int isJumpingPlayerCollidingUpMapObject(int MapObjectID);
	inline int isFallingPlayerCollidingDownMapObject(int MapObjectID);

	inline int isFallingEnemyCollidingDownMapObject(Enemy* EnemyPtr, int MapObjectID);
	inline int isWalkingEnemyCollidingLeftMapObject(Enemy* EnemyPtr, int MapObjectID);
	inline int isWalkingEnemyCollidingRightMapObject(Enemy* EnemyPtr, int MapObjectID);

	void doPlayerEnemyCollisions(void);
	inline int isWalkingPlayerCollidingEnemy(Enemy* EnemyPtr);
	inline int isFallingPlayerCollidingDownEnemy(Enemy* EnemyPtr);

	inline int isColliding(CollisionBox ObjectA, CollisionBox ObjectB, vector2d ObjectAMovement);
private:
	SDL_Renderer * myRen;

	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
	EnemyManager * myEnemyManager;
	PlayerManager * myPlayerManager;
	MapManager * myMapManager;
	InputManager * myInputManager;
	IDManager * myIDManager;

	Logger * myLogger;

	int max_jump_height = 15;
};

#endif
