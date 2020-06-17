//GameInterface.hpp

#ifndef _GAME_INTERFACE_
#define _GAME_INTERFACE_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

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
