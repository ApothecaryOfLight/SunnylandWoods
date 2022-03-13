#ifndef _ASSET_FACTORY_
#define _ASSET_FACTORY_

#include <SDL.h>
#include <SDL_image.h>

#include <string>

#include "../logger/logger.hpp"
#include "../camera_manager/CameraManager.hpp"

class CollisionBox {
	CollisionBox(int x, int y, int h, int w);
	int x, y, h, w;
};

class StaticAsset {
public:
	SDL_Texture* myTexture;
	SDL_Rect myRect_src, myRect_dst;
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

	void doLoadImage ( int inAssetID, std::string inAssetName, std::string inSrcFilename, int inSrcX, int inSrcY, int inWidth, int inHeight, int inFrames );
	void doLoadImage(int inAssetID, std::string inAssetName, std::string inSrcFilename, int inSrcX, int inSrcY, int inWidth, int inHeight);

	void doLoadImages(void);
	void doLoadMapImages ( void );
	void doLoadPlayerImages ( void );
	void doLoadEnemyImages(void);

	void doResizeImages(int inMagnification);

	StaticAsset * myStaticAssets[100];//TODO: Unify these two. IDs will be the distinciton between ranges and singles.
	AnimatedAsset * myAnimatedAssets[100];

private:
	SDL_Renderer * myRen;
	CameraManager * myCameraManager;
	Logger * myLogger;
	int static_asset_count, animated_asset_count;
};

#endif
