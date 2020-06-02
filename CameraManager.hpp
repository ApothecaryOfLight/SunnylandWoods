//CameraType.hpp

#ifndef _CAMERA_MANAGER_
#define _CAMERA_MANAGER_

class CameraManager {
public:
	CameraManager ( int inPlayerX, int inPlayerY, int inScreenWidth, int inScreenHeight );
	void doSetSectors ( int inSectorsX, int inSectorsY );
	void doResize ( int inNewWidth, int inNewHeight );
	void doSetPlayerSize ( int inPlayerSizeX, int inPlayerSizeY );

	int PlayerX_screen, PlayerY_screen, //Poisitional system of where the player is on the screen.
		PlayerX_level, PlayerY_level, //Positional system within the level of where the player is. Can be used to obtain CurrentSectorX/Y
		CameraX, CameraY, //Position of the camera's upper-left corner. Used to lens everything that is not the player.
		ScreenWidth, ScreenHeight, //Size of the screen
		SectorsX, SectorsY,  //Size of level, set by LevelManager on loading a level.
		CurrentSectorX, CurrentSectorY; //Which sector of the current level in which the player is. Can be derived from PlayerX_Level/PlayerY_Level.
	int magnification;
	int ScreenWall_Left, ScreenWall_Right; //The boundaries of how far the character can go left and right before they move the camera.
	int PlayerSize_X, PlayerSize_Y;
};

#endif
