//CameraType.hpp

#ifndef _CAMERA_MANAGER_
#define _CAMERA_MANAGER_

#include "../logger/logger.hpp"

class CameraManager {
public:
	CameraManager ( Logger * inLogger, int inPlayerX, int inPlayerY, int inScreenWidth, int inScreenHeight );
	void doResize ( int inNewWidth, int inNewHeight );
	void doSetPlayerSize ( int inPlayerSizeX, int inPlayerSizeY );

	void doInitializeCamera(int PlayerGameCoordX, int PlayerGameCoordY);

	int CameraX, CameraY; //Position of the camera's upper-left corner. Used to lens everything that is not the player.
	int ScreenWidth, ScreenHeight; //Size of the screen
	int ScreenWall_Left, ScreenWall_Right; //The boundaries of how far the character can go left and right before they move the camera.
	int ScreenWall_Top, ScreenWall_Bottom;
	int PlayerSize_X, PlayerSize_Y;
	int zoom;
	Logger* myLogger;
};

#endif
