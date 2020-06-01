//GameInterface.hpp

#ifndef _GAME_INTERFACE_
#define _GAME_INTERFACE_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "AnimationManager.hpp"
#include "PlayerManager.hpp"
#include "EnemyManager.hpp"
#include "InputManager.hpp"
#include "CollisionManager.hpp"
#include "LevelManager.hpp"
#include "MapManager.hpp"

#include "ClickableManager.hpp"

#include "AssetFactory.hpp"

#include "CameraManager.hpp"

class GameInterface {
public:
	GameInterface ( SDL_Renderer * inRen );
	~GameInterface ( void );
	void doGameLogic ( void );
	void doRenderFrame ( void );
	void doProcessInput ( SDL_Event * inEvent );
	bool isQuit ( void );
private:
	AnimationManager * myAnimationManager;
	MapManager * myMapManager;
	PlayerManager * myPlayerManager;
	EnemyManager * myEnemyManager;
	InputManager * myInputManager;
	AssetFactory * myAssetFactory;
	CameraManager * myCameraManager;
	ClickableManager * myClickableManager;
	CollisionManager * myCollisionManager;
	LevelManager * myLevelManager;
};

#endif
