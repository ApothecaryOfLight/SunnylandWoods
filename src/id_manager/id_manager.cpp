#include "id_manager.hpp"

IDManager::IDManager ( void ) {
	IDcounter = 0;
}

int IDManager::getNewID ( void ) {
	if( _retiredIDs.empty() ) {
		return ++IDcounter;
	} else {
		int ret = _retiredIDs.front();
		_retiredIDs.pop_front();
		return ret;
	}
}

void IDManager::retireID ( int inID ) {
	_retiredIDs.push_back( inID );
}

int IDManager::getIDcounter() {
	return IDcounter;
}
