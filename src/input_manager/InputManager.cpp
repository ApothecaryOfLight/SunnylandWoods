//InputManager.cpp
#include "InputManager.hpp"
#include "../asset_factory/AssetFactory.hpp"

#include <SDL.h>

#include <string>
#include <iostream>

InputManager::InputManager ( Logger* inLogger, CameraManager* inCameraManager, AssetFactory* inAssetFactory ) {
	myLogger = inLogger;
	myAssetFactory = inAssetFactory;
	isQuit = false;

	isChangeAnimationAlert = false;

	isPressed_LeftArrowKey = false;
	isPressed_RightArrowKey = false;
	isPressed_DownArrowKey = false;
	isPressed_SpaceBar = false;
	inputFlag_Jumping = false;
	inputFlag_Crouching = false;
	inputFlag_Left = false;
	inputFlag_Right = false;

	isPlayerFacingLeft = true;

	isResized = false;
	newWidth = 1024;
	newHeight = 640;

	isPressed_F5 = false;

	myCameraManager = inCameraManager;

	mouseX_pos = 0;
	mouseY_pos = 0;
	mouseX_gamepos = 0;
	mouseY_gamepos = 0;

	mouse_wheel = 0;
}

void InputManager::doProcessInput ( SDL_Event * inEvent ) {
	SDL_GetMouseState(&mouseX_pos, &mouseY_pos);
	coord myMouseGameCoords = myCameraManager->translate_screen_coords(mouseX_pos, mouseY_pos);
	mouseX_gamepos = myMouseGameCoords.x;
	mouseY_gamepos = myMouseGameCoords.y;

	while (SDL_PollEvent(inEvent)){
		if( inEvent->type == SDL_QUIT ){
			isQuit = true; 
		}
		if( inEvent->type == SDL_KEYDOWN ){
			switch( inEvent->key.keysym.sym ) {
				case SDLK_ESCAPE: isQuit = true; break;
				case SDLK_LEFT:
					if( isPressed_LeftArrowKey == false ) {
						isPressed_LeftArrowKey = true;
						if( isPressed_RightArrowKey == false ) { isPlayerFacingLeft = true; }
						if (inputFlag_Right == false) {
							inputFlag_Left = true;
						}
						else { isPressed_LeftArrowKey = false; }
					}
				break;
				case SDLK_RIGHT:
					if( isPressed_RightArrowKey == false ) {
						isPressed_RightArrowKey = true;
						if( isPressed_LeftArrowKey == false ) {
							isPlayerFacingLeft = false;
						}
						if (inputFlag_Left == false) {
							inputFlag_Right = true;
						} else {
							isPressed_RightArrowKey = false;
						}
					}
				break;
				case SDLK_DOWN:
					if( isPressed_DownArrowKey == false ) {
						isPressed_DownArrowKey = true;
						if( inputFlag_Jumping == false && inputFlag_Left == false && inputFlag_Right == false ) {
							inputFlag_Crouching = true;
						}
						else { isPressed_DownArrowKey = false; }
					}
				break;
				case SDLK_SPACE:
					if( isPressed_SpaceBar == false ) {
						isPressed_SpaceBar = true;
						inputFlag_Jumping = true;
					}
				break;
				case SDLK_F5:
					if (isPressed_F5 == false) {
						isPressed_F5 = true;
					}
					else {
						isPressed_F5 = false;
					}
				break;
			};
		}
		if( inEvent->type == SDL_KEYUP ) {
			switch( inEvent->key.keysym.sym ) {
				case SDLK_LEFT:
					isPressed_LeftArrowKey = false;
					inputFlag_Left = false;
				break;
				case SDLK_RIGHT:
					isPressed_RightArrowKey = false;
					inputFlag_Right = false;
				break;
				case SDLK_DOWN:
					isPressed_DownArrowKey = false;
					inputFlag_Crouching = false;
				break;
				case SDLK_SPACE:
					isPressed_SpaceBar = false;
					inputFlag_Jumping = false;
				break;
			};
		}
		if( inEvent->type == SDL_WINDOWEVENT ) {
			switch( inEvent->window.event ) {
				case SDL_WINDOWEVENT_RESIZED:
					doResize(inEvent->window.data1, newHeight = inEvent->window.data2);
					break;
			}
		}
		if (inEvent->type == SDL_MOUSEWHEEL) {
			if (inEvent->wheel.y > 0) {
				mouse_wheel = inEvent->wheel.y;
				if (myCameraManager->zoom < 10) {
					myCameraManager->zoom += 1;
				}
				if (myCameraManager->zoom > 10) {
					myCameraManager->zoom = 10;
				}
				myAssetFactory->doResizeImages(myCameraManager->zoom);
				isResized = true;
			}
			else if (inEvent->wheel.y < 0) {
				mouse_wheel = inEvent->wheel.y;
				if (myCameraManager->zoom > 1) {
					myCameraManager->zoom -= 1;
				}
				if (myCameraManager->zoom < 1) {
					myCameraManager->zoom = 1;
				}
				myAssetFactory->doResizeImages(myCameraManager->zoom);
				isResized = true;
			}
		}
	}
}

void InputManager::doResize(int WindowSizeX, int WindowSizeY) {
	newWidth = WindowSizeX;
	newHeight = WindowSizeY;
	isResized = true;
}