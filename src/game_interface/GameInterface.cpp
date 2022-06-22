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

GameInterface::GameInterface ( SDL_Window * inWin, SDL_Renderer * inRen, Logger * inLogger, bool* interfaceSwitched, int* interfaceID) {
	myLogger = inLogger;
	myWin = inWin;
	myIDManager = new IDManager();

	myCameraManager = new CameraManager { myLogger, 0, 50, screenWIDTH, screenHEIGHT };
	myAssetFactory = new AssetFactory( myLogger, inRen, myCameraManager );
	myInputManager = new InputManager( myLogger, myCameraManager, myAssetFactory );
	myTextManager = new TextManager(inRen,myLogger,myCameraManager);

	myMapManager = new MapManager ( inRen, myLogger, myCameraManager, myAssetFactory, myIDManager );
	myClickableManager = new ClickableManager ( inRen, myMapManager, myCameraManager, myIDManager );
	myEnemyManager = new EnemyManager ( inRen, myLogger, myAssetFactory, myMapManager, myCameraManager, myIDManager );
	myPlayerManager = new PlayerManager ( myLogger, inRen, myInputManager, myCameraManager, myAssetFactory, myIDManager );
	myCollisionManager = new CollisionManager ( myLogger, inRen, myCameraManager, myAssetFactory, myMapManager, myEnemyManager, myPlayerManager, myInputManager, myIDManager );

	myLevelManager = new LevelManager( myLogger, myMapManager, myEnemyManager, myClickableManager, myIDManager );
	myAnimationManager = new AnimationManager ( inRen, myCameraManager, myMapManager, myPlayerManager, myEnemyManager, myInputManager, myCollisionManager, myIDManager, myTextManager );

	setScreenTextFields();
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
		myLogger->log("New screen height/width: " + std::to_string(myInputManager->newWidth) + "/" + std::to_string(myInputManager->newHeight));
		myCameraManager->doResize( myInputManager->newWidth, myInputManager->newHeight );
		myCameraManager->doZoomCamera( myPlayerManager->PlayerGameCoordX, myPlayerManager->PlayerGameCoordY );
		myMapManager->doResize(myInputManager->newWidth, myInputManager->newHeight);
		myInputManager->isResized = false;
	}
	myAnimationManager->doRenderFrame();
}

void GameInterface::doProcessInput ( SDL_Event * inEvent ) {
	myInputManager->doProcessInput( inEvent );
}

void GameInterface::setScreenTextFields(void) {
	myTextManager->add_text_entity("CameraX", &myCameraManager->CameraX, 1);
	myTextManager->add_text_entity("CameraY", &myCameraManager->CameraY, 1);
	myTextManager->add_text_entity("PlayerXGameCoord", &myPlayerManager->PlayerGameCoordX, 1);
	myTextManager->add_text_entity("PlayerYGameCoord", &myPlayerManager->PlayerGameCoordY, 1);

	myTextManager->add_text_entity("mouseYGamepos", &myInputManager->mouseY_gamepos, 2);
	myTextManager->add_text_entity("mouseXGamepos", &myInputManager->mouseX_gamepos, 2);
	myTextManager->add_text_entity("mouseYPos", &myInputManager->mouseY_pos, 2);
	myTextManager->add_text_entity("mouseXPos", &myInputManager->mouseX_pos, 2);

	myTextManager->add_text_entity("Zoom", &myCameraManager->zoom, 3);
}

void GameInterface::doGameStart(void) {
	int WindowSizeX, WindowSizeY;
	SDL_GetWindowSize(myWin, &WindowSizeX, &WindowSizeY);
	myInputManager->doResize(WindowSizeX, WindowSizeY);
}

bool GameInterface::isQuit ( void ) {
	return myInputManager->isQuit;
}
