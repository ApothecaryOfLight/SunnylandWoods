//CameraType.cpp

#include "CameraManager.hpp"

CameraManager::CameraManager ( int inPlayerX, int inPlayerY, int inScreenWidth, int inScreenHeight ) {
	PlayerX_level = inPlayerX;
	PlayerY_level = inPlayerY;

	ScreenWidth = inScreenWidth;
	ScreenHeight = inScreenHeight;

	PlayerX_screen = ((ScreenWidth/3)*2)-((90*3)/2);
	PlayerY_screen = ScreenHeight*(2/3);

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
}
