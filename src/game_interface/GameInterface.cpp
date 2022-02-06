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

#include "../text_manager/text_manager.hpp"

#define screenWIDTH 1024
#define screenHEIGHT 640

GameInterface::GameInterface ( SDL_Window * inWin, SDL_Renderer * inRen, Logger * inLogger ) {
	myLogger = inLogger;
	myCameraManager = new CameraManager { myLogger, 0, 50, screenWIDTH, screenHEIGHT };
	myTextManager = new TextManager(inRen,myLogger,myCameraManager);

	myIDManager = new IDManager();

	myAssetFactory = new AssetFactory( myLogger, inRen, myCameraManager );

	myInputManager = new InputManager( myCameraManager );

	myMapManager = new MapManager ( inRen, myLogger, myCameraManager, myAssetFactory, myIDManager );
	myClickableManager = new ClickableManager ( inRen, myMapManager, myCameraManager, myIDManager );
	myEnemyManager = new EnemyManager ( inRen, myLogger, myAssetFactory, myMapManager, myCameraManager, myIDManager );
	myPlayerManager = new PlayerManager ( myLogger, inRen, myInputManager, myCameraManager, myAssetFactory, myIDManager );
	myCollisionManager = new CollisionManager ( myLogger, inRen, myCameraManager, myAssetFactory, myMapManager, myEnemyManager, myPlayerManager, myInputManager, myIDManager );

	myLevelManager = new LevelManager( myMapManager, myEnemyManager, myClickableManager, myIDManager );
	myAnimationManager = new AnimationManager ( inRen, myCameraManager, myMapManager, myPlayerManager, myEnemyManager, myInputManager, myCollisionManager, myIDManager, myTextManager );
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
	myEnemyManager->doAnimateEnemy();
	myAnimationManager->doGameLogic();
}

void GameInterface::doRenderFrame ( void ) {
	if( myInputManager->isResized ) {
		myCameraManager->doResize( myInputManager->newWidth, myInputManager->newHeight );
		myCameraManager->doInitializeCamera( myPlayerManager->PlayerGameCoordX, myPlayerManager->PlayerGameCoordY );
		myAssetFactory->doResize( myInputManager->newWidth, myInputManager->newHeight );
		myMapManager->doResize(myInputManager->newWidth, myInputManager->newHeight);
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
