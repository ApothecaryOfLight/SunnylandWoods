//CameraType.cpp

#include "CameraManager.hpp"
#include "../logger/logger.hpp"

#include <iostream>


CameraManager::CameraManager ( Logger * inLogger, int inPlayerX, int inPlayerY, int inScreenWidth, int inScreenHeight ) {
	myLogger = inLogger;

	magnification = 1;
	doResize( inScreenWidth, inScreenHeight );

	CameraX = 0;
	CameraY = 0;

	SectorsX = 1;
	SectorsY = 1;

	zoom = 2;
}

void CameraManager::doInitializeCamera( int PlayerGameCoordX, int PlayerGameCoordY ) {
	CameraX = (PlayerGameCoordX*zoom) - ((ScreenWidth - PlayerSize_X) / 2);
	CameraY = (PlayerGameCoordY*zoom) - ((ScreenHeight - PlayerSize_Y) / 2);
}

void CameraManager::doSetSectors ( int inSectorsX, int inSectorsY ) {
	SectorsX = inSectorsX;
	SectorsY = inSectorsY;
}

void CameraManager::doResize ( int inNewWidth, int inNewHeight ) {
	myLogger->log("New screen height/width: " + std::to_string(inNewWidth) + "/" + std::to_string(inNewHeight));
	ScreenWidth = inNewWidth;
	ScreenHeight = inNewHeight;

	ScreenWall_Left = ScreenWidth/5;
	ScreenWall_Right = ScreenWidth - ScreenWall_Left;
	ScreenWall_Top = ScreenHeight / 5;
	ScreenWall_Bottom = ScreenHeight/2;
}

void CameraManager::doSetPlayerSize ( int inPlayerSizeX, int inPlayerSizeY ) {
	PlayerSize_X = inPlayerSizeX;
	PlayerSize_Y = inPlayerSizeY;
}