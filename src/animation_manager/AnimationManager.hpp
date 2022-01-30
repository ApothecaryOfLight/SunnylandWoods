//../animation_manager/AnimationManager.hpp

#ifndef _ANIMATION_MANAGER_
#define _ANIMATION_MANAGER_

#include <SDL.h>
#include <SDL_image.h>

#include "../map_manager/MapManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"
#include "../input_manager/InputManager.hpp"
#include "../collision_manager/CollisionManager.hpp"
#include "../id_manager/id_manager.hpp"
#include "../text_manager/text_manager.hpp"
#include "../camera_manager/CameraManager.hpp"

class AnimationManager {
public:
	AnimationManager ( SDL_Renderer * inRen, CameraManager* inCameraManager, MapManager * inMapManager, PlayerManager * inPlayerManager, EnemyManager * inEnemyManager, InputManager * inInputManager, CollisionManager * inCollisionManager, IDManager * inIDManager, TextManager* inTextManager );
	void doGameLogic ( void );
	void doRenderFrame ( void );
private:
	SDL_Renderer * myRen;
	CameraManager* myCameraManager;
	MapManager * myMapManager;
	PlayerManager * myPlayerManager;
	EnemyManager * myEnemyManager;
	InputManager * myInputManager;
	CollisionManager * myCollisionManager;
	IDManager * myIDManager;

	TextManager* myTextManager;
};

#endif
