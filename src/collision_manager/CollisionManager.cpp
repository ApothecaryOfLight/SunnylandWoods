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

#include "../logger/logger.hpp"

CollisionManager::CollisionManager ( Logger * inLogger, SDL_Renderer * inRen, CameraManager * inCameraManager, AssetFactory * inAssetFactory, MapManager * inMapManager, EnemyManager * inEnemyManager, PlayerManager * inPlayerManager, InputManager * inInputManager, IDManager * inIDManager ) {
	myRen = inRen;
	myLogger = inLogger;
	myLogger->log( "Initializing Collision Manager." );
	myCameraManager = inCameraManager;
	myAssetFactory = inAssetFactory;
	myEnemyManager = inEnemyManager;
	myPlayerManager = inPlayerManager;
	myMapManager = inMapManager;
	myInputManager = inInputManager;
	myIDManager = inIDManager;
}

CollisionManager::~CollisionManager ( void ) {

}

void CollisionManager::doInitializeCollisions ( void ) {

}

void CollisionManager::doAddCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 ) {

}

void CollisionManager::doUpdateCollisionRectangle ( int inID, int inX1, int inY1, int inX2, int inY2 ) {

}

//1) Match each on-screen object to its Asset ID
//2) Construct the rect for each onscreen object by applying the ObjectManager's positional values
//3) Draw the collision boxes.
void CollisionManager::doDrawCollisionBoxes ( void ) {
	SDL_SetRenderDrawColor( myRen, 255, 0, 0, SDL_ALPHA_OPAQUE);

	int magnification = myCameraManager->magnification;

	//0) Draw screen walls


	//1) Enumerate and draw player collisions
	StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0];
	/*SDL_Rect * myPlayer = &myStaticAssetPtr->myRect_dst;
	SDL_RenderDrawLine(
		myRen,
		myPlayer->x,
		myPlayer->y,
		myPlayer->x+ myPlayer->w,
		myPlayer->y+ myPlayer->h
	);*/
	SDL_Rect myPlayerCollisionBox = myStaticAssetPtr->myRect_dst;
	//myPlayerCollisionBox.x *= magnification;
	//myPlayerCollisionBox.y *= magnification;
	myPlayerCollisionBox.w *= magnification;
	myPlayerCollisionBox.h *= magnification;

	SDL_RenderDrawRect(
		myRen,
		&myPlayerCollisionBox
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
		myCollisionBox.x = (myMapObject->XPos - myCameraManager->PlayerX_level) * magnification;
		myCollisionBox.y = (myMapObject->YPos - myCameraManager->PlayerY_level) * magnification;
		myCollisionBox.w *= magnification;
		myCollisionBox.h *= magnification;

		/*SDL_RenderDrawLine(
			myRen,
			(myCollisionBox.x - myCameraManager->PlayerX_level) * myMagnification,
			(myCollisionBox.y - myCameraManager->PlayerY_level) * myMagnification,
			(myCollisionBox.x+myCollisionBox.w - myCameraManager->PlayerX_level) * myMagnification,
			(myCollisionBox.y+myCollisionBox.h - myCameraManager->PlayerY_level) * myMagnification
		);*/

		if (myMapObject->has_collided == true) {
			myMapManager->decrement_collided(MapObjectID);
			SDL_SetRenderDrawColor(
				myRen,
				0,
				100,
				100,
				0
			);
		}
		else {
			SDL_SetRenderDrawColor(
				myRen,
				255,
				0,
				0,
				0
			);
		}

		SDL_RenderDrawRect(
			myRen,
			&myCollisionBox
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
	doPlayerCollisions();
	//3) doEnemyLogicTick();
	//4) TODO: Check for enemy collisions.
	//5) doCollectiblesLogicTick();
	//6) doClickablesLogicTick();
	//7) doInteractablesLogicTick();
}

void CollisionManager::doPlayerCollisions ( void ) {
	//TODO: Collisions with: enemies, interactables, collectibles
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

	int myMagnification = myCameraManager->magnification;
	int movement_increment = 3 * myMagnification;
	if( myInputManager->inputFlag_Jumping == true && myPlayerManager->jump_counter < 30 ) {
		myPlayerManager->jump_counter++;
		int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
		int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
		SDL_Rect myCopy;
		myCopy = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myCopy.y = myCopy.y- movement_increment;

		bool is_jumping = true;
		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while( MapObjs_myStart != MapObjs_myEnd ) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject * myMapObject = myMapManager->getMapObject( MapObjectID );
			int MapObjectAssetID = myMapObject->myAssetID;

			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_dst;

			myCollisionBox.x = (myMapObject->XPos - myCameraManager->PlayerX_level) * myMagnification;
			myCollisionBox.y = (myMapObject->YPos - myCameraManager->PlayerY_level) * myMagnification;
			myCollisionBox.w *= myMagnification;
			myCollisionBox.h *= myMagnification;

			//1) Get leading top edge of player collision box
			int top_collision_player = myCopy.y;
			int left_collision_player = myCopy.x;
			int right_collision_player = myCopy.x + myCopy.w;

			//2) Check against bottom edge of map object
			int top_map_object = myCollisionBox.y;
			int bottom_map_object = myCollisionBox.y + myCollisionBox.h;
			int left_map_object = myCollisionBox.x;
			int right_map_object = myCollisionBox.x + myCollisionBox.w;

			//3) Check for collision
			if (
				left_collision_player <= right_map_object && left_collision_player >= left_map_object ||
				right_collision_player >= right_map_object && right_collision_player <= right_map_object
			) {
				if( top_collision_player <= bottom_map_object && top_collision_player >= top_map_object ) {
					myLogger->log("Collision to top!");
					myLogger->log(MapObjectID);
					myMapManager->mark_collided(MapObjectID);
					is_jumping = false;
					break;
				}
			}

			++MapObjs_myStart;
		}

		if (is_jumping == true) {
			myCameraManager->PlayerY_screen = (myCameraManager->PlayerY_screen) - movement_increment;
			myAssetFactory->doAdjustPlayerDest(myCameraManager->PlayerX_screen, myCameraManager->PlayerY_screen);
		}
	}
	else { //jump key not pressed or jump_counter has been maxed out.
		//Check for collisions below, otherwise apply gravity effect.
		int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
		int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
		SDL_Rect myCopy;
		myCopy = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myCopy.y = myCopy.y + (movement_increment*3); //You fall faster than you rise

		bool is_falling = true;
		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while (MapObjs_myStart != MapObjs_myEnd) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
			int MapObjectAssetID = myMapObject->myAssetID;

			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[MapObjectAssetID]->myRect_dst;

			myCollisionBox.x = (myMapObject->XPos - myCameraManager->PlayerX_level) * myMagnification;
			myCollisionBox.y = (myMapObject->YPos - myCameraManager->PlayerY_level) * myMagnification;
			myCollisionBox.w *= myMagnification;
			myCollisionBox.h *= myMagnification;

			//1) Get leading top edge of player collision box
			int bottom_collision_player = myCopy.y + myCopy.h;
			int left_collision_player = myCopy.x;
			int right_collision_player = myCopy.x + myCopy.w;

			//2) Check against bottom edge of map object
			int top_map_object = myCollisionBox.y;
			int bottom_map_object = myCollisionBox.y + myCollisionBox.h;
			int left_map_object = myCollisionBox.x;
			int right_map_object = myCollisionBox.x + myCollisionBox.w;

			//3) Check for collision
			if (
				left_collision_player <= right_map_object && left_collision_player >= left_map_object ||
				right_collision_player >= right_map_object && right_collision_player <= right_map_object
				) {
				if (bottom_collision_player <= bottom_map_object && bottom_collision_player >= top_map_object) {
					myLogger->log("Collision to bottom!");
					myLogger->log(MapObjectID);
					myMapManager->mark_collided(MapObjectID);
					is_falling = false;
					break;
				}
			}

			++MapObjs_myStart;
		}
		if (is_falling == true) {
			myPlayerManager->jump_counter = 31;
			myCameraManager->PlayerY_screen = (myCameraManager->PlayerY_screen) + movement_increment;
			myAssetFactory->doAdjustPlayerDest(myCameraManager->PlayerX_screen, myCameraManager->PlayerY_screen);
		}
		else {
			myPlayerManager->jump_counter = 0;
		}
	}

	//TODO: Replace input flags with case, and ensure in InputManager that only one flag can be set at a time.
	if( myInputManager->inputFlag_Left == true && myInputManager->inputFlag_Right == false ) { //Logic for moving left
		//myLogger->log("Running left ");
		//1) Create a rect that would be where we want to move the player
		int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
		int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
		SDL_Rect myCopy;
		myCopy = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myCopy.x = myCopy.x- movement_increment;

		//2) Check that rect against the map objects IN THIS SECTOR and adjacent sectors
		bool is_colliding_left = false;
		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while( MapObjs_myStart != MapObjs_myEnd ) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject * myMapObject = myMapManager->getMapObject( MapObjectID );

			int MapObjectAssetID = myMapObject->myAssetID;
			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_dst;

			int myMagnification = myCameraManager->magnification;

			myCollisionBox.x = (myMapObject->XPos - myCameraManager->PlayerX_level) * myMagnification;
			myCollisionBox.y = (myMapObject->YPos - myCameraManager->PlayerY_level) * myMagnification;
			myCollisionBox.w *= myMagnification;
			myCollisionBox.h *= myMagnification;

			int left_collision_player = myCopy.x;
			int top_collision_player = myCopy.y;
			int bottom_collision_player = myCopy.y + myCopy.w;

			int right_collision_object = myCollisionBox.x + myCollisionBox.w;
			int left_collision_object = myCollisionBox.x;
			int top_collision_object = myCollisionBox.y;
			int bottom_collision_object = myCollisionBox.y + myCollisionBox.w;

			if (
				top_collision_player > top_collision_object && top_collision_player < bottom_collision_object ||
				bottom_collision_player > top_collision_object && bottom_collision_player < bottom_collision_object	
			) {
				if (left_collision_player <= right_collision_object && left_collision_player >= left_collision_object) {
					myLogger->log("Collision on left!");
					myLogger->log(MapObjectID);
					myMapManager->mark_collided(MapObjectID);
					is_colliding_left = true;
					break;
				}
			}

			++MapObjs_myStart;
		}

		//3) Conditionally apply.
		if (is_colliding_left == false) {
			myCameraManager->PlayerX_screen = (myCameraManager->PlayerX_screen) - movement_increment;
			if (myCameraManager->PlayerX_screen <= myCameraManager->ScreenWall_Left) {
				myLogger->log("Hitting screen-wall left!");
				myCameraManager->PlayerX_screen = myCameraManager->ScreenWall_Left;
				myCameraManager->PlayerX_level = myCameraManager->PlayerX_level - movement_increment;
			}
			myAssetFactory->doAdjustPlayerDest(myCameraManager->PlayerX_screen, myCameraManager->PlayerY_screen); //TODO: Remove, AssetFactory shouldn't track object positions, they're not the same thing.
		}
	}
	else if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == true ) { //Logic for moving right
		//1) Create a rect that would be where we want to move the player
		int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
		int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
		SDL_Rect myCopy;
		myCopy = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myCopy.x = myCopy.x+ movement_increment;
		//2) Check that rect against the map objects IN THIS SECTOR and adjacent sectors
		bool is_colliding_right = false;
		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while (MapObjs_myStart != MapObjs_myEnd) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);

			int MapObjectAssetID = myMapObject->myAssetID;
			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[MapObjectAssetID]->myRect_dst;

			int myMagnification = myCameraManager->magnification;

			myCollisionBox.x = (myMapObject->XPos - myCameraManager->PlayerX_level) * myMagnification;
			myCollisionBox.y = (myMapObject->YPos - myCameraManager->PlayerY_level) * myMagnification;
			myCollisionBox.w *= myMagnification;
			myCollisionBox.h *= myMagnification;

			int right_collision_player = myCopy.x + myCopy.w;
			int top_collision_player = myCopy.y;
			int bottom_collision_player = myCopy.y + myCopy.w;

			int right_collision_object = myCollisionBox.x + myCollisionBox.w;
			int left_collision_object = myCollisionBox.x;
			int top_collision_object = myCollisionBox.y;
			int bottom_collision_object = myCollisionBox.y + myCollisionBox.w;

			if (
				top_collision_player > top_collision_object && top_collision_player < bottom_collision_object ||
				bottom_collision_player > top_collision_object && bottom_collision_player < bottom_collision_object
				) {
				if (right_collision_player <= right_collision_object && right_collision_player >= left_collision_object) {
					myLogger->log("Collision on right!");
					myLogger->log(MapObjectID);
					myMapManager->mark_collided(MapObjectID);
					is_colliding_right = true;
				}
			}

			++MapObjs_myStart;
		}

		if (is_colliding_right == false) {
			myCameraManager->PlayerX_screen = (myCameraManager->PlayerX_screen) + movement_increment;
			if (myCameraManager->PlayerX_screen + (myCameraManager->PlayerSize_X * myCameraManager->magnification) >= myCameraManager->ScreenWall_Right) {
				myLogger->log("Hitting screen-wall right!");
				myCameraManager->PlayerX_screen = myCameraManager->ScreenWall_Right - (myCameraManager->PlayerSize_X * myCameraManager->magnification);
				myCameraManager->PlayerX_level = myCameraManager->PlayerX_level + movement_increment;
				//return;
			}
			myAssetFactory->doAdjustPlayerDest(myCameraManager->PlayerX_screen, myCameraManager->PlayerY_screen); //TODO: Remove, AssetFactory shouldn't track object positions, they're not the same thing.
		}
	}
}

void CollisionManager::doEnemyCollisions ( void ) {
	
}

