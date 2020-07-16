//Remember to set the SectorsX and SectorsY in EnemyManager, MapManager and ClickableManager
#include "LevelManager.hpp"

#include "../map_manager/MapManager.hpp"
#include "../clickable_manager/ClickableManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"


LevelManager::LevelManager ( MapManager * inMapManager, EnemyManager * inEnemyManager, ClickableManager * inClickableManager, IDManager * inIDManager ) {
	myMapManager = inMapManager;
	myEnemyManager = inEnemyManager;
	myClickableManager = inClickableManager;
	myIDManager = inIDManager;
}

void LevelManager::doLoadLevel( std::string inLevelName ) {
//0) Resize everything as appropriate

//1) Load MapObjects
	myMapManager->doLoadMapObjects();
//2) Load Enemies

//3) Load Interactables

//4) Load Clickables

//5) Load player position and set sectors

}
