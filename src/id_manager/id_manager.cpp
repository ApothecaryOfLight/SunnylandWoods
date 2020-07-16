
#include "id_manager.hpp"

IDManager::IDManager ( void ) {
	IDcounter = 0;
	//myGlobalObjects = new GlobalObject[5000];
}

int IDManager::getNewID ( void ) {
	if( _retiredIDs.empty() ) {
		return ++IDcounter;
	} else {
		int ret = _retiredIDs.front();
		_retiredIDs.pop_front();
		//myGlobalObjects[ret] = nullptr;
		return ret;
	}
}

void IDManager::retireID ( int inID ) {
	_retiredIDs.push_back( inID );
}

