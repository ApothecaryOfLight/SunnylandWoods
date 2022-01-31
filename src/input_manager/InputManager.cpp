//InputManager.cpp
#include "InputManager.hpp"

#include <SDL.h>

#include <string>
#include <iostream>

InputManager::InputManager ( CameraManager* inCameraManager ) {
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
	int newWidth, newHeight;

	isPressed_F5 = false;

	myCameraManager = inCameraManager;
}

void InputManager::doProcessInput ( SDL_Event * inEvent ) {
	SDL_GetMouseState(&mouseX_pos, &mouseY_pos);

	mouseX_gamepos = mouseX_pos + myCameraManager->CameraX;
	mouseY_gamepos = mouseY_pos + myCameraManager->CameraY;

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
						//if( inputFlag_Jumping == false && inputFlag_Right == false ) { //TODO: Make this configable
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
							//myCameraManager->PlayerX_level -= 18;
							//myCameraManager->PlayerX_screen -= 18;
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
					//myCameraManager->PlayerX_level -= 9;
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
					newWidth = inEvent->window.data1;
					newHeight = inEvent->window.data2;
					isResized = true;
					break;
			}
		}
	}
}
