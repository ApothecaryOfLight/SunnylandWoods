//GameInterface.hpp

#ifndef _GAME_INTERFACE_
#define _GAME_INTERFACE_

#include <SDL.h>
#include <SDL_image.h>

#include "../logger/logger.hpp"
#include "../animation_manager/AnimationManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"
#include "../input_manager/InputManager.hpp"
#include "../clickable_manager/ClickableManager.hpp"
#include "../collision_manager/CollisionManager.hpp"
#include "../level_manager/LevelManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../camera_manager/CameraManager.hpp"
#include "../map_manager/MapManager.hpp"
#include "../id_manager/id_manager.hpp"
#include "../text_manager/text_manager.hpp"

class GameInterface {
public:
	GameInterface ( SDL_Window * inWin, SDL_Renderer * inRen, Logger * inLogger );
	~GameInterface ( void );
	void doGameLogic ( void );
	void doRenderFrame ( void );
	void doProcessInput ( SDL_Event * inEvent );
	void setScreenTextFields(void);
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
	IDManager * myIDManager;
	Logger * myLogger;
	TextManager* myTextManager;
};

#endif
