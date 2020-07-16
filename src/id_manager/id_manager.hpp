#ifndef _ID_MANAGER_
#define _ID_MANAGER_

#include <list>

class GlobalObject {
public:
	GlobalObject( int inID, int inOwner );
};

class IDManager {
public:
	IDManager ( void );
	int getNewID ( void );
	//int getNewID ( int inOwningManager );
	void retireID ( int inID );
private:
	std::list<int> _retiredIDs;
	//GlobalObject * myGlobalObjects;
	int IDcounter;
};

#endif
