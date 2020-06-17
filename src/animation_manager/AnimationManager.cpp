//AnimationManager.hpp
#include "AnimationManager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../map_manager/MapManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"
#include "../input_manager/InputManager.hpp"
#include "../collision_manager/CollisionManager.hpp"

AnimationManager::AnimationManager ( SDL_Renderer * inRen, MapManager * inMapManager, PlayerManager * inPlayerManager, EnemyManager * inEnemyManager, InputManager * inInputManager, CollisionManager * inCollisionManager ) {
	myRen = inRen;
	myMapManager = inMapManager;
	myPlayerManager = inPlayerManager;
	myEnemyManager = inEnemyManager;
	myInputManager = inInputManager;
	myCollisionManager = inCollisionManager;
}

void AnimationManager::doGameLogic ( void ) {
	myCollisionManager->doGameLogic();
}

void AnimationManager::doRenderFrame ( void ) {
	SDL_RenderClear( myRen );

	myMapManager->doRenderFrame();
	myPlayerManager->doRenderFrame();
	myCollisionManager->doDrawCollisionBoxes();

	SDL_RenderPresent(myRen);
}
