#ifndef _LEVEL_MANAGER_
#define _LEVEL_MANAGER_

#include <fstream>

/*
All levels are stored as sectors of 100x100 units.
All entities are stored in this style grid with the same coordinants
You only need to check the current sector and adjacent sectors

Enemies can move between sectors.
Collectibles, Objects, and Interactables cannot.

Everything has a unique integer ID: objects, collectibles, interactables, etc.
Prototype ID is for each species of objects/collectible/etc.
ID is a unique value for each instantiation of an object/collectible etc.

Prototype ID is specified in XML files.
ID is issued on the fly when loading a level.
*/

/*
ASSET LIST:
	Links names in XML file to PNGs
*/

/*
PER PIXEL BOUNDING BOXES
	Generates bounding boxes for each frame on startup.
*/

#include "MapManager.hpp"
#include "ClickableManager.hpp"
#include "EnemyManager.hpp"

class LevelManager {
public:
	LevelManager ( MapManager * inMapManager, EnemyManager * inEnemyManager, ClickableManager * inClickableManager );
	void doLoadLevel( std::string inLevelName );
private:
	MapManager * myMapManager;
	EnemyManager * myEnemyManager;
	ClickableManager * myClickableManager;
	int ID_counter;
};

#endif
