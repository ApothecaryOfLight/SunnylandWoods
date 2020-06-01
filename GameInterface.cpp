//GameInterface.cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameInterface.hpp"

#include "AnimationManager.hpp"
#include "PlayerManager.hpp"
#include "EnemyManager.hpp"
#include "InputManager.hpp"

#include "ClickableManager.hpp"

#include "CollisionManager.hpp"
#include "LevelManager.hpp"

#include "AssetFactory.hpp"

#include "CameraManager.hpp"
#include "MapManager.hpp"

#define screenWIDTH 960
#define screenHEIGHT 624

GameInterface::GameInterface ( SDL_Renderer * inRen ) {
	myCameraManager = new CameraManager { (screenWIDTH/2)-((90*3)/2), (screenHEIGHT/2), screenWIDTH, screenHEIGHT };

	myAssetFactory = new AssetFactory( inRen, myCameraManager );

	myInputManager = new InputManager();

	myMapManager = new MapManager ( inRen, myCameraManager ); //root of the spatial context
	myClickableManager = new ClickableManager ( inRen, myMapManager, myCameraManager );
	myEnemyManager = new EnemyManager ( inRen, myMapManager, myCameraManager );
	myPlayerManager = new PlayerManager ( inRen, myInputManager, myCameraManager, myAssetFactory );
	myCollisionManager = new CollisionManager ( inRen, myCameraManager, myAssetFactory, myMapManager, myEnemyManager, myPlayerManager ); //Move all movement code here

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
