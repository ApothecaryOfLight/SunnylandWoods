#ifndef _ID_MANAGER_
#define _ID_MANAGER_

#include <list>

class IDManager {
public:
	IDManager ( void );
	int getNewID ( void );
	void retireID ( int inID );
	int getIDcounter();
private:
	std::list<int> _retiredIDs;
	int IDcounter;
};

#endif
