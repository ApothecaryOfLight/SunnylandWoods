//MapManager.cpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "MapManager.hpp"

#include "CameraManager.hpp"

#include <iostream>

MapManager::MapManager ( SDL_Renderer * inRen, CameraManager * inCameraManager ) {
	myRen = inRen;

	SDL_Rect toRetByValue;
	rect_Clouds.x = 0;
	rect_Clouds.y = 0;
	rect_Clouds.w = 160;
	rect_Clouds.h = 208;

	rect_Clouds_dest.x = 0;
	rect_Clouds_dest.y = 0;
	rect_Clouds_dest.w = inCameraManager->ScreenWidth;
	rect_Clouds_dest.h = inCameraManager->ScreenHeight;

	doLoadMapTextures();

	myObjects = new int*[1000];
	for( int i=0; i<999; i++ ) {
		myObjects[i] = new int[6];
	}

	myObjects[0][0] = 1;
	myObjects[0][1] = 1;
	myObjects[0][2] = 1;
	myObjects[0][3] = 1;
	myObjects[0][4] = 1;
	myObjects[0][5] = 1;

}

MapManager::~MapManager ( void ) {
	SDL_DestroyTexture( texture_Clouds );
}

void MapManager::doLoadMapTextures ( void ) {
	SDL_Surface *myNewSurface = IMG_Load( "media/ENVIRONMENT/bg-clouds.png" );
	if(!myNewSurface) { std::cout << IMG_GetError() << std::endl; }
	texture_Clouds = SDL_CreateTextureFromSurface( myRen, myNewSurface );
	SDL_FreeSurface( myNewSurface );
}

void MapManager::doRenderFrame ( void ) {
	SDL_RenderCopy( myRen, texture_Clouds, &rect_Clouds, &rect_Clouds_dest );
}
