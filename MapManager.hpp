//MapManager.hpp

#ifndef _MAP_MANAGER_
#define _MAP_MANAGER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "CameraManager.hpp"

#include <string>

/*
Stores all Objects, Collectibles and Interactables
*/

class MapManager {
public:
	MapManager ( SDL_Renderer * inRen, CameraManager * inCameraManager );
	~MapManager ( void );

	void doLoadMapTextures ( void );

	void doLoadMap ( std::string inFilename );

	void doLoadObjects ( void );

	void doRenderFrame ( void );

	int isColliding ( int inX, int inY, int inWidth, int inHeight );

private:
	SDL_Renderer * myRen;

	//Rects
	SDL_Rect rect_Clouds;
	SDL_Rect rect_Clouds_dest;

	//Textures
	SDL_Texture * texture_Clouds;

	int ** myObjects;
};

#endif
