//CameraType.cpp

#include "CameraManager.hpp"
#include "../logger/logger.hpp"

#include <iostream>

coord::coord(int inX, int inY) {
	x = inX;
	y = inY;
}

CameraManager::CameraManager ( Logger * inLogger, int inPlayerX, int inPlayerY, int inScreenWidth, int inScreenHeight ) {
	myLogger = inLogger;

	doResize( inScreenWidth, inScreenHeight );

	CameraX = 0;
	CameraY = 0;

	zoom = 1;
}

void CameraManager::doInitializeCamera( int PlayerGameCoordX, int PlayerGameCoordY ) {
	CameraX = (PlayerGameCoordX*zoom) - ((ScreenWidth - PlayerSize_X) / 2);
	CameraY = (PlayerGameCoordY*zoom) - ((ScreenHeight - PlayerSize_Y) / 2);
}

void CameraManager::doZoomCamera(int PlayerGameCoordX, int PlayerGameCoordY) {
	CameraX = (PlayerGameCoordX * zoom) - ((ScreenWidth - PlayerSize_X) / 2);
	CameraY = (PlayerGameCoordY * zoom) - ((ScreenHeight - PlayerSize_Y) / 2);
}

void CameraManager::doResize ( int inNewWidth, int inNewHeight ) {
	myLogger->log("New screen height/width: " + std::to_string(inNewWidth) + "/" + std::to_string(inNewHeight));
	ScreenWidth = inNewWidth;
	ScreenHeight = inNewHeight;

	ScreenWall_Left = ScreenWidth/5;
	ScreenWall_Right = ScreenWidth - ScreenWall_Left;
	ScreenWall_Top = ScreenHeight / 5;
	ScreenWall_Bottom = ScreenHeight - (ScreenWall_Top*2);
}

void CameraManager::doSetPlayerSize ( int inPlayerSizeX, int inPlayerSizeY ) {
	PlayerSize_X = inPlayerSizeX;
	PlayerSize_Y = inPlayerSizeY;
}

coord CameraManager::translate_coords(int GameCoordX, int GameCoordY) {
	int ScreenX = (GameCoordX * zoom) - CameraX;
	int ScreenY = (GameCoordY * zoom) - CameraY;
	return coord(ScreenX, ScreenY);
}

coord CameraManager::translate_screen_coords(int ScreenCoordX, int ScreenCoordY) {
	int GameCoordX = (ScreenCoordX + CameraX);
	int GameCoordY = (ScreenCoordY + CameraY);
	return coord(GameCoordX, GameCoordY);

}