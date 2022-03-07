#include "AssetFactory.hpp"
#include "../logger/logger.hpp"

#include <iostream>
#include <string>
#include <sstream>

#include "../camera_manager/CameraManager.hpp"

AssetFactory::AssetFactory ( Logger * inLogger, SDL_Renderer * inRen, CameraManager * inCameraManager ) {
	myLogger = inLogger;
	myRen = inRen;
	myCameraManager = inCameraManager;

	static_asset_count = 0;
	animated_asset_count = 0;

	doLoadImages();
	doResizeImages(inCameraManager->zoom);
}

SDL_Rect AssetFactory::doCreateRect ( int inX, int inY, int inW, int inH ) {
	SDL_Rect toRetByValue;
	toRetByValue.x = inX;
	toRetByValue.y = inY;
	toRetByValue.w = inW;
	toRetByValue.h = inH;
	return toRetByValue;
}

void AssetFactory::doResizeImages(int inMagnification) {
	for (int set = 0; set < animated_asset_count; set++) {
		AnimatedAsset* myAnimatedAssetPtr = myAnimatedAssets[set];
		for (int frame = 0; frame<myAnimatedAssetPtr->frames; frame++) {
			StaticAsset* myStaticAssetPtr = myAnimatedAssets[set]->myStaticAssets[frame];
			myStaticAssetPtr->myRect_dst.w = static_cast<int>(myStaticAssetPtr->myRect_src.w * inMagnification);
			myStaticAssetPtr->myRect_dst.h = static_cast<int>(myStaticAssetPtr->myRect_src.h * inMagnification);
		}
	}

	SDL_Rect myPlayer = myAnimatedAssets[0]->myStaticAssets[0]->myRect_dst;
	myCameraManager->doSetPlayerSize(myPlayer.w, myPlayer.h);

	for (int static_asset_id = 0; static_asset_id < static_asset_count; static_asset_id++) {
		StaticAsset* myStaticAssetPtr = myStaticAssets[static_asset_id];
		myStaticAssetPtr->myRect_dst.w = static_cast<int>(myStaticAssetPtr->myRect_src.w * inMagnification);
		myStaticAssetPtr->myRect_dst.h = static_cast<int>(myStaticAssetPtr->myRect_src.h * inMagnification);
	}
}

void AssetFactory::doLoadImage(int inAssetID, bool isAnimated, std::string inSrcFilename, int inSrcX, int inSrcY, int inWidth, int inHeight, int inFrames) {
	myLogger->log("doLoadImage");
	myLogger->log(inAssetID);
	if (isAnimated) {
		myAnimatedAssets[inAssetID] = new AnimatedAsset;
		myAnimatedAssets[inAssetID]->frames = inFrames;
		myAnimatedAssets[inAssetID]->myStaticAssets = new StaticAsset * [inFrames];
		for (int i = 1; i <= inFrames; i++) {
			std::stringstream myStringstreamConv;
			myStringstreamConv << i;
			std::string myConv = myStringstreamConv.str();

			SDL_Surface* myNewSurface = IMG_Load(std::string(inSrcFilename + myConv + ".png").c_str());
			myLogger->log(std::string(inSrcFilename + myConv + ".png"));

			if (!myNewSurface) { myLogger->log(IMG_GetError()); }

			myAnimatedAssets[inAssetID]->myStaticAssets[i - 1] = new StaticAsset;
			myAnimatedAssets[inAssetID]->myStaticAssets[i - 1]->myTexture = SDL_CreateTextureFromSurface(myRen, myNewSurface);
			myAnimatedAssets[inAssetID]->myStaticAssets[i - 1]->PixelWidth = inWidth;
			myAnimatedAssets[inAssetID]->myStaticAssets[i - 1]->PixelHeight = inHeight;
			myAnimatedAssets[inAssetID]->myStaticAssets[i - 1]->myRect_src = doCreateRect(inSrcX, inSrcY, inWidth, inHeight);
			myAnimatedAssets[inAssetID]->myStaticAssets[i - 1]->myRect_dst = doCreateRect(
				0,
				0,
				inWidth * myCameraManager->magnification,
				inHeight * myCameraManager->magnification
			);

			SDL_FreeSurface(myNewSurface);
		}
		animated_asset_count++;
	}
	else if (!isAnimated) {
		std::cout << "Loading image 5." << std::endl;
		std::cout << inAssetID << std::endl;
		myStaticAssets[inAssetID] = new StaticAsset;

		SDL_Surface* myNewSurface = IMG_Load(inSrcFilename.c_str());
		if (!myNewSurface) { std::cout << IMG_GetError() << std::endl; }

		myStaticAssets[inAssetID]->myTexture = SDL_CreateTextureFromSurface(myRen, myNewSurface);
		myStaticAssets[inAssetID]->PixelWidth = inWidth;
		myStaticAssets[inAssetID]->PixelHeight = inHeight;
		myStaticAssets[inAssetID]->myRect_src = doCreateRect(inSrcX, inSrcY, inWidth, inHeight);
		myStaticAssets[inAssetID]->myRect_dst = doCreateRect(0, 0, inWidth, inHeight);

		SDL_FreeSurface(myNewSurface);
		static_asset_count++;
	}
}

void AssetFactory::doLoadImages(void) {
	doLoadMapImages();
	doLoadPlayerImages();
	doLoadEnemyImages();
}

void AssetFactory::doLoadMapImages(void) {
	doLoadImage(0, false, "media/ENVIRONMENT/tileset.png", 384, 96, 16, 16, 0);
	doLoadImage(1, false, "media/ENVIRONMENT/tileset.png", 128, 224, 32, 32, 0);
}

void AssetFactory::doLoadPlayerImages ( void ) {
	doLoadImage( 0, true, "media/SPRITES/player/idle/player-idle-", 20, 0, 36, 48, 8 );
	doLoadImage( 1, true, "media/SPRITES/player/run/player-run-", 9, 0, 54, 48, 6 );
	doLoadImage( 2, true, "media/SPRITES/player/hurt/player-hurt-", 0, 0, 90, 58, 2 );
	doLoadImage( 3, true, "media/SPRITES/player/crouch/player-crouch-", 0, 0, 90, 58, 2 );
	doLoadImage( 4, true, "media/SPRITES/player/jump/player-jump-", 21, 0, 45, 58, 4 );

	SDL_Rect myPlayer = myAnimatedAssets[0]->myStaticAssets[0]->myRect_dst;
	myCameraManager->doSetPlayerSize(myPlayer.w, myPlayer.h);
	myLogger->log("Player images loaded.");
}

void AssetFactory::doLoadEnemyImages(void) {
	doLoadImage(5, true, "media/SPRITES/enemies/ant/ant-", 0, 0, 37, 31, 8);
	doLoadImage(6, true, "media/SPRITES/misc/enemy-death/enemy-death-", 0, 0, 28, 26, 4);
	doLoadImage(7, true, "media/SPRITES/enemies/gator/gator-", 0, 0, 46, 49, 4);
}