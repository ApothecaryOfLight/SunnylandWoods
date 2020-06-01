//AnimationManager.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "AnimationManager.hpp"

#include "MapManager.hpp"
#include "PlayerManager.hpp"
#include "EnemyManager.hpp"
#include "InputManager.hpp"
#include "CollisionManager.hpp"

AnimationManager::AnimationManager ( SDL_Renderer * inRen, MapManager * inMapManager, PlayerManager * inPlayerManager, EnemyManager * inEnemyManager, InputManager * inInputManager, CollisionManager * inCollisionManager ) {
	myRen = inRen;
	myMapManager = inMapManager;
	myPlayerManager = inPlayerManager;
	myEnemyManager = inEnemyManager;
	myInputManager = inInputManager;
	myCollisionManager = inCollisionManager;
}

void AnimationManager::doRenderFrame ( void ) {
	SDL_RenderClear( myRen );

	myMapManager->doRenderFrame();
	myPlayerManager->doRenderFrame();
	myCollisionManager->doDrawCollisionBoxes();

	SDL_RenderPresent(myRen);
}

void AnimationManager::doGameLogic ( void ) {
	//TODO: Replace with CollisionManager call
	myPlayerManager->doGameLogic();
}
