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

#include "../map_manager/MapManager.hpp"
#include "../clickable_manager/ClickableManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"

class LevelManager {
public:
	LevelManager ( Logger* inLogger, MapManager * inMapManager, EnemyManager * inEnemyManager, ClickableManager * inClickableManager, IDManager * inIDManager );
	void doLoadLevel( std::string inLevelName );
	void loadEnemyObjects(std::fstream* inFileStream);
	void loadMapObjects(std::fstream* inFileStream);
	void ParseEnemyObject(std::string inEnemyObjectText);
	void ParseMapObject(std::string inMapObjectText);
	void doSaveLevel(std::string inLevelName);
	void saveEnemyObjects(std::fstream* inFileStream);
	void saveMapObjects(std::fstream* inFileStream);
private:
	MapManager * myMapManager;
	EnemyManager * myEnemyManager;
	ClickableManager * myClickableManager;
	IDManager * myIDManager;
	Logger* myLogger;
	int ID_counter;
};

#endif
