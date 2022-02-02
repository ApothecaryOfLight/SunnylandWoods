//CameraType.cpp

#include "CameraManager.hpp"
#include "../logger/logger.hpp"

#include <iostream>


CameraManager::CameraManager ( Logger * inLogger, int inPlayerX, int inPlayerY, int inScreenWidth, int inScreenHeight ) {
	myLogger = inLogger;

	PlayerX_level = inPlayerX;
	PlayerY_level = inPlayerY;

	PlayerX_screen = (ScreenWidth / 2) - (PlayerSize_X / 2);
	PlayerY_screen = (ScreenHeight / 2) - (PlayerSize_Y / 2);

	magnification = 1;
	doResize( inScreenWidth, inScreenHeight );

	CameraX = 0;
	CameraY = 0;

	SectorsX = 1;
	SectorsY = 1;

}

void CameraManager::doInitializeCamera( int PlayerGameCoordX, int PlayerGameCoordY ) {
	CameraX = PlayerGameCoordX - ((ScreenWidth - PlayerSize_X) / 2);
	CameraY = PlayerGameCoordY - ((ScreenHeight - PlayerSize_Y) / 2);
}

void CameraManager::doSetSectors ( int inSectorsX, int inSectorsY ) {
	SectorsX = inSectorsX;
	SectorsY = inSectorsY;
}

void CameraManager::doResize ( int inNewWidth, int inNewHeight ) {
	ScreenWidth = inNewWidth;
	ScreenHeight = inNewHeight;

	ScreenWall_Left = ScreenWidth/5;
	ScreenWall_Right = ScreenWidth - ScreenWall_Left;
	ScreenWall_Top = ScreenHeight / 5;
	ScreenWall_Bottom = ScreenHeight/2;

	PlayerSize_X = 90; //TODO: Apply resizing if appropriate.
	PlayerSize_Y = 58;

	PlayerX_screen = (ScreenWidth/2)-(PlayerSize_X/2);
	PlayerY_screen = (ScreenHeight/2)-(PlayerSize_Y/2);
}

void CameraManager::doSetPlayerSize ( int inPlayerSizeX, int inPlayerSizeY ) {
	PlayerSize_X = inPlayerSizeX;
	PlayerSize_Y = inPlayerSizeY;
}

int CameraManager::getPlayerAdjX() {
	return (ScreenWidth / 2) - (PlayerSize_X / 2);
}

int CameraManager::getPlayerAdjY() {
	return (ScreenHeight / 2) - (PlayerSize_Y / 2);
}