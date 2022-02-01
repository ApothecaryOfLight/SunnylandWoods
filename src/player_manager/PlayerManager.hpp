//PlayerManager.hpp

#ifndef _PLAYER_MANAGER_
#define _PLAYER_MANAGER_

#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <sstream>

#include "../logger/logger.hpp"
#include "../input_manager/InputManager.hpp"
#include "../camera_manager/CameraManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../id_manager/id_manager.hpp"

class PlayerManager {
public:
	PlayerManager ( Logger * inLogger, SDL_Renderer * inRen, InputManager * inInputManager, CameraManager * inCameraManager, AssetFactory * inAssetFactory, IDManager * inIDManager );
	~PlayerManager ( void );

	void doLoadPlayerAnimationCycle ( SDL_Texture ** DestTextureArray, std::string inFileLocation, int inLength );
	void doAnimatePlayer(void);
	void doRenderFrame ( void );
	void doGameLogic ( void );

	SDL_Rect getPlayerDest(void);

	int anim_frame_Player;
	int jump_counter;
	int PlayerAnimationType;
	int anim_frame_Player_MAX;
	int FPSCounter;

	int PlayerGameCoordX, PlayerGameCoordY;
private:
	Logger * myLogger;
	SDL_Renderer * myRen;
	InputManager * myInputManager;


	IDManager * myIDManager;
	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
};

#endif
