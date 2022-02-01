//PlayerManager.cpp
#include "PlayerManager.hpp"

#include <string>
#include <sstream>
#include <iostream>

#include "../logger/logger.hpp"
#include "../input_manager/InputManager.hpp"
#include "../camera_manager/CameraManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../id_manager/id_manager.hpp"

#define screenWIDTH 960
#define screenHEIGHT 624

PlayerManager::PlayerManager ( Logger * inLogger, SDL_Renderer * inRen, InputManager * inInputManager, CameraManager * inCameraManager, AssetFactory * inAssetFactory, IDManager * inIDManager ) {
	myLogger = inLogger;
	myRen = inRen;
	anim_frame_Player = 0;
	anim_frame_Player_MAX = 8;
	PlayerAnimationType = 0;
	myInputManager = inInputManager;

	myCameraManager = inCameraManager;
	myAssetFactory = inAssetFactory;
	myIDManager = inIDManager;

	FPSCounter = 0;
	jump_counter = 0;

	PlayerGameCoordX = 0;
	PlayerGameCoordY = -80;

	myCameraManager->doInitializeCamera(PlayerGameCoordX, PlayerGameCoordY);
}

PlayerManager::~PlayerManager ( void ) {
}

void PlayerManager::doLoadPlayerAnimationCycle ( SDL_Texture ** DestTextureArray, std::string inFileLocation, int inLength ) {
	for( int i=1; i<=inLength; i++ ) {
		std::stringstream myStringstreamConv;
		myStringstreamConv << i;
		std::string myConv = myStringstreamConv.str();
		SDL_Surface *myNewSurface = IMG_Load( std::string(inFileLocation+myConv+".png").c_str() );
		if(!myNewSurface) { std::cout << IMG_GetError(); }
		DestTextureArray[i-1] = SDL_CreateTextureFromSurface( myRen, myNewSurface );
		SDL_FreeSurface( myNewSurface );
	}
}

void PlayerManager::doGameLogic ( void ) {

}

SDL_Rect PlayerManager::getPlayerDest(void) {
	SDL_Rect PlayerDest;
	PlayerDest.x = (PlayerGameCoordX - myCameraManager->CameraX);
	PlayerDest.y = (PlayerGameCoordY - myCameraManager->CameraY);
	return PlayerDest;
}

void PlayerManager::doRenderFrame ( void ) {
	SDL_Rect PlayerDest = getPlayerDest();
	if( jump_counter != 0 ) {
		anim_frame_Player_MAX = 4;
		PlayerAnimationType = 4;
		if( anim_frame_Player >= 4 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[4]->myStaticAssets[anim_frame_Player];
			PlayerDest.w = myStaticAssetPtr->myRect_dst.w;
			PlayerDest.h = myStaticAssetPtr->myRect_dst.h;
			if( myInputManager->isPlayerFacingLeft == true ) { //Jumping left
				SDL_RenderCopyEx(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(PlayerDest),
					0,
					NULL,
					SDL_FLIP_HORIZONTAL
				);
			}
			else if( myInputManager->isPlayerFacingLeft == false ) { //Jumping right
				SDL_RenderCopy(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(PlayerDest)
				);
			}
	}
	else if ( myInputManager->inputFlag_Jumping == false ) {
		if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == false && myInputManager->inputFlag_Crouching == true ) { //Crouching
			anim_frame_Player_MAX = 2;
			PlayerAnimationType = 3;
			if( anim_frame_Player >= 2 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[3]->myStaticAssets[anim_frame_Player];
			PlayerDest.w = myStaticAssetPtr->myRect_dst.w;
			PlayerDest.h = myStaticAssetPtr->myRect_dst.h;
			if( myInputManager->isPlayerFacingLeft == true ) {
				SDL_RenderCopyEx(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(PlayerDest),
					0,
					NULL,
					SDL_FLIP_HORIZONTAL
				);
			}
			else if( myInputManager->isPlayerFacingLeft == false ) {
				SDL_RenderCopy(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(myStaticAssetPtr->myRect_dst)
				);
			}
		}
		else if( myInputManager->inputFlag_Left == true && myInputManager->inputFlag_Right == false ) {
			anim_frame_Player_MAX = 6;
			PlayerAnimationType = 1;
			if( anim_frame_Player >= 6 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[1]->myStaticAssets[anim_frame_Player];
			PlayerDest.w = myStaticAssetPtr->myRect_dst.w;
			PlayerDest.h = myStaticAssetPtr->myRect_dst.h;
			SDL_RenderCopyEx(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(PlayerDest),
					0,
					NULL,
					SDL_FLIP_HORIZONTAL
				);
		}
		else if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == true ) { //Running right
			anim_frame_Player_MAX = 6;
			PlayerAnimationType = 1;
			if( anim_frame_Player >= 6 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[1]->myStaticAssets[anim_frame_Player];
			SDL_Rect PlayerCopy = myStaticAssetPtr->myRect_dst;
			PlayerDest.w = myStaticAssetPtr->myRect_dst.w;
			PlayerDest.h = myStaticAssetPtr->myRect_dst.h;
			PlayerDest.x -= 18;
			SDL_RenderCopy(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					(&PlayerDest)
				);
		}
		else {
			anim_frame_Player_MAX = 8;
			PlayerAnimationType = 0;
			if( anim_frame_Player >= 8 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[anim_frame_Player];
			PlayerDest.w = myStaticAssetPtr->myRect_dst.w;
			PlayerDest.h = myStaticAssetPtr->myRect_dst.h;
			if( myInputManager->isPlayerFacingLeft == true ) { //Idle left
				//myLogger->log( "Idling left." );
				SDL_RenderCopyEx( myRen, myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(PlayerDest),
					0, NULL, SDL_FLIP_HORIZONTAL
				);
			}
			else if( myInputManager->isPlayerFacingLeft == false ) { //Idle right
				SDL_RenderCopy(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(PlayerDest)
				);
			}
		}
	}
}

void PlayerManager::doAnimatePlayer(void) {
	FPSCounter++;
	if (myInputManager->inputFlag_Jumping == true || myInputManager->inputFlag_Right == true || myInputManager->inputFlag_Left == true) {
		if (FPSCounter >= 2) {
			anim_frame_Player++;
			if (anim_frame_Player >= anim_frame_Player_MAX) {
				anim_frame_Player = 0;
			}
			FPSCounter = 0;
		}
	}
	else if (myInputManager->inputFlag_Right == false || myInputManager->inputFlag_Left == false || myInputManager->inputFlag_Jumping == false) {
		if (FPSCounter >= 3) {
			anim_frame_Player++;
			if (anim_frame_Player >= anim_frame_Player_MAX) {
				anim_frame_Player = 0;
			}
			FPSCounter = 0;
		}
	}
}