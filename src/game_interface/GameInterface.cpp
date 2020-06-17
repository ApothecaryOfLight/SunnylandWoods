//GameInterface.cpp
#include "GameInterface.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../animation_manager/AnimationManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"
#include "../input_manager/InputManager.hpp"
#include "../clickable_manager/ClickableManager.hpp"
#include "../collision_manager/CollisionManager.hpp"
#include "../level_manager/LevelManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../camera_manager/CameraManager.hpp"
#include "../map_manager/MapManager.hpp"

#define screenWIDTH 960
#define screenHEIGHT 624

GameInterface::GameInterface ( SDL_Renderer * inRen ) {
	myCameraManager = new CameraManager { (screenWIDTH/2)-((90*3)/2), (screenHEIGHT/2), screenWIDTH, screenHEIGHT };

	myAssetFactory = new AssetFactory( inRen, myCameraManager );

	myInputManager = new InputManager();

	myMapManager = new MapManager ( inRen, myCameraManager, myAssetFactory ); //root of the spatial context
	myClickableManager = new ClickableManager ( inRen, myMapManager, myCameraManager );
	myEnemyManager = new EnemyManager ( inRen, myMapManager, myCameraManager );
	myPlayerManager = new PlayerManager ( inRen, myInputManager, myCameraManager, myAssetFactory );
	myCollisionManager = new CollisionManager ( inRen, myCameraManager, myAssetFactory, myMapManager, myEnemyManager, myPlayerManager, myInputManager ); //Move all movement code here

	myLevelManager = new LevelManager( myMapManager, myEnemyManager, myClickableManager );
	myAnimationManager = new AnimationManager ( inRen, myMapManager, myPlayerManager, myEnemyManager, myInputManager, myCollisionManager );
}

GameInterface::~GameInterface ( void ) {
	delete myAnimationManager;
	delete myPlayerManager;
	delete myEnemyManager;
	delete myMapManager;
	delete myInputManager;
	delete myAssetFactory;
	delete myCameraManager;
}

void GameInterface::doGameLogic ( void ) {
	myAnimationManager->doGameLogic();
}

void GameInterface::doRenderFrame ( void ) {
	if( myInputManager->isResized ) {
		myCameraManager->doResize( myInputManager->newWidth, myInputManager->newHeight );
		myAssetFactory->doResize();
		myInputManager->isResized = false;
	}
	myAnimationManager->doRenderFrame();
}

void GameInterface::doProcessInput ( SDL_Event * inEvent ) {
	myInputManager->doProcessInput( inEvent );
}

bool GameInterface::isQuit ( void ) {
	return myInputManager->isQuit;
}
