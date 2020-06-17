//MapManager.hpp

#ifndef _MAP_MANAGER_
#define _MAP_MANAGER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "../camera_manager/CameraManager.hpp"
#include "../asset_factory/AssetFactory.hpp"

#include <string>

/*
Stores all Map Objects
*/

class MapManager {
public:
	MapManager ( SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory );
	~MapManager ( void );

	void doLoadMapTextures ( void );

	void doLoadMap ( std::string inFilename );

	//SDL_Rect doCreateRect ( int inX, int inY, int inW, int inH );
	void doCreateRect ( int inMapObjectID, int inX, int inY, int inW, int inH );

	void doLoadMapObjects ( void );

	void doRenderFrame ( void );

	int isColliding ( int inX, int inY, int inWidth, int inHeight );

	SDL_Rect ** myCollisionBoxes;
private:
	//My class pointers
	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
	SDL_Renderer * myRen;

	//Rects
	SDL_Rect rect_Clouds;
	SDL_Rect rect_Clouds_dest;

	//Textures
	SDL_Texture * texture_Clouds;

	//My Objects
	int ** myObjects;
	int myObjectCounter;

};

#endif
