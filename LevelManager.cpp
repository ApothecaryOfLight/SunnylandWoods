

//Remember to set the SectorsX and SectorsY in EnemyManager, MapManager and ClickableManager

#include "LevelManager.hpp"

#include "MapManager.hpp"
#include "ClickableManager.hpp"
#include "EnemyManager.hpp"


LevelManager::LevelManager ( MapManager * inMapManager, EnemyManager * inEnemyManager, ClickableManager * inClickableManager ) {
	myMapManager = inMapManager;
	myEnemyManager = inEnemyManager;
	myClickableManager = inClickableManager;
}

void LevelManager::doLoadLevel( std::string inLevelName ) {

}
