//Remember to set the SectorsX and SectorsY in EnemyManager, MapManager and ClickableManager
#include "LevelManager.hpp"

#include "../logger/logger.hpp"
#include "../map_manager/MapManager.hpp"
#include "../clickable_manager/ClickableManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"

#include <vector>


LevelManager::LevelManager ( Logger* inLogger, MapManager * inMapManager, EnemyManager * inEnemyManager, ClickableManager * inClickableManager, IDManager * inIDManager ) {
	myLogger = inLogger;
	myMapManager = inMapManager;
	myEnemyManager = inEnemyManager;
	myClickableManager = inClickableManager;
	myIDManager = inIDManager;

	doLoadLevel("level_0.txt");
}

void LevelManager::ParseMapObject(std::string inMapObjectText) {
	std::size_t last_location = 0;
	int counter = 0;
	std::vector<int> ret;
	while (last_location != std::string::npos) {
		std::size_t next_location = inMapObjectText.find_first_of(" ", last_location + 1);
		ret.push_back(atoi(inMapObjectText.substr(last_location, next_location).c_str()));
		last_location = next_location;
	}
	myMapManager->doCreateMapObject(ret[1], ret[2], ret[0]);
}

void LevelManager::ParseEnemyObject(std::string inEnemyObjectText) {
	std::size_t last_location = 0;
	int counter = 0;
	std::vector<int> ret;
	while (last_location != std::string::npos) {
		std::size_t next_location = inEnemyObjectText.find_first_of(" ", last_location + 1);
		ret.push_back(atoi(inEnemyObjectText.substr(last_location, next_location).c_str()));
		last_location = next_location;
	}
	myEnemyManager->doAddEnemy(ret[0], ret[1], ret[2]);
}

void LevelManager::loadMapObjects(std::fstream* inFileStream) {
	char inLine[256];
	inFileStream->getline(inLine, 255);
	while (strcmp(inLine, "END_MAP_OBJECTS") != 0) {
		ParseMapObject(inLine);
		inFileStream->getline(inLine, 255);
	}
}


void LevelManager::loadEnemyObjects(std::fstream* inFileStream) {
	char inLine[256];
	inFileStream->getline(inLine, 255);
	while (strcmp(inLine, "END_ENEMY_OBJECTS") != 0) {
		ParseEnemyObject(inLine);
		inFileStream->getline(inLine, 255);
	}
}

void LevelManager::doLoadLevel( std::string inLevelName ) {
	std::fstream myFileStream;
	myFileStream.open(inLevelName, std::fstream::in);
	char inLine[256];

	while ((myFileStream.rdstate() & std::ifstream::failbit) == 0) {
		myFileStream.getline(inLine, 255);
		if (strcmp(inLine, "BEGIN_MAP_OBJECTS") == 0) {
			loadMapObjects(&myFileStream);
		}
		else if (strcmp(inLine, "BEGIN_ENEMY_OBJECTS") == 0) {
			loadEnemyObjects(&myFileStream);
		}
	}
	myFileStream.close();
}
