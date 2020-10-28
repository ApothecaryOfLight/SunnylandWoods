//CameraType.cpp

#include "CameraManager.hpp"
#include "../logger/logger.hpp"

#include <iostream>


CameraManager::CameraManager ( Logger * inLogger, int inPlayerX, int inPlayerY, int inScreenWidth, int inScreenHeight ) {
	myLogger = inLogger;
	PlayerX_level = inPlayerX;
	PlayerY_level = inPlayerY;

	magnification = 1;
	doResize( inScreenWidth, inScreenHeight );

	CameraX = 0;
	CameraY = 0;

	//PlayerX_level = 0;
	//PlayerY_level = 0;

	SectorsX = 1;
	SectorsY = 1;

}

void CameraManager::doSetSectors ( int inSectorsX, int inSectorsY ) {
	SectorsX = inSectorsX;
	SectorsY = inSectorsY;
}

void CameraManager::doResize ( int inNewWidth, int inNewHeight ) {
	ScreenWidth = inNewWidth;
	ScreenHeight = inNewHeight;

std::cout << "ScreenSize: " << ScreenWidth << "/" << ScreenHeight << std::endl;

	ScreenWall_Left = ScreenWidth/5;
	ScreenWall_Right = ScreenWidth - ScreenWall_Left;

std::cout << "ScreenWalls: " << ScreenWall_Left << "/" << ScreenWall_Right << std::endl;

	PlayerSize_X = 90; //TODO: Apply resizing if appropriate.
	PlayerSize_Y = 58;
std::cout << "PlayerSize: " << PlayerSize_X << "/" << PlayerSize_Y << std::endl;

	PlayerX_screen = (ScreenWidth/2)-(PlayerSize_X/2);
	PlayerY_screen = (ScreenHeight*.5)-((PlayerSize_Y*magnification)/2);
	myLogger->log("YY:");
	myLogger->log(PlayerY_screen);
}

void CameraManager::doSetPlayerSize ( int inPlayerSizeX, int inPlayerSizeY ) {
	PlayerSize_X = inPlayerSizeX;
	PlayerSize_Y = inPlayerSizeY;
}
