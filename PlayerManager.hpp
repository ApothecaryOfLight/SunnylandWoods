//PlayerManager.hpp

#ifndef _PLAYER_MANAGER_
#define _PLAYER_MANAGER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "InputManager.hpp"

#include <string>
#include <sstream>

#include "CameraManager.hpp"
#include "AssetFactory.hpp"

class PlayerManager {
public:
	PlayerManager ( SDL_Renderer * inRen, InputManager * inInputManager, CameraManager * inCameraManager, AssetFactory * inAssetFactory );
	~PlayerManager ( void );

	void doLoadPlayerAnimationCycle ( SDL_Texture ** DestTextureArray, std::string inFileLocation, int inLength );
	void doRenderFrame ( void );
	void doGameLogic ( void );

private:
	SDL_Renderer * myRen;
	InputManager * myInputManager;

	int anim_frame_Player;
	int anim_frame_Player_MAX;
	int FPSCounter;

	int PlayerAnimationType;

	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
};

#endif
