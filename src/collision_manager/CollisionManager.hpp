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

class CollisionManager {
public:
	CollisionManager ( Logger * inLogger, SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory, MapManager * inMapManager, EnemyManager * inEnemyManager, PlayerManager * myPlayerManager, InputManager * inInputManager, IDManager * inIDManager );
	~CollisionManager ( void );

	void doGameLogic ( void );

	void doPlayerCollisions ( void );
	void doEnemyCollisions ( void );

	void doAntCollisions(Enemy* EnemyPtr);
	void doGatorCollisions(Enemy* EnemyPtr);

	void doDrawCollisionBoxes ( void );

	inline int isWalkingPlayerCollidingLeftMapObject(int MapObjectID);
	inline int isWalkingPlayerCollidingRightMapObject(int MapObjectID);
	inline int isJumpingPlayerCollidingUpMapObject(int MapObjectID);
	inline int isFallingPlayerCollidingDownMapObject(int MapObjectID);

	inline int isFallingEnemyCollidingDownMapObject(Enemy* EnemyPtr, int MapObjectID);
	inline int isFlyingEnemyCollidingUpMapObject(Enemy* EnemyPtr, int MapObjectID);
	inline int isWalkingEnemyCollidingLeftMapObject(Enemy* EnemyPtr, int MapObjectID);
	inline int isWalkingEnemyCollidingRightMapObject(Enemy* EnemyPtr, int MapObjectID);

	void doPlayerEnemyCollisions(void);
	inline int isWalkingPlayerCollidingEnemy(Enemy* EnemyPtr);
	inline int isFallingPlayerCollidingDownEnemy(Enemy* EnemyPtr);
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
	int max_jump_peak = 5;
};

#endif
