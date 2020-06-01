//CameraType.hpp

#ifndef _CAMERA_MANAGER_
#define _CAMERA_MANAGER_

class CameraManager {
public:
	CameraManager ( int inPlayerX, int inPlayerY, int inScreenWidth, int inScreenHeight );
	void doSetSectors ( int inSectorsX, int inSectorsY );
	void doResize ( int inNewWidth, int inNewHeight );
	int PlayerX_screen, PlayerY_screen, //Positional system within the level of where the player is. Can be used to obtain CurrentSectorX/Y
		PlayerX_level, PlayerY_level,
		CameraX, CameraY, //Position of the camera's upper-left corner
		ScreenWidth, ScreenHeight, //Currently set at initialization, will be readjustable.
		SectorsX, SectorsY,  //Size of level, set by LevelManager on loading a level.
		CurrentSectorX, CurrentSectorY; //Which sector of the current level in which the player is.
	int magnification;
};

#endif
