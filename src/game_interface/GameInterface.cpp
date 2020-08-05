//GameInterface.cpp
#include "GameInterface.hpp"

#include <SDL.h>
#include <SDL_image.h>

#include "../logger/logger.hpp"
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
#include "../id_manager/id_manager.hpp"

#define screenWIDTH 960
#define screenHEIGHT 624

GameInterface::GameInterface ( SDL_Renderer * inRen ) {
	myLogger = new Logger;
	myCameraManager = new CameraManager { myLogger, (screenWIDTH/2)-((90*3)/2), (screenHEIGHT/2), screenWIDTH, screenHEIGHT };

	myIDManager = new IDManager();

	myAssetFactory = new AssetFactory( myLogger, inRen, myCameraManager );

	myInputManager = new InputManager();

	myMapManager = new MapManager ( inRen, myCameraManager, myAssetFactory, myIDManager );
	myClickableManager = new ClickableManager ( inRen, myMapManager, myCameraManager, myIDManager );
	myEnemyManager = new EnemyManager ( inRen, myMapManager, myCameraManager, myIDManager );
	myPlayerManager = new PlayerManager ( myLogger, inRen, myInputManager, myCameraManager, myAssetFactory, myIDManager );
	myCollisionManager = new CollisionManager ( myLogger, inRen, myCameraManager, myAssetFactory, myMapManager, myEnemyManager, myPlayerManager, myInputManager, myIDManager );

	myLevelManager = new LevelManager( myMapManager, myEnemyManager, myClickableManager, myIDManager );
	myAnimationManager = new AnimationManager ( inRen, myMapManager, myPlayerManager, myEnemyManager, myInputManager, myCollisionManager, myIDManager );
}

GameInterface::~GameInterface ( void ) {
	delete myAnimationManager;
	delete myPlayerManager;
	delete myEnemyManager;
	delete myMapManager;
	delete myInputManager;
	delete myAssetFactory;
	delete myIDManager;
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
