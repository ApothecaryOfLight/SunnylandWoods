//CameraType.cpp

#include "CameraManager.hpp"

CameraManager::CameraManager ( int inPlayerX, int inPlayerY, int inScreenWidth, int inScreenHeight ) {
	PlayerX_level = inPlayerX;
	PlayerY_level = inPlayerY;

	doResize( inScreenWidth, inScreenHeight );

	CameraX = 0;
	CameraY = 0;

	SectorsX = 1;
	SectorsY = 1;

	magnification = 3;
}

void CameraManager::doSetSectors ( int inSectorsX, int inSectorsY ) {
	SectorsX = inSectorsX;
	SectorsY = inSectorsY;
}

void CameraManager::doResize ( int inNewWidth, int inNewHeight ) {
	ScreenWidth = inNewWidth;
	ScreenHeight = inNewHeight;

	ScreenWall_Left = ScreenWidth/5;
	ScreenWall_Right = ScreenWidth-ScreenWall_Left;

	PlayerSize_X = 90; //TODO: Apply resizing if appropriate.
	PlayerSize_Y = 58;

	PlayerX_screen = (ScreenWidth/2)-(PlayerSize_X/2);
	PlayerY_screen = (ScreenHeight*.5)-((PlayerSize_Y*magnification)/2);

}

void CameraManager::doSetPlayerSize ( int inPlayerSizeX, int inPlayerSizeY ) {
	PlayerSize_X = inPlayerSizeX;
	PlayerSize_Y = inPlayerSizeY;
}
