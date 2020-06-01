#ifndef _CLICKABLE_MANAGER_
#define _CLICKABLE_MANAGER_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "MapManager.hpp"
#include "CameraManager.hpp"

class ClickableManager {
public:
	ClickableManager ( SDL_Renderer * inRen, MapManager * inMapManager, CameraManager * inCameraManager );

	void doClearMyClickables ( void );
	void doAddClickable ( int inAssetID, int inPosX, int inPosY, int FunctionID );

	void doProcessClick ( int ClickX, int ClickY );
private:
	SDL_Renderer * myRen;
	MapManager * myMapManager;

	CameraManager * myCameraManager;

	//Make additional nesting array in order to represent sectors.
	int *** myClickables; 	//Asset ID		//X Pos			//Y Pos		//Width		//height	//Facing direction		//Function ID
};

#endif
