//MapManager.hpp

#ifndef _MAP_MANAGER_
#define _MAP_MANAGER_

#include <SDL.h>
#include <SDL_image.h>

#include "../camera_manager/CameraManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../id_manager/id_manager.hpp"

#include <list>
#include <string>

/*
Stores all Map Objects
*/

class MapObject {
public:
	//GlobalID = from IDManager
	//AssetID = from AssetManager
	//LocalID = How MapManager stores it.
	MapObject ( void );
	MapObject( int GlobalID, int AssetID );
	int myGlobalID, myAssetID;
	int XPos, YPos;
	bool has_collided;
	int has_collided_counter;
};

class MapManager {
public:
	MapManager ( SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory, IDManager * inIDManager );
	~MapManager ( void );

	void doLoadMapTextures ( void );

	void doLoadMap ( std::string inFilename );

	//SDL_Rect doCreateRect ( int inX, int inY, int inW, int inH );
	void doCreateRect ( int inMapObjectID, int inX, int inY, int inW, int inH );

	void doLoadMapObjects ( void );

	void doRenderFrame ( void );

	int isColliding ( int inX, int inY, int inWidth, int inHeight );

	void mark_collided(int inMapObjectID);
	void decrement_collided(int inMapObjectID);

	int getAssetID ( int inGlobalID );
	MapObject * getMapObject ( int inGlobalID );

	SDL_Rect ** myCollisionBoxes;
	std::list<int> myActiveMapObjects;
private:
	//My class pointers
	CameraManager * myCameraManager;
	AssetFactory * myAssetFactory;
	SDL_Renderer * myRen;
	IDManager * myIDManager;

	//Rects
	SDL_Rect rect_Clouds;
	SDL_Rect rect_Clouds_dest;

	//Textures
	SDL_Texture * texture_Clouds;

	//My Map Objects
	MapObject * myMapObjects;
};

#endif
