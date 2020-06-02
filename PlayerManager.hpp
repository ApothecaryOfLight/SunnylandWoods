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
	SDL_Rect doCreateRect ( int inX, int inY, int inW, int inH );
	void doLoadPlayer ( void );
	void doRenderFrame ( void );
	void doGameLogic ( void );

	SDL_Rect rect_PlayerSrc, rect_PlayerDest; //TODO: Reintegrate all player positions to myCamera, not myAssetFactory. AssetFactory is not an object manager, it is a resource loader.
private:
	SDL_Renderer * myRen;
	InputManager * myInputManager;

	//SDL_Texture * texture_PlayerIdleFrames[8];
	SDL_Texture * texture_PlayerRunFrames[6];
	SDL_Texture * texture_PlayerJumpFrames[4];
	SDL_Texture * texture_PlayerCrouchFrames[2];
	//SDL_Texture * texture_PlayerHurtFrames[2];


	int anim_frame_Player;
	int anim_frame_Player_MAX;
	int FPSCounter;

	int PlayerAnimationType;

	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
};

#endif
