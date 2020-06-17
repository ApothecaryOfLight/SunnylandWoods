//../animation_manager/AnimationManager.hpp

#ifndef _ANIMATION_MANAGER_
#define _ANIMATION_MANAGER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../map_manager/MapManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"
#include "../input_manager/InputManager.hpp"
#include "../collision_manager/CollisionManager.hpp"

class AnimationManager {
public:
	AnimationManager ( SDL_Renderer * inRen, MapManager * inMapManager, PlayerManager * inPlayerManager, EnemyManager * inEnemyManager, InputManager * inInputManager, CollisionManager * inCollisionManager );
	void doGameLogic ( void );
	void doRenderFrame ( void );
private:
	SDL_Renderer * myRen;
	MapManager * myMapManager;
	PlayerManager * myPlayerManager;
	EnemyManager * myEnemyManager;
	InputManager * myInputManager;
	CollisionManager * myCollisionManager;
};

#endif
