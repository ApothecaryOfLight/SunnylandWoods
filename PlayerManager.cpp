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
	PlayerAnimationType = 0;
	myInputManager = inInputManager;

	myCameraManager = inCameraManager;
	myAssetFactory = inAssetFactory;

	FPSCounter = 0;
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
	/*FPSCounter++;
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

	if( myInputManager->inputFlag_Left == true && myInputManager->inputFlag_Right == false ) { //Logic for moving left
		myCameraManager->PlayerX_screen = (myCameraManager->PlayerX_screen)-9;
		if( myCameraManager->PlayerX_screen <= myCameraManager->ScreenWall_Left ) {
			myCameraManager->PlayerX_screen = myCameraManager->ScreenWall_Left;
			myCameraManager->PlayerX_level = myCameraManager->PlayerX_level-9;
		}
		myAssetFactory->doAdjustPlayerDest( myCameraManager->PlayerX_screen ); //TODO: Remove, AssetFactory shouldn't track object positions, they're not the same thing.
	}
	else if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == true ) { //Logic for moving right
		myCameraManager->PlayerX_screen = (myCameraManager->PlayerX_screen)+9;
		if( myCameraManager->PlayerX_screen + (myCameraManager->PlayerSize_X*myCameraManager->magnification) >= myCameraManager->ScreenWall_Right ) {
			myCameraManager->PlayerX_screen = myCameraManager->ScreenWall_Right - (myCameraManager->PlayerSize_X*myCameraManager->magnification);
			myCameraManager->PlayerX_level = myCameraManager->PlayerX_level+9;
		}
		myAssetFactory->doAdjustPlayerDest( myCameraManager->PlayerX_screen ); //TODO: Remove, AssetFactory shouldn't track object positions, they're not the same thing.
	}*/
}

void PlayerManager::doRenderFrame ( void ) {
	if( myInputManager->inputFlag_Jumping == true ) {
		anim_frame_Player_MAX = 4;
		PlayerAnimationType = 4;
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
			PlayerAnimationType = 3;
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
			PlayerAnimationType = 1;
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
			PlayerAnimationType = 1;
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
			PlayerAnimationType = 0;
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
