//AnimationManager.hpp
#include "AnimationManager.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "../camera_manager/CameraManager.hpp"
#include "../map_manager/MapManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"
#include "../input_manager/InputManager.hpp"
#include "../collision_manager/CollisionManager.hpp"
#include "../id_manager/id_manager.hpp"
#include "../text_manager/text_manager.hpp"

AnimationManager::AnimationManager ( SDL_Renderer * inRen, CameraManager* inCameraManager, MapManager * inMapManager, PlayerManager * inPlayerManager, EnemyManager * inEnemyManager, InputManager * inInputManager, CollisionManager * inCollisionManager, IDManager * inIDManager, TextManager * inTextManager ) {
	myRen = inRen;
	myCameraManager = inCameraManager;
	myMapManager = inMapManager;
	myPlayerManager = inPlayerManager;
	myEnemyManager = inEnemyManager;
	myInputManager = inInputManager;
	myCollisionManager = inCollisionManager;
	myIDManager = inIDManager;
	myTextManager = inTextManager;
}

void AnimationManager::doGameLogic ( void ) {
	myCollisionManager->doGameLogic();
}

void AnimationManager::doRenderFrame ( void ) {
	SDL_RenderClear( myRen );

	myMapManager->doRenderFrame();
	myPlayerManager->doAnimatePlayer();
	myPlayerManager->doRenderFrame();
	myEnemyManager->doRenderFrame();

	if (myInputManager->isPressed_F5) {
		//myCollisionManager->doDrawCollisionBoxes();
		myTextManager->draw_text_entities();
	}

	SDL_RenderPresent(myRen);
}