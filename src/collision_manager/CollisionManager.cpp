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

vector2d::vector2d(int inX, int inY) {
	x = inX;
	y = inY;
}

int vector2d::magnitude() {
	return x + y;
}

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

void CollisionManager::doDrawCollisionBoxes ( void ) {
	SDL_SetRenderDrawColor( myRen, 255, 0, 0, SDL_ALPHA_OPAQUE);

	int magnification = myCameraManager->magnification;

	//0) Draw screen walls

	//1) Enumerate and draw player collisions
	int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
	int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
	StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame];
	SDL_Rect myPlayerDrawnCollisionBox = myStaticAssetPtr->myRect_dst;
	myPlayerDrawnCollisionBox.x = myPlayerManager->PlayerGameCoordX - myCameraManager->CameraX;
	myPlayerDrawnCollisionBox.y = myPlayerManager->PlayerGameCoordY - myCameraManager->CameraY;

	if (!myInputManager->inputFlag_Left && myInputManager->inputFlag_Right && !myInputManager->inputFlag_Jumping && myPlayerManager->jump_counter < max_jump_height) {
		myPlayerDrawnCollisionBox.x -= 18;
	}
	if (myInputManager->inputFlag_Jumping && !myInputManager->isPlayerFacingLeft) {
		//myPlayerDrawnCollisionBox.x -= 5;
	}

	/*myPlayerDrawnCollisionBox.x *= magnification;
	myPlayerDrawnCollisionBox.y *= magnification;
	myPlayerDrawnCollisionBox.w *= magnification;
	myPlayerDrawnCollisionBox.h *= magnification;*/

	SDL_RenderDrawRect(
		myRen,
		&myPlayerDrawnCollisionBox
	);

	//2) Enumerate and draw enemy collisions

	//3) Enumerate and draw map object collisions
	//std::list<int> myActiveMapObjects( myMapManager->myActiveMapObjects );
	std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
	std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
	while( MapObjs_myStart != MapObjs_myEnd ) {
		int MapObjectID = (*MapObjs_myStart);
		MapObject * myMapObject = myMapManager->getMapObject( MapObjectID );
		int MapObjectAssetID = myMapObject->myAssetID;
		SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_dst;
		myCollisionBox.x = myMapObject->XPos - myCameraManager->CameraX;
		myCollisionBox.y = myMapObject->YPos - myCameraManager->CameraY;
		myCollisionBox.w *= magnification;
		myCollisionBox.h *= magnification;

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

	int movement_increment = 7;
	if( myInputManager->inputFlag_Jumping == true && myPlayerManager->jump_counter < max_jump_height) { //jumping logic
		myPlayerManager->jump_counter++;
		int PlayerAnimationFrame = 0;
		int PlayerAnimationType = 4;
		SDL_Rect myPlayer;
		myPlayer = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myPlayer.x = myPlayerManager->PlayerGameCoordX;
		myPlayer.y = myPlayerManager->PlayerGameCoordY - movement_increment;

		bool is_jumping = true;
		bool is_colliding_left = false;
		bool is_moving_left = false;
		bool is_colliding_right = false;
		bool is_moving_right = false;
		int distance_remaining = 0;
		int distance_remaining_left = 0;
		int distance_remaining_right = 0;
		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while( MapObjs_myStart != MapObjs_myEnd ) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject * myMapObject = myMapManager->getMapObject( MapObjectID );
			int MapObjectAssetID = myMapObject->myAssetID;

			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_dst;

			myCollisionBox.x = myMapObject->XPos;
			myCollisionBox.y = myMapObject->YPos;

			//1) Get leading top edge of player collision box
			int top_collision_player = myPlayer.y;
			int left_collision_player = myPlayer.x;
			int right_collision_player = myPlayer.x + myPlayer.w;
			int bottom_collision_player = myPlayer.y + myPlayer.h - 10;

			if (!myInputManager->isPlayerFacingLeft) {
				right_collision_player -= 10;
			}
			if (myInputManager->isPlayerFacingLeft) {
				left_collision_player += 10;
			}

			//2) Check against bottom edge of map object
			int top_collision_object = myCollisionBox.y;
			int bottom_collision_object = myCollisionBox.y + myCollisionBox.h;
			int left_collision_object = myCollisionBox.x;
			int right_collision_object = myCollisionBox.x + myCollisionBox.w;

			//3) Check for collision to top
			if (
				left_collision_player < right_collision_object && left_collision_player > left_collision_object ||
				right_collision_player > right_collision_object && right_collision_player < right_collision_object ||
				right_collision_object > left_collision_player && right_collision_object < right_collision_player ||
				left_collision_object > left_collision_player && left_collision_object < right_collision_player
			) {
				if( top_collision_player <= bottom_collision_object && top_collision_player >= top_collision_object ) {
					myMapManager->mark_collided(MapObjectID);
					is_jumping = false;
					int overlap_after_potential_movement = (bottom_collision_object - top_collision_player) - movement_increment;
					if (distance_remaining == 0 || overlap_after_potential_movement > distance_remaining && overlap_after_potential_movement < 0) {
						distance_remaining = overlap_after_potential_movement;
					}
				}
			}

			//4) If moving left, check for collision to left.
			if (myInputManager->inputFlag_Left == true && myInputManager->inputFlag_Right == false) {
				is_moving_left = true;
				left_collision_player -= (10 + movement_increment);
				if (
					top_collision_player > top_collision_object && top_collision_player < bottom_collision_object ||
					bottom_collision_player > top_collision_object && bottom_collision_player < bottom_collision_object ||
					top_collision_object > top_collision_player && top_collision_object < bottom_collision_player ||
					bottom_collision_object > top_collision_player && bottom_collision_object < bottom_collision_player
					) {
					if (left_collision_player <= right_collision_object && left_collision_player >= left_collision_object) {
						myMapManager->mark_collided(MapObjectID);
						is_colliding_left = true;
						int overlap_after_potential_movement = (left_collision_player + movement_increment) - right_collision_object;
						if (distance_remaining_left == 0 && overlap_after_potential_movement > 0) {
							distance_remaining_left = overlap_after_potential_movement;
						}
					}
				}
			}

			//5) If moving right, check for collision to right.
			if (myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == true) {
				is_moving_right = true;
				right_collision_player += 10 + movement_increment;
				if (
					top_collision_player > top_collision_object && top_collision_player < bottom_collision_object ||
					bottom_collision_player > top_collision_object && bottom_collision_player < bottom_collision_object ||
					top_collision_object > top_collision_player && top_collision_object < bottom_collision_player ||
					bottom_collision_object > top_collision_player && bottom_collision_object < bottom_collision_player
					) {
					if (right_collision_player <= right_collision_object && right_collision_player >= left_collision_object) {
						myMapManager->mark_collided(MapObjectID);
						is_colliding_right = true;
						int overlap_after_potential_movement = left_collision_object - (right_collision_player - movement_increment);
						if (distance_remaining_right == 0 && overlap_after_potential_movement > 0) {
							distance_remaining_right = overlap_after_potential_movement;
						}
					}
				}
			}

			++MapObjs_myStart;
		}

		//If there is no collision to the top, move the player up.
		if (is_jumping == true) {
			myPlayerManager->PlayerGameCoordY -= movement_increment;
			if ((myPlayerManager->PlayerGameCoordY - myCameraManager->CameraY) <= myCameraManager->ScreenWall_Top) {
				myCameraManager->CameraY -= movement_increment;
			}
		}
		//If there is a gap smaller than the movement increment between the player and the object above, close it.
		if (distance_remaining != 0) {
			myPlayerManager->PlayerGameCoordY -= distance_remaining * -1;
		}

		//If there is no collision to the left, while the player is trying to move left, move the player left.
		if (is_colliding_left == false && is_moving_left) {
			myPlayerManager->PlayerGameCoordX -= movement_increment;
			if ((myPlayerManager->PlayerGameCoordX - myCameraManager->CameraX) <= myCameraManager->ScreenWall_Left) {
				myCameraManager->CameraX -= movement_increment;
			}
		}
		if (distance_remaining_left > 0) {
			myPlayerManager->PlayerGameCoordX -= distance_remaining_left;
		}


		//If there is no collision to the right, while the player is trying to move right, move the player right.
		if (is_colliding_right == false && is_moving_right) {
			myPlayerManager->PlayerGameCoordX += movement_increment;
			if ((myPlayerManager->PlayerGameCoordX - myCameraManager->CameraX) >= myCameraManager->ScreenWall_Right) {
				myCameraManager->CameraX += movement_increment;
			}
		}
		if (distance_remaining_right > 0) {
			myPlayerManager->PlayerGameCoordX += distance_remaining_right;
		}
		return;
	}
	else { //jump key not pressed or jump_counter has been maxed out.
		//Check for collisions below, otherwise apply gravity effect.
		int PlayerAnimationFrame = 0;
		int PlayerAnimationType = 0;

		SDL_Rect myPlayerDest = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myPlayerDest.x = myPlayerManager->PlayerGameCoordX;
		myPlayerDest.y = myPlayerManager->PlayerGameCoordY + (movement_increment * 2); //You fall faster than you rise

		bool is_falling = true;
		int distance_remaining = 0;
		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while (MapObjs_myStart != MapObjs_myEnd) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
			int MapObjectAssetID = myMapObject->myAssetID;

			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[MapObjectAssetID]->myRect_dst;

			myCollisionBox.x = myMapObject->XPos;
			myCollisionBox.y = myMapObject->YPos;

			//1) Get bottom edge of player collision box
			int bottom_collision_player = myPlayerDest.y + myPlayerDest.h - 10;
			int left_collision_player = myPlayerDest.x;
			int right_collision_player = myPlayerDest.x + myPlayerDest.w;

			//2) Check against top edge of map object
			int top_collision_object = myCollisionBox.y;
			int bottom_collision_object = myCollisionBox.y + myCollisionBox.h;
			int left_collision_object = myCollisionBox.x;
			int right_collision_object = myCollisionBox.x + myCollisionBox.w;

			//3) Check for collision
			if (
				left_collision_player < right_collision_object && left_collision_player > left_collision_object ||
				right_collision_player < right_collision_object && right_collision_player > left_collision_object ||
				right_collision_object > left_collision_player && right_collision_object < right_collision_player ||
				left_collision_object > left_collision_player && left_collision_object < right_collision_player
			) {
				if (bottom_collision_player <= bottom_collision_object && bottom_collision_player >= top_collision_object) {
					myMapManager->mark_collided(MapObjectID);
					is_falling = false;
					int overlap_after_potential_movement = top_collision_object - (bottom_collision_player - (movement_increment*2));
					if (distance_remaining == 0 && overlap_after_potential_movement > 0) {
						distance_remaining = overlap_after_potential_movement;
					}
					else if (distance_remaining > overlap_after_potential_movement) {
						distance_remaining = overlap_after_potential_movement;
					}
					if (overlap_after_potential_movement < distance_remaining ) {
						distance_remaining = overlap_after_potential_movement;
					}
				}
			}

			++MapObjs_myStart;
		}
		if (is_falling == true) {
			myPlayerManager->jump_counter = max_jump_height+1;
			myPlayerManager->PlayerGameCoordY += movement_increment * 2;
			if ((myPlayerManager->PlayerGameCoordY - myCameraManager->CameraY) >= myCameraManager->ScreenWall_Bottom) {
				myCameraManager->CameraY += movement_increment*2;
			}
		}
		else {
			myPlayerManager->jump_counter = 0;
			if (distance_remaining > 0) {
				myPlayerManager->PlayerGameCoordY += distance_remaining - 10;
			}
		}
	}

	if( myInputManager->inputFlag_Left == true && myInputManager->inputFlag_Right == false ) { //Logic for moving left
		//1) Create an object containing player coordinates and size.
		int PlayerAnimationFrame = 0;
		int PlayerAnimationType = 1;
		SDL_Rect myPlayerDest = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myPlayerDest.x = myPlayerManager->PlayerGameCoordX;
		myPlayerDest.y = myPlayerManager->PlayerGameCoordY;

		//2) Check that rect against the map objects IN THIS SECTOR and adjacent sectors
		bool is_colliding_left = false;
		int distance_remaining = 0;
		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while( MapObjs_myStart != MapObjs_myEnd ) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject * myMapObject = myMapManager->getMapObject( MapObjectID );

			int MapObjectAssetID = myMapObject->myAssetID;
			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_dst;

			myCollisionBox.x = myMapObject->XPos;
			myCollisionBox.y = myMapObject->YPos;

			int left_collision_player = myPlayerDest.x - movement_increment;
			int top_collision_player = myPlayerDest.y;
			int bottom_collision_player = myPlayerDest.y + myPlayerDest.h;

			int right_collision_object = myCollisionBox.x + myCollisionBox.w;
			int left_collision_object = myCollisionBox.x;
			int top_collision_object = myCollisionBox.y;
			int bottom_collision_object = myCollisionBox.y + myCollisionBox.h;

			if (
				top_collision_player > top_collision_object && top_collision_player < bottom_collision_object ||
				bottom_collision_player > top_collision_object && bottom_collision_player < bottom_collision_object ||
				top_collision_object > top_collision_player && top_collision_object < bottom_collision_player ||
				bottom_collision_object > top_collision_player && bottom_collision_object < bottom_collision_player
			) {
				if (left_collision_player <= right_collision_object && left_collision_player >= left_collision_object) {
					myMapManager->mark_collided(MapObjectID);
					is_colliding_left = true;
					int overlap_after_potential_movement = (left_collision_player+movement_increment) - right_collision_object;
					if (distance_remaining == 0 && overlap_after_potential_movement > 0 ) {
						distance_remaining = overlap_after_potential_movement;
					}
				}
			}

			++MapObjs_myStart;
		}

		//3) Conditionally apply.
		if (is_colliding_left == false) {
			myPlayerManager->PlayerGameCoordX -= movement_increment;
			if ((myPlayerManager->PlayerGameCoordX - myCameraManager->CameraX) <= myCameraManager->ScreenWall_Left) {
				myCameraManager->CameraX -= movement_increment;
			}
		}
		if (distance_remaining > 0) {
			myPlayerManager->PlayerGameCoordX -= distance_remaining;
		}
	}
	else if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == true ) { //Logic for moving right
		//1) Create a rect that would be where we want to move the player
		int PlayerAnimationFrame = 0;
		int PlayerAnimationType = 1;

		SDL_Rect myPlayerDest = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame]->myRect_dst;
		myPlayerDest.x = myPlayerManager->PlayerGameCoordX + movement_increment - 18;
		myPlayerDest.y = myPlayerManager->PlayerGameCoordY;

		//2) Check that rect against the map objects IN THIS SECTOR and adjacent sectors
		bool is_colliding_right = false;
		int distance_remaining = 0;
		std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while (MapObjs_myStart != MapObjs_myEnd) {
			int MapObjectID = (*MapObjs_myStart);
			MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);

			int MapObjectAssetID = myMapObject->myAssetID;
			SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[MapObjectAssetID]->myRect_dst;

			myCollisionBox.x = myMapObject->XPos;
			myCollisionBox.y = myMapObject->YPos;

			int right_collision_player = myPlayerDest.x + myPlayerDest.w;
			int top_collision_player = myPlayerDest.y;
			int bottom_collision_player = myPlayerDest.y + myPlayerDest.h;

			int right_collision_object = myCollisionBox.x + myCollisionBox.w;
			int left_collision_object = myCollisionBox.x;
			int top_collision_object = myCollisionBox.y;
			int bottom_collision_object = myCollisionBox.y + myCollisionBox.h;

			if (
				top_collision_player > top_collision_object && top_collision_player < bottom_collision_object ||
				bottom_collision_player > top_collision_object && bottom_collision_player < bottom_collision_object ||
				top_collision_object > top_collision_player && top_collision_object < bottom_collision_player ||
				bottom_collision_object > top_collision_player && bottom_collision_object < bottom_collision_player
				) {
				if (right_collision_player <= right_collision_object && right_collision_player >= left_collision_object) {
					myMapManager->mark_collided(MapObjectID);
					is_colliding_right = true;
					int overlap_after_potential_movement = left_collision_object - (right_collision_player - movement_increment);
					if (distance_remaining == 0 && overlap_after_potential_movement > 0) {
						distance_remaining = overlap_after_potential_movement;
					}
				}
			}
			++MapObjs_myStart;
		}

		if (is_colliding_right == false) {
			myPlayerManager->PlayerGameCoordX += movement_increment;
			if ((myPlayerManager->PlayerGameCoordX - myCameraManager->CameraX) >= myCameraManager->ScreenWall_Right) {
				myCameraManager->CameraX += movement_increment;
			}
		}
		if (distance_remaining > 0) {
			myPlayerManager->PlayerGameCoordX += distance_remaining;
		}
	}
}

void CollisionManager::doEnemyCollisions ( void ) {
	
}

