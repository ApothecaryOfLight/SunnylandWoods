//InputManager.hpp

#ifndef _INPUT_MANAGER_
#define _INPUT_MANAGER_

#include <SDL.h>
#include "../logger/logger.hpp"
#include "../camera_manager/CameraManager.hpp"
#include "../asset_factory/AssetFactory.hpp"

class InputManager {
public:
	InputManager ( Logger * inLogger, CameraManager * inCameraManager, AssetFactory* inAssetFactory );
	void doProcessInput ( SDL_Event * inEvent );
	bool isQuit;
	bool isPressed_LeftArrowKey, isPressed_RightArrowKey, isPressed_SpaceBar, isPressed_DownArrowKey;
	bool isChangeAnimationAlert;

	bool inputFlag_Jumping, inputFlag_Crouching, inputFlag_Left, inputFlag_Right;

	bool isPlayerFacingLeft;

	bool isResized;
	int newWidth, newHeight;

	bool isPressed_F5;

	int mouseX_pos, mouseY_pos;
	int mouseX_gamepos, mouseY_gamepos;

	int mouse_wheel;

	//float zoom;
private:
	Logger* myLogger;
	CameraManager* myCameraManager;
	AssetFactory* myAssetFactory;
};

#endif


