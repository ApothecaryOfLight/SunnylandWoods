#include <list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../camera_manager/CameraManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../map_manager/MapManager.hpp"
#include "../input_manager/InputManager.hpp"
#include "../id_manager/id_manager.hpp"

#ifndef _COLLISION_MANAGER_
#define _COLLISION_MANAGER_

class CollisionManager {
public:
	CollisionManager ( SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory, MapManager * inMapManager, EnemyManager * inEnemyManager, PlayerManager * myPlayerManager, InputManager * inInputManager, IDManager * inIDManager );
	~CollisionManager ( void );

	void doInitializeCollisions ( void );

	void doAddCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 );
	void doUpdateCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 );

	void doGameLogic ( void );

	void doPlayerCollisions ( void );
	void doEnemyCollisions ( void );

	void doDrawCollisionBoxes ( void );
private:
	SDL_Renderer * myRen;

	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
	EnemyManager * myEnemyManager;
	PlayerManager * myPlayerManager;
	MapManager * myMapManager;
	InputManager * myInputManager;
	IDManager * myIDManager;

	//int myCollisionBoxCounter;
	//std::list<int> retiredCollisionBoxIDs;
	SDL_Rect * myCollisionBoxes;
	//std::list<int> * myCollisionBoxLookup;
};

#endif
