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

	void doAnimatePlayer(void);
	void doRenderFrame ( void );

	SDL_Rect getPlayerDest(void);

	int anim_frame_Player;
	int jump_counter, jump_peak_counter;
	int PlayerAnimationType;
	int anim_frame_Player_MAX;
	int FPSCounter;

	bool isHitByEnemy;

	int PlayerGameCoordX, PlayerGameCoordY;

	int player_movement_increment;
private:
	Logger * myLogger;
	SDL_Renderer * myRen;
	InputManager * myInputManager;


	IDManager * myIDManager;
	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
};

#endif
