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
	doSaveLevel("level_1.txt");
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

void LevelManager::saveMapObjects(std::fstream* inFileStream) {
	(*inFileStream) << "BEGIN_MAP_OBJECTS\n";
	std::list<int>::iterator myIter = myMapManager->myActiveMapObjects.begin();
	std::list<int>::iterator myEnd = myMapManager->myActiveMapObjects.end();
	while (myIter != myEnd) {
		int myID = (*myIter);
		MapObject* myMapObjectPtr = &myMapManager->myMapObjects[myID];
		(*inFileStream) << myMapObjectPtr->myAssetID << " ";
		(*inFileStream) << myMapObjectPtr->XPos << " ";
		(*inFileStream) << myMapObjectPtr->YPos << " \n";
		++myIter;
	}
	(*inFileStream) << "END_MAP_OBJECTS\n";
}

void LevelManager::saveEnemyObjects(std::fstream* inFileStream) {
	(*inFileStream) << "BEGIN_ENEMY_OBJECTS\n";
	std::list<Enemy>::iterator myIter = myEnemyManager->myEnemies.begin();
	std::list<Enemy>::iterator myEnd = myEnemyManager->myEnemies.end();
	while (myIter != myEnd) {
		Enemy* myEnemyObjectPtr = &(*myIter);
		(*inFileStream) << myEnemyObjectPtr->AssetID << " ";
		(*inFileStream) << myEnemyObjectPtr->StartPosX << " ";
		(*inFileStream) << myEnemyObjectPtr->StartPosY << " \n";
		++myIter;
	}
	(*inFileStream) << "END_ENEMY_OBJECTS\n";
}

void LevelManager::doSaveLevel(std::string inLevelName) {
	std::fstream myFileStream;
	myFileStream.open(inLevelName, std::fstream::out);
	saveMapObjects(&myFileStream);
	saveEnemyObjects(&myFileStream);
	myFileStream.close();
}