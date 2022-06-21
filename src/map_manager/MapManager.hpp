//MapManager.hpp

#ifndef _MAP_MANAGER_
#define _MAP_MANAGER_

#include <SDL.h>
#include <SDL_image.h>

#include "../camera_manager/CameraManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../id_manager/id_manager.hpp"
#include "../logger/logger.hpp"

#include <list>
#include <string>

/*
Stores all Map Objects
*/

class MapObject {
public:
	MapObject ( void );
	MapObject( int GlobalID, int AssetID );
	int myGlobalID, myAssetID;
	int XPos, YPos;
	bool has_collided;
	int has_collided_counter;
};

class MapManager {
public:
	MapManager ( SDL_Renderer * inRen, Logger *inLogger, CameraManager * inCameraManager, AssetFactory * inAssetFactory, IDManager * inIDManager );
	~MapManager ( void );

	void doLoadMapTextures ( void );

	void doCreateMapObject(int XPos, int YPos, int AssetID);

	void doResize( int screenWidth, int screenHeight );

	void doRenderFrame ( void );

	void mark_collided(int inMapObjectID);
	void decrement_collided(int inMapObjectID);

	int getAssetID ( int inGlobalID );
	MapObject * getMapObject ( int inGlobalID );

	std::list<int> myActiveMapObjects;
private:
	//My class pointers
	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
	SDL_Renderer * myRen;
	IDManager * myIDManager;
	Logger* myLogger;

	//Rects
	SDL_Rect rect_Clouds;
	SDL_Rect rect_Clouds_dest;

	//Textures
	SDL_Texture * texture_Clouds;

	//My Map Objects
	MapObject * myMapObjects;
};

#endif
