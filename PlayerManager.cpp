//PlayerManager.cpp

#include "PlayerManager.hpp"

#include "InputManager.hpp"

#include <string>
#include <sstream>

#include <iostream>

#include "CameraManager.hpp"

#define screenWIDTH 960
#define screenHEIGHT 624

PlayerManager::PlayerManager ( SDL_Renderer * inRen, InputManager * inInputManager, CameraManager * inCameraManager, AssetFactory * inAssetFactory ) {
	myRen = inRen;
	anim_frame_Player = 0;
	anim_frame_Player_MAX = 8;
	myInputManager = inInputManager;

	myCameraManager = inCameraManager;
	myAssetFactory = inAssetFactory;

	FPSCounter = 0;

	doLoadPlayer();
}

PlayerManager::~PlayerManager ( void ) {
	/*for( int i=0; i<8; i++ ) {
		SDL_DestroyTexture( texture_PlayerIdleFrames[i] );
	}*/
	/*for( int i=0; i<6; i++ ) {
		SDL_DestroyTexture( texture_PlayerRunFrames[i] );
	}
	for( int i=0; i<4; i++ ) {
		SDL_DestroyTexture( texture_PlayerJumpFrames[i] );
	}
	for( int i=0; i<2; i++ ) {
		SDL_DestroyTexture( texture_PlayerCrouchFrames[i] );
	}*/
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

SDL_Rect PlayerManager::doCreateRect ( int inX, int inY, int inW, int inH ) {
	SDL_Rect toRetByValue; toRetByValue.x = inX; toRetByValue.y = inY; toRetByValue.w = inW; toRetByValue.h = inH; return toRetByValue;
}

void PlayerManager::doLoadPlayer ( void ) {
	//rect_PlayerSrc = doCreateRect ( 0, 0, 90, 58 );
	//rect_PlayerDest = doCreateRect ( myCameraManager->PlayerX_screen, myCameraManager->PlayerY_screen, 90*3, 58*3 ); //TODO: Tie this into CameraManager
	//rect_PlayerDest.x = myCameraManager->PlayerX_screen;
	//rect_PlayerDest.x = myCameraManager->PlayerY_screen;

	//doLoadPlayerAnimationCycle( texture_PlayerIdleFrames, "media/SPRITES/player/idle/player-idle-", 8 );
	//doLoadPlayerAnimationCycle( texture_PlayerRunFrames, "media/SPRITES/player/run/player-run-", 6 );
	//doLoadPlayerAnimationCycle( texture_PlayerJumpFrames, "media/SPRITES/player/jump/player-jump-", 4 );
	//doLoadPlayerAnimationCycle( texture_PlayerCrouchFrames, "media/SPRITES/player/crouch/player-crouch-", 2 );
}

//TODO: Seperate animation code and game logic code. This function shouldn't set movement and positioning, just draw them.
void PlayerManager::doRenderFrame ( void ) {
	FPSCounter++;
	if( myInputManager->inputFlag_Jumping == true || myInputManager->inputFlag_Right == true || myInputManager->inputFlag_Left == true  ) {
		if( FPSCounter >= 2 ) {
			anim_frame_Player++;
			if( anim_frame_Player >= anim_frame_Player_MAX ) {
				anim_frame_Player = 0;
			}
			FPSCounter = 0;
		}
	}
	else if( myInputManager->inputFlag_Right == false || myInputManager->inputFlag_Left == false || myInputManager->inputFlag_Jumping == false ) {
		if( FPSCounter >= 3 ) {
			anim_frame_Player++;
			if( anim_frame_Player >= anim_frame_Player_MAX ) {
				anim_frame_Player = 0;
			}
			FPSCounter = 0;
		}
	}


	//TODO: Needs adjustment for half of player sprite width at the time.
	if( myInputManager->inputFlag_Left == true && myInputManager->inputFlag_Right == false ) { //Logic for moving left
		myCameraManager->PlayerX_screen = (myCameraManager->PlayerX_screen)-9;
		if( myCameraManager->PlayerX_screen <= myCameraManager->ScreenWall_Left ) {
			myCameraManager->PlayerX_screen = myCameraManager->ScreenWall_Left;
			myCameraManager->PlayerX_level = myCameraManager->PlayerX_level-9;
		}
		myAssetFactory->doAdjustPlayerDest( myCameraManager->PlayerX_screen );
	}
	else if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == true ) { //Logic for moving right
		myCameraManager->PlayerX_screen = (myCameraManager->PlayerX_screen)+9;
		if( myCameraManager->PlayerX_screen + myCameraManager->PlayerSize_X >= myCameraManager->ScreenWall_Right ) {
			myCameraManager->PlayerX_screen = myCameraManager->ScreenWall_Right - myCameraManager->PlayerSize_X;
			myCameraManager->PlayerX_level = myCameraManager->PlayerX_level+9;
		}
		myAssetFactory->doAdjustPlayerDest( myCameraManager->PlayerX_screen );
	}


	if( myInputManager->inputFlag_Jumping == true ) {
		anim_frame_Player_MAX = 4;
		if( anim_frame_Player >= 4 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[4]->myStaticAssets[anim_frame_Player];
			if( myInputManager->isPlayerFacingLeft == true ) { //Jumping left
				SDL_RenderCopyEx(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(myStaticAssetPtr->myRect_dst),
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
					&(myStaticAssetPtr->myRect_dst)
				);
			}
	}
	else if ( myInputManager->inputFlag_Jumping == false ) {
		if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == false && myInputManager->inputFlag_Crouching == true ) { //Crouching
			anim_frame_Player_MAX = 2;
			if( anim_frame_Player >= 2 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[3]->myStaticAssets[anim_frame_Player];
			if( myInputManager->isPlayerFacingLeft == true ) {
				SDL_RenderCopyEx(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(myStaticAssetPtr->myRect_dst),
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
			if( anim_frame_Player >= 6 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[1]->myStaticAssets[anim_frame_Player];
			SDL_RenderCopyEx(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(myStaticAssetPtr->myRect_dst),
					0,
					NULL,
					SDL_FLIP_HORIZONTAL
				);
		}
		else if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == true ) { //Running right
			anim_frame_Player_MAX = 6;
			if( anim_frame_Player >= 6 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[1]->myStaticAssets[anim_frame_Player];
			SDL_RenderCopy(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(myStaticAssetPtr->myRect_dst)
				);
		}
		else {
			anim_frame_Player_MAX = 8;
			if( anim_frame_Player >= 8 ) { anim_frame_Player = 0; }
			StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[anim_frame_Player];
			if( myInputManager->isPlayerFacingLeft == true ) { //Idle left
				SDL_RenderCopyEx( myRen, myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(myStaticAssetPtr->myRect_dst),
					0, NULL, SDL_FLIP_HORIZONTAL
				);
			}
			else if( myInputManager->isPlayerFacingLeft == false ) { //Idle right
				SDL_RenderCopy(
					myRen,
					myStaticAssetPtr->myTexture,
					&(myStaticAssetPtr->myRect_src),
					&(myStaticAssetPtr->myRect_dst)
				);
			}
		}
	}
}

void PlayerManager::doGameLogic ( void ) {
	//TODO: Likely remove this in favor of game logic functions elsewhere.
}
