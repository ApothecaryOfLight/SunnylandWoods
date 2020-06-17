#ifndef _ASSET_FACTORY_
#define _ASSET_FACTORY_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>

#include "../camera_manager/CameraManager.hpp"

class StaticAsset {
public:
	SDL_Texture * myTexture;
	SDL_Rect myRect_src, myRect_dst;//TODO: This should only be used for drawing, not collisions. Move that to the object managers.
	int PixelWidth, PixelHeight;
};

class AnimatedAsset {
public:
	StaticAsset ** myStaticAssets;
	int frames; 
};

class AssetFactory {
public:
	AssetFactory ( SDL_Renderer * inRen, CameraManager * inCameraManager );

	SDL_Rect doCreateRect ( int inX, int inY, int inW, int inH );

	void doLoadImages ( void );
	void doLoadImage ( int inAssetID, bool isAnimated, std::string inSrcFilename, int inSrcX, int inSrcY, int inWidth, int inHeight, int inFrames );
	void doLoadPlayerImages ( void );
	void doResize ( void );

	void doAdjustPlayerDest ( int inDestX );

	StaticAsset * myStaticAssets[100];
	AnimatedAsset * myAnimatedAssets[100];

private:
	SDL_Renderer * myRen;
	CameraManager * myCameraManager;
};

#endif
