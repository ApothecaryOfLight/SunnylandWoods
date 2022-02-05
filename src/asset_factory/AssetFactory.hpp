#ifndef _ASSET_FACTORY_
#define _ASSET_FACTORY_

#include <SDL.h>
#include <SDL_image.h>

#include <string>

#include "../logger/logger.hpp"
#include "../camera_manager/CameraManager.hpp"

/*
Should only include the sprite image, stored with asset ID
As well as the source dimensions of the image
*/

class StaticAsset {
public:
	SDL_Texture * myTexture;
	SDL_Rect myRect_src, myRect_dst;
	//SDL_Rect myRect_draw, myRect_collision;
	//int offsetX, offsetY;
	int PixelWidth, PixelHeight;
};

class AnimatedAsset {
public:
	StaticAsset ** myStaticAssets;
	int frames; 
};

class AssetFactory {
public:
	AssetFactory ( Logger * inLogger, SDL_Renderer * inRen, CameraManager * inCameraManager );

	SDL_Rect doCreateRect ( int inX, int inY, int inW, int inH );

	void doLoadImages ( void );
	void doLoadImage ( int inAssetID, bool isAnimated, std::string inSrcFilename, int inSrcX, int inSrcY, int inWidth, int inHeight, int inFrames );

	void doLoadPlayerImages ( void );
	void doLoadEnemyImages(void);

	void doResize ( int screenWidth, int screenHeight );

	StaticAsset * myStaticAssets[100];//TODO: Unify these two. IDs will be the distinciton between ranges and singles.
	AnimatedAsset * myAnimatedAssets[100];

private:
	SDL_Renderer * myRen;
	CameraManager * myCameraManager;
	Logger * myLogger;
};

#endif
