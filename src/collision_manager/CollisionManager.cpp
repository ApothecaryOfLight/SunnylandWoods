//CollisionManager.cpp

/*
TODO:
	Collision dimensions are stored in AssetFactory
	Object positions are stored in their respective Object Managers
	CollisionManager is not to be queried by other managers, but instead is to crunch collisions and run the simulation.
*/
#include "CollisionManager.hpp"

#include <iostream>

#include <list>

#include <SDL.h>
#include <SDL_image.h>

#include "../camera_manager/CameraManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../enemy_manager/EnemyManager.hpp"
#include "../player_manager/PlayerManager.hpp"
#include "../asset_factory/AssetFactory.hpp"
#include "../map_manager/MapManager.hpp"
#include "../input_manager/InputManager.hpp"
#include "../id_manager/id_manager.hpp"

CollisionManager::CollisionManager ( SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory, MapManager * inMapManager, EnemyManager * inEnemyManager, PlayerManager * inPlayerManager, InputManager * inInputManager, IDManager * inIDManager ) {
	myRen = inRen;
	myCameraManager = inCameraManager;
	myAssetFactory = inAssetFactory;
	myEnemyManager = inEnemyManager;
	myPlayerManager = inPlayerManager;
	myMapManager = inMapManager;
	myInputManager = inInputManager;
	myIDManager = inIDManager;

	//myCollisionBoxLookup = new std::list<int>[1000];
	//myCollisionBoxCounter = 0;

	/*myCollisionBoxes = new int*[1000];//Series of stacked rectangles from top to bottom stored as contiguous ranges.
	for( int i=0; i<=999; i++ ) {
		myCollisionBoxes[i] = new int[4];
	}*/

	//doInitializeCollisions(); //TODO: Run only after level is fully loaded
}

CollisionManager::~CollisionManager ( void ) {
	/*delete[] myCollisionBoxLookup;

	for( int i=0; i<=999; i++ ) {
		delete[] myCollisionBoxes[i];
	}
	delete[] myCollisionBoxes;*/
}


//1) Match each object in the managers with their asset IDs from AssetFactory
//2) Combine that data with information from CameraManager
//3) Get their coordinants from ObjectManagers
//4) Use that to create an x/y/w/h collision box for each object
	//FUTURE: Make collision boxes stack frame by frame for finer collisions.
void CollisionManager::doInitializeCollisions ( void ) {
	//doAddCollisionRectangle( 0, 0, 0, 90, 90 );
	//StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0]; //Needs to reference myCameraManager for player coords, not AssetFactory (!!) but also not PlayerManager
	//SDL_Rect * myRect = &myStaticAssetPtr->myRect_dst;
	//std::cout << myRect->x << "/" << myRect->y << "/" << myRect->w << "/" << myRect->h << std::endl;

	//X) Enumerate map objects.
	/*std::list<int> myActiveMapObjects = myMapManager->myActiveMapObjects;
	std::list<int>::iterator MapObjs_myStart = myActiveMapObjects.begin();
	std::list<int>::iterator MapObjs_myEnd = myActiveMapObjects.end();
	while( MapObjs_myStart != MapObjs_myEnd ) {
		int newID = (*MapObjs_myStart);
		doAddCollisionRectangle(
			0,
			myRect->x,
			myRect->y,
			myRect->x + myRect->w,
			myRect->y + myRect->h
		);
		++MapObjs_myStart;
	}*/
}

void CollisionManager::doAddCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 ) {
	/*myCollisionBoxLookup[inID].push_back( myCollisionBoxCounter );
	myCollisionBoxes[myCollisionBoxCounter][0] = inX1;
	myCollisionBoxes[myCollisionBoxCounter][1] = inY1;
	myCollisionBoxes[myCollisionBoxCounter][2] = inX1+inX2;
	myCollisionBoxes[myCollisionBoxCounter][3] = inY1+inY2;
	myCollisionBoxCounter++;*/
}

void CollisionManager::doUpdateCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 ) {
	/*myCollisionBoxes[myCollisionBoxCounter][0] = inX1;
	myCollisionBoxes[myCollisionBoxCounter][1] = inY1;
	myCollisionBoxes[myCollisionBoxCounter][2] = inX1+inX2;
	myCollisionBoxes[myCollisionBoxCounter][3] = inY1+inY2;*/
}

//1) Match each on-screen object to its Asset ID
//2) Construct the rect for each onscreen object by applying the ObjectManager's positional values
//3) Draw the collision boxes.
void CollisionManager::doDrawCollisionBoxes ( void ) {
	SDL_SetRenderDrawColor( myRen, 255, 0, 0, SDL_ALPHA_OPAQUE);

	int myMagnification = myCameraManager->magnification;

	//1) Enumerate and draw player collisions
	StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0];
	SDL_Rect * myRect = &myStaticAssetPtr->myRect_dst;
	SDL_RenderDrawLine(
		myRen,
		myRect->x,
		myRect->y,
		myRect->x+myRect->w,
		myRect->y+myRect->h
	);
	//2) Enumerate and draw enemy collisions
	
	

	//3) Enumerate and draw map object collisions
	//std::list<int> myActiveMapObjects( myMapManager->myActiveMapObjects );
	std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
	std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
	while( MapObjs_myStart != MapObjs_myEnd ) {
		int MapObjectID = (*MapObjs_myStart);
//std::cout << MapObjectID << std::endl;
		MapObject * myMapObject = myMapManager->getMapObject( MapObjectID );
//std::cout << myMapObject->myGlobalID << std::endl;
		int MapObjectAssetID = myMapObject->myAssetID;
//std::cout << MapObjectAssetID << std::endl;
		SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_dst;
		myCollisionBox.x = myMapObject->XPos;
		myCollisionBox.y = myMapObject->YPos;

		SDL_RenderDrawLine(
			myRen,
			(myCollisionBox.x - myCameraManager->PlayerX_level) * myMagnification,
			(myCollisionBox.y - myCameraManager->PlayerY_level) * myMagnification,
			(myCollisionBox.x+myCollisionBox.w - myCameraManager->PlayerX_level) * myMagnification,
			(myCollisionBox.y+myCollisionBox.h - myCameraManager->PlayerY_level) * myMagnification
		);

		++MapObjs_myStart;
	}

	/*SDL_Rect * myMapObject = myMapManager->myCollisionBoxes[0];
	SDL_RenderDrawLine(
		myRen,
		myMapObject->x - myCameraManager->PlayerX_level,
		myMapObject->y - myCameraManager->PlayerY_level,
		myMapObject->x+(myMapObject->w) - myCameraManager->PlayerX_level,
		myMapObject->y+(myMapObject->h) - myCameraManager->PlayerY_level
	);*/


	//4) Enumerate and draw collectibles collisions
	//5) Enumerate and draw clickables collisions
	//6) Enumerate and draw interactables collisions and ranges.
}

void CollisionManager::doGameLogic ( void ) {
	myPlayerManager->doGameLogic();
	doPlayerCollisions(); //2) TODO: Check for player collisions
	//3) doEnemyLogicTick();
	//4) TODO: Check for enemy collisions.
	//5) doCollectiblesLogicTick();
	//6) doClickablesLogicTick();
	//7) doInteractablesLogicTick();
}

void CollisionManager::doPlayerCollisions ( void ) {
	myPlayerManager->FPSCounter++;
	if( myInputManager->inputFlag_Jumping == true || myInputManager->inputFlag_Right == true || myInputManager->inputFlag_Left == true  ) {
		if( myPlayerManager->FPSCounter >= 2 ) {
			myPlayerManager->anim_frame_Player++;
			if( myPlayerManager->anim_frame_Player >= myPlayerManager->anim_frame_Player_MAX ) {
				myPlayerManager->anim_frame_Player = 0;
			}
			myPlayerManager->FPSCounter = 0;
		}
	}
	else if( myInputManager->inputFlag_Right == false || myInputManager->inputFlag_Left == false || myInputManager->inputFlag_Jumping == false ) {
		if( myPlayerManager->FPSCounter >= 3 ) {
			myPlayerManager->anim_frame_Player++;
			if( myPlayerManager->anim_frame_Player >= myPlayerManager->anim_frame_Player_MAX ) {
				myPlayerManager->anim_frame_Player = 0;
			}
			myPlayerManager->FPSCounter = 0;
		}
	}

	if( myInputManager->inputFlag_Jumping == true ) {
		int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
		int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
		SDL_Rect myCopy;
		myCopy = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myCopy.x = myCopy.y-9;

std::cout << std::endl << "Player: " << myCopy.x << "/" << myCopy.y << std::endl;
std::cout << myCopy.w << "/" << myCopy.h << std::endl;
std::cout << "Box: " << myMapManager->myCollisionBoxes[0]->x << "/" << myMapManager->myCollisionBoxes[0]->y << std::endl;
std::cout << myMapManager->myCollisionBoxes[0]->w << "/" << myMapManager->myCollisionBoxes[0]->h << std::endl;





		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while( MapObjs_myStart != MapObjs_myEnd ) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject * myMapObject = myMapManager->getMapObject( MapObjectID );
			int MapObjectAssetID = myMapObject->myAssetID;
			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_dst;
			int myMagnification = myCameraManager->magnification;
			myCollisionBox.x = (myCollisionBox.x - myCameraManager->PlayerX_level) * myMagnification;
			myCollisionBox.y = (myCollisionBox.y - myCameraManager->PlayerY_level) * myMagnification;
			myCollisionBox.w = (myCollisionBox.w - myCameraManager->PlayerX_level) * myMagnification;
			myCollisionBox.h = (myCollisionBox.h - myCameraManager->PlayerY_level) * myMagnification;

			/*SDL_RenderDrawLine(
				myRen,
				(myCollisionBox.x - myCameraManager->PlayerX_level) * myMagnification,
				(myCollisionBox.y - myCameraManager->PlayerY_level) * myMagnification,
				(myCollisionBox.x+myCollisionBox.w - myCameraManager->PlayerX_level) * myMagnification,
				(myCollisionBox.y+myCollisionBox.h - myCameraManager->PlayerY_level) * myMagnification
			);*/
			if( SDL_HasIntersection( &myCopy, &myCollisionBox ) == SDL_TRUE ) {
					std::cout << "Collision to the top!" << std::endl;
					return; //Barrier.
			}
			++MapObjs_myStart;
		}

		




		/*if( SDL_HasIntersection( &myCopy, myMapManager->myCollisionBoxes[0] ) == SDL_TRUE ) {
				std::cout << "Collision to the top!" << std::endl;
				return; //Barrier.
		}*/
		myCameraManager->PlayerY_screen = (myCameraManager->PlayerY_screen)-9;
		/*if( myCameraManager->PlayerX_screen <= -500 ) { //TODO: A camera/level setting for max y
			std::cout << "Hitting screen-wall top!" << std::endl;
			myCameraManager->PlayerX_screen = myCameraManager->ScreenWall_Left;
			myCameraManager->PlayerX_level = myCameraManager->PlayerX_level-9;
		}*/
		myAssetFactory->doAdjustPlayerDest( myCameraManager->PlayerX_screen, myCameraManager->PlayerY_screen );
	}

	//TODO: Replace input flags with case, and ensure in InputManager that only one flag can be set at a time.
	if( myInputManager->inputFlag_Left == true && myInputManager->inputFlag_Right == false ) { //Logic for moving left
		//1) Create a rect that would be where we want to move the player
		int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
		int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
		SDL_Rect myCopy;
		myCopy = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myCopy.x = myCopy.x-9;
		//2) Check that rect against the map objects IN THIS SECTOR and adjacent sectors
		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while( MapObjs_myStart != MapObjs_myEnd ) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject * myMapObject = myMapManager->getMapObject( MapObjectID );
			int MapObjectAssetID = myMapObject->myAssetID;
			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_dst;
			int myMagnification = myCameraManager->magnification;
			myCollisionBox.x = (myCollisionBox.x - myCameraManager->PlayerX_level) * myMagnification;
			myCollisionBox.y = (myCollisionBox.y - myCameraManager->PlayerY_level) * myMagnification;
			myCollisionBox.w = (myCollisionBox.w - myCameraManager->PlayerX_level) * myMagnification;
			myCollisionBox.h = (myCollisionBox.h - myCameraManager->PlayerY_level) * myMagnification;

			/*SDL_RenderDrawLine(
				myRen,
				(myCollisionBox.x - myCameraManager->PlayerX_level) * myMagnification,
				(myCollisionBox.y - myCameraManager->PlayerY_level) * myMagnification,
				(myCollisionBox.x+myCollisionBox.w - myCameraManager->PlayerX_level) * myMagnification,
				(myCollisionBox.y+myCollisionBox.h - myCameraManager->PlayerY_level) * myMagnification
			);*/
			if( SDL_HasIntersection( &myCopy, &myCollisionBox ) == SDL_TRUE ) {
					std::cout << "Collision to the top!" << std::endl;
					return; //Barrier.
			}
			++MapObjs_myStart;
		}


		/*if( SDL_HasIntersection( &myCopy, myMapManager->myCollisionBoxes[0] ) == SDL_TRUE ) {
				std::cout << "Collision to the left!" << std::endl;


std::cout << std::endl << "Player: " << myCopy.x << "/" << myCopy.y << std::endl;
std::cout << myCopy.w << "/" << myCopy.h << std::endl;
std::cout << "Box: " << myMapManager->myCollisionBoxes[0]->x << "/" << myMapManager->myCollisionBoxes[0]->y << std::endl;
std::cout << myMapManager->myCollisionBoxes[0]->w << "/" << myMapManager->myCollisionBoxes[0]->h << std::endl;


				return; //Barrier.
		}*/

		//3) Break or apply
		myCameraManager->PlayerX_screen = (myCameraManager->PlayerX_screen)-9;
		if( myCameraManager->PlayerX_screen <= myCameraManager->ScreenWall_Left ) {
			std::cout << "Hitting screen-wall left!" << std::endl;
			myCameraManager->PlayerX_screen = myCameraManager->ScreenWall_Left;
			myCameraManager->PlayerX_level = myCameraManager->PlayerX_level-9;
			return;
		}
		myAssetFactory->doAdjustPlayerDest( myCameraManager->PlayerX_screen, myCameraManager->PlayerY_screen ); //TODO: Remove, AssetFactory shouldn't track object positions, they're not the same thing.
	}
	else if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == true ) { //Logic for moving right
		//1) Create a rect that would be where we want to move the player
		int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
		int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
		SDL_Rect myCopy;
		myCopy = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myCopy.x = myCopy.x+9;
		//2) Check that rect against the map objects IN THIS SECTOR and adjacent sectors
		/*if( SDL_HasIntersection( &myCopy, myMapManager->myCollisionBoxes[0] ) == SDL_TRUE ) {
				std::cout << "Collision to the right!" << std::endl;
				return; //Barrier.
		}*/

		myCameraManager->PlayerX_screen = (myCameraManager->PlayerX_screen)+9;
		if( myCameraManager->PlayerX_screen + (myCameraManager->PlayerSize_X*myCameraManager->magnification) >= myCameraManager->ScreenWall_Right ) {
			std::cout << "Hitting screen-wall right!" << std::endl;
			myCameraManager->PlayerX_screen = myCameraManager->ScreenWall_Right - (myCameraManager->PlayerSize_X*myCameraManager->magnification);
			myCameraManager->PlayerX_level = myCameraManager->PlayerX_level+9;
			return;
		}
		myAssetFactory->doAdjustPlayerDest( myCameraManager->PlayerX_screen, myCameraManager->PlayerY_screen ); //TODO: Remove, AssetFactory shouldn't track object positions, they're not the same thing.
	}



	//1) Get the player's collisions and sector
	/*int SectorX = myCameraManager->CurrentSectorX, SectorY = myCameraManager->CurrentSectorY;
	int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
	int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
	//std::cout << PlayerAnimationType << "/" << PlayerAnimationFrame << std::endl;
	SDL_Rect * myPlayerCollisionBox = &(myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst);
	//2) Iterate over the map objects, testing for collisions,
	//for iter over list
		if( SDL_HasIntersection( myPlayerCollisionBox, myMapManager->myCollisionBoxes[0] ) == SDL_TRUE ) {
				std::cout << "Collision!" << std::endl;
		}*/
	//3) Iterate over the enemy objects, testing for collisions
}

void CollisionManager::doEnemyCollisions ( void ) {
	
}

