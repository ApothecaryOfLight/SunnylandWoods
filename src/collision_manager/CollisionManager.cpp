//CollisionManager.cpp

/*
TODO:
	Collision dimensions are stored in AssetFactory
	Object positions are stored in their respective Object Managers
	CollisionManager is not to be queried by other managers, but instead is to crunch collisions and run the simulation.
*/
#include "CollisionManager.hpp"

#include <algorithm>
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

	//0) Draw screen walls

	//1) Enumerate and draw player collisions
	int PlayerAnimationFrame = myPlayerManager->anim_frame_Player;
	int PlayerAnimationType = myPlayerManager->PlayerAnimationType;
	StaticAsset * myStaticAssetPtr = myAssetFactory->myAnimatedAssets[PlayerAnimationType]->myStaticAssets[PlayerAnimationFrame];
	SDL_Rect myPlayerDrawnCollisionBox = myStaticAssetPtr->myRect_src;
	coord myPlayerCoords = myCameraManager->translate_coords(myPlayerManager->PlayerGameCoordX, myPlayerManager->PlayerGameCoordY);
	myPlayerDrawnCollisionBox.x = myPlayerCoords.x;
	myPlayerDrawnCollisionBox.y = myPlayerCoords.y;
	myPlayerDrawnCollisionBox.w *= myCameraManager->zoom;
	myPlayerDrawnCollisionBox.h *= myCameraManager->zoom;

	if (!myInputManager->inputFlag_Left && myInputManager->inputFlag_Right && !myInputManager->inputFlag_Jumping && myPlayerManager->jump_counter < max_jump_height) {
		myPlayerDrawnCollisionBox.x -= 18;
	}
	if (myInputManager->inputFlag_Jumping && !myInputManager->isPlayerFacingLeft) {
		//myPlayerDrawnCollisionBox.x -= 5;
	}

	SDL_RenderDrawRect(
		myRen,
		&myPlayerDrawnCollisionBox
	);

	//2) Enumerate and draw enemy collisions
	std::list<Enemy>::iterator myIter = myEnemyManager->myEnemies.begin(), myEnd = myEnemyManager->myEnemies.end();
	while (myIter != myEnd) {
		Enemy* myEnemy = &(*myIter);
		int myAssetID = myEnemy->AssetID;
		StaticAsset* myStaticAssetPtr = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[myEnemy->Frame];
		SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[myEnemy->Frame]->myRect_src;
		coord myEnemyCoords = myCameraManager->translate_coords(myEnemy->EnemyGameCoordX, myEnemy->EnemyGameCoordY);
		myEnemyCollisionBox.x = myEnemyCoords.x;
		myEnemyCollisionBox.y = myEnemyCoords.y;
		myEnemyCollisionBox.w *= myCameraManager->zoom;
		myEnemyCollisionBox.h *= myCameraManager->zoom;
		SDL_RenderDrawRect(
			myRen,
			&myEnemyCollisionBox
		);
		++myIter;
	}

	//3) Enumerate and draw map object collisions
	std::list<int>::iterator MapObjs_myStart = myMapManager->myActiveMapObjects.begin();
	std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
	while( MapObjs_myStart != MapObjs_myEnd ) {
		int MapObjectID = (*MapObjs_myStart);
		MapObject * myMapObject = myMapManager->getMapObject( MapObjectID );
		int MapObjectAssetID = myMapObject->myAssetID;
		SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_src;
		coord myCollisionBoxCoords = myCameraManager->translate_coords(myMapObject->XPos, myMapObject->YPos);
		myCollisionBox.x = myCollisionBoxCoords.x;
		myCollisionBox.y = myCollisionBoxCoords.y;
		myCollisionBox.w *= myCameraManager->zoom;
		myCollisionBox.h *= myCameraManager->zoom;

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

	//7) Draw screen lines
	SDL_SetRenderDrawColor(
		myRen,
		0,
		255,
		0,
		0
	);
	SDL_RenderDrawLine(
		myRen,
		myCameraManager->ScreenWall_Left,
		0,
		myCameraManager->ScreenWall_Left,
		myCameraManager->ScreenHeight
	);
	SDL_RenderDrawLine(
		myRen,
		myCameraManager->ScreenWall_Right,
		0,
		myCameraManager->ScreenWall_Right,
		myCameraManager->ScreenHeight
	);
	SDL_RenderDrawLine(
		myRen,
		0,
		myCameraManager->ScreenWall_Top,
		myCameraManager->ScreenWidth,
		myCameraManager->ScreenWall_Top
	);
	SDL_RenderDrawLine(
		myRen,
		0,
		myCameraManager->ScreenWall_Bottom,
		myCameraManager->ScreenWidth,
		myCameraManager->ScreenWall_Bottom
	);
}

void CollisionManager::doGameLogic ( void ) {
	doPlayerCollisions();
	doEnemyCollisions();
	doPlayerEnemyCollisions();
}

/*
Player/Map Object collisions
*/
inline int CollisionManager::isWalkingPlayerCollidingLeftMapObject(int MapObjectID) {
	//1) Get the collision box of the player.
	SDL_Rect myPlayerCollisionBox = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0]->myRect_src;
	int PlayerEdge_Left = myPlayerManager->PlayerGameCoordX;
	int PlayerEdge_Top = myPlayerManager->PlayerGameCoordY + 10;
	int PlayerEdge_Bottom = myPlayerManager->PlayerGameCoordY + myPlayerCollisionBox.h;
	int PlayerEdge_Right = myPlayerManager->PlayerGameCoordX + myPlayerCollisionBox.w - 10;
	int PlayerEdge_LeftAfterMovement = myPlayerManager->PlayerGameCoordX - myPlayerManager->player_movement_increment;

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Left = myMapObject->XPos;

	//3) Determine if player, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(PlayerEdge_Top, MapObjectEdge_Top) < std::min(PlayerEdge_Bottom, MapObjectEdge_Bottom)) {
		if (std::max(PlayerEdge_LeftAfterMovement, MapObjectEdge_Left) < std::min(PlayerEdge_Right, MapObjectEdge_Right)) {
			return PlayerEdge_Left - MapObjectEdge_Right;
		}
	}
	return -1;
}

inline int CollisionManager::isWalkingPlayerCollidingRightMapObject(int MapObjectID) {
	//1) Get the collision box of the player.
	SDL_Rect myPlayerCollisionBox = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0]->myRect_src;
	int PlayerEdge_Right = myPlayerManager->PlayerGameCoordX + myPlayerCollisionBox.w;
	int PlayerEdge_Top = myPlayerManager->PlayerGameCoordY + 10;
	int PlayerEdge_Bottom = myPlayerManager->PlayerGameCoordY + myPlayerCollisionBox.h;
	int PlayerEdge_Left = myPlayerManager->PlayerGameCoordX + 10;
	int PlayerEdge_RightAfterMovement = PlayerEdge_Right + myPlayerManager->player_movement_increment;

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Left = myMapObject->XPos;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;

	//3)Determine if player, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(PlayerEdge_Top, MapObjectEdge_Top) < std::min(PlayerEdge_Bottom, MapObjectEdge_Bottom)) {
		if (std::max(PlayerEdge_Left, MapObjectEdge_Left) < std::min(PlayerEdge_RightAfterMovement, MapObjectEdge_Right)) {
			return MapObjectEdge_Left - PlayerEdge_Right;
		}
	}
	return -1;
}

inline int CollisionManager::isJumpingPlayerCollidingUpMapObject(int MapObjectID) {
	//1) Get the collision box of the player.
	SDL_Rect myPlayerCollisionBox = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0]->myRect_src;
	int PlayerEdge_Right = myPlayerManager->PlayerGameCoordX + myPlayerCollisionBox.w;
	int PlayerEdge_Bottom = myPlayerManager->PlayerGameCoordY + myPlayerCollisionBox.h - 20;
	int PlayerEdge_Left = myPlayerManager->PlayerGameCoordX;
	int PlayerEdge_Top = myPlayerManager->PlayerGameCoordY + 12;
	int PlayerEdge_TopAfterMovement = myPlayerManager->PlayerGameCoordY - myPlayerManager->player_movement_increment;

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Left = myMapObject->XPos;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;

	//3)Determine if player, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(PlayerEdge_TopAfterMovement, MapObjectEdge_Top) < std::min(PlayerEdge_Bottom, MapObjectEdge_Bottom)) {
		if (std::max(PlayerEdge_Left, MapObjectEdge_Left) < std::min(PlayerEdge_Right, MapObjectEdge_Right)) {
			return PlayerEdge_Top - MapObjectEdge_Bottom;
		}
	}
	return -1;
}

inline int CollisionManager::isJumpingPlayerCollidingLeftMapObject(int MapObjectID) {
	//1) Get the collision box of the player.
	SDL_Rect myPlayerCollisionBox = myAssetFactory->myAnimatedAssets[4]->myStaticAssets[0]->myRect_src;
	int PlayerEdge_Right = myPlayerManager->PlayerGameCoordX + myPlayerCollisionBox.w;
	int PlayerEdge_Bottom = myPlayerManager->PlayerGameCoordY + myPlayerCollisionBox.h;
	int PlayerEdge_Left = myPlayerManager->PlayerGameCoordX;
	int PlayerEdge_Top = myPlayerManager->PlayerGameCoordY + 12;
	int PlayerEdge_LeftAfterMovement = myPlayerManager->PlayerGameCoordX - myPlayerManager->player_movement_increment;

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Left = myMapObject->XPos;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;

	//3)Determine if player, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(PlayerEdge_Top, MapObjectEdge_Top) < std::min(PlayerEdge_Bottom, MapObjectEdge_Bottom)) {
		if (std::max(PlayerEdge_LeftAfterMovement, MapObjectEdge_Left) < std::min(PlayerEdge_Right, MapObjectEdge_Right)) {
			return PlayerEdge_LeftAfterMovement - MapObjectEdge_Right;
		}
	}
	return -1;
}

inline int CollisionManager::isJumpingPlayerCollidingRightMapObject(int MapObjectID) {
	//1) Get the collision box of the player.
	SDL_Rect myPlayerCollisionBox = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0]->myRect_src;
	int PlayerEdge_Right = myPlayerManager->PlayerGameCoordX + myPlayerCollisionBox.w;
	int PlayerEdge_Bottom = myPlayerManager->PlayerGameCoordY + myPlayerCollisionBox.h - 20;
	int PlayerEdge_Left = myPlayerManager->PlayerGameCoordX;
	int PlayerEdge_Top = myPlayerManager->PlayerGameCoordY + 12;
	int PlayerEdge_RightAfterMovement = PlayerEdge_Right + myPlayerManager->player_movement_increment;

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Left = myMapObject->XPos;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;

	//3)Determine if player, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(PlayerEdge_Top, MapObjectEdge_Top) < std::min(PlayerEdge_Bottom, MapObjectEdge_Bottom)) {
		if (std::max(PlayerEdge_Left, MapObjectEdge_Left) < std::min(PlayerEdge_RightAfterMovement, MapObjectEdge_Right)) {
			return MapObjectEdge_Left - PlayerEdge_RightAfterMovement;
		}
	}
	return -1;
}

inline int CollisionManager::isFallingPlayerCollidingDownMapObject(int MapObjectID) {
	//1) Get the collision box of the player.
	SDL_Rect myPlayerCollisionBox = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0]->myRect_src;
	int PlayerEdge_Right = myPlayerManager->PlayerGameCoordX + myPlayerCollisionBox.w;
	int PlayerEdge_Bottom = myPlayerManager->PlayerGameCoordY + myPlayerCollisionBox.h;
	int PlayerEdge_Left = myPlayerManager->PlayerGameCoordX;
	int PlayerEdge_Top = myPlayerManager->PlayerGameCoordY + 12;
	int PlayerEdge_BottomAfterMovement = PlayerEdge_Bottom + (myPlayerManager->player_movement_increment*2);

	if (!myInputManager->inputFlag_Left && !myInputManager->inputFlag_Right) {
		if (myInputManager->isPlayerFacingLeft) {
			PlayerEdge_Right -= 10;
		}
		else {
			PlayerEdge_Left += 10;
		}
	}

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Left = myMapObject->XPos;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;

	//3)Determine if player, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(PlayerEdge_Top, MapObjectEdge_Top) < std::min(PlayerEdge_BottomAfterMovement, MapObjectEdge_Bottom)) {
		if (std::max(PlayerEdge_Left, MapObjectEdge_Left) < std::min(PlayerEdge_Right, MapObjectEdge_Right)) {
			return MapObjectEdge_Top - PlayerEdge_Bottom;
		}
	}
	return -1;
}

void CollisionManager::doPlayerCollisions ( void ) {
	if( myInputManager->inputFlag_Jumping == true && (myPlayerManager->jump_counter < max_jump_height || myPlayerManager->jump_peak_counter < max_jump_peak) ) { //jumping logic

		//2) Check that rect against the map objects IN THIS SECTOR and adjacent sectors
		bool is_colliding_up = false, is_colliding_left = false, is_colliding_right = false;
		int distance_remaining_top = -1, distance_remaining_left = -1, distance_remaining_right = -1;
		std::list<int>::iterator MapObjs_myIter = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();

		while (MapObjs_myIter != MapObjs_myEnd) {
			int distance_top = isJumpingPlayerCollidingUpMapObject((*MapObjs_myIter));
			int distance_left = -1;
			int distance_right = -1;
			if (myInputManager->inputFlag_Left && !myInputManager->inputFlag_Right) {
				distance_left = isJumpingPlayerCollidingLeftMapObject((*MapObjs_myIter));
			}
			else if (myInputManager->inputFlag_Right && !myInputManager->inputFlag_Left) {
				distance_right = isJumpingPlayerCollidingRightMapObject((*MapObjs_myIter));
			}
			if (distance_top != -1) {
				is_colliding_up = true;
				myMapManager->mark_collided((*MapObjs_myIter));
				if (distance_remaining_top == -1) {
					distance_remaining_top = distance_top;
				}
				else if (distance_top < distance_remaining_top) {
					distance_remaining_top = distance_top;
				}
			}
			if (distance_left != -1) {
				is_colliding_left = true;
				myMapManager->mark_collided((*MapObjs_myIter));
				if (distance_remaining_left == -1) {
					distance_remaining_left = distance_left;
				}
				else if (distance_left < distance_remaining_left) {
					distance_remaining_left = distance_left;
				}
			}
			if (distance_right != -1) {
				is_colliding_right = true;
				myMapManager->mark_collided((*MapObjs_myIter));
				if (distance_remaining_right == -1) {
					distance_remaining_right = distance_right;
				}
				else if (distance_right < distance_remaining_right) {
					distance_remaining_right = distance_right;
				}
			}
			++MapObjs_myIter;
		}


		if (myPlayerManager->jump_counter >= max_jump_height) {
			myPlayerManager->jump_peak_counter++;
		}
		else {
			myPlayerManager->jump_counter++;
			if (is_colliding_up == false) {
				myPlayerManager->PlayerGameCoordY -= myPlayerManager->player_movement_increment;
				if (((myPlayerManager->PlayerGameCoordY* myCameraManager->zoom) - myCameraManager->CameraY) <= myCameraManager->ScreenWall_Top) {
					myCameraManager->CameraY -= (myPlayerManager->player_movement_increment* myCameraManager->zoom);
				}
			}
			else if (distance_remaining_top > 0) {
				myPlayerManager->PlayerGameCoordY -= distance_remaining_top;
			}
		}

		//Jumping left
		if (myInputManager->inputFlag_Left && !myInputManager->inputFlag_Right) {
			if (is_colliding_left == false) {
				myPlayerManager->PlayerGameCoordX -= myPlayerManager->player_movement_increment;
				if (((myPlayerManager->PlayerGameCoordX * myCameraManager->zoom) - myCameraManager->CameraX) <= myCameraManager->ScreenWall_Left) {
					myCameraManager->CameraX -= myPlayerManager->player_movement_increment * myCameraManager->zoom;
				}
			}
			else if (distance_remaining_left > 0) {
				myLogger->log("distance remaining");
				myLogger->log(distance_remaining_left);
				myPlayerManager->PlayerGameCoordX -= distance_remaining_left;
			}
		}

		//Jumping right
		else if (!myInputManager->inputFlag_Left && myInputManager->inputFlag_Right) {
			if (is_colliding_right == false) {
				myPlayerManager->PlayerGameCoordX += myPlayerManager->player_movement_increment;
				if (((myPlayerManager->PlayerGameCoordX * myCameraManager->zoom) - myCameraManager->CameraX) >= myCameraManager->ScreenWall_Right) {
					myCameraManager->CameraX += myPlayerManager->player_movement_increment * myCameraManager->zoom;
				}
			}
			else if (distance_remaining_right > 0) {
				myPlayerManager->PlayerGameCoordX += distance_remaining_right;
			}
		}
		return;
	}
	else { //jump key not pressed or jump_counter has been maxed out.
		//Check for collisions below, otherwise apply gravity effect.
		bool is_colliding_down = false, is_colliding_left = false, is_colliding_right = false;
		int distance_remaining_bottom = -1, distance_remaining_left = -1, distance_remaining_right = -1;
		std::list<int>::iterator MapObjs_myIter = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();

		while (MapObjs_myIter != MapObjs_myEnd) {
			int distance_bottom = isFallingPlayerCollidingDownMapObject((*MapObjs_myIter));
			if (distance_bottom != -1) {
				is_colliding_down = true;
				myMapManager->mark_collided((*MapObjs_myIter));
				if (distance_remaining_bottom == -1) {
					distance_remaining_bottom = distance_bottom;
				}
				else if (distance_bottom < distance_remaining_bottom) {
					distance_remaining_bottom = distance_bottom;
				}
			}
			++MapObjs_myIter;
		}

		if (is_colliding_down == false) {
			myPlayerManager->jump_counter = max_jump_height + 1;
			myPlayerManager->PlayerGameCoordY += (myPlayerManager->player_movement_increment*2);
			if ((((myPlayerManager->PlayerGameCoordY * myCameraManager->zoom) + myCameraManager->PlayerSize_Y) - myCameraManager->CameraY) >= myCameraManager->ScreenWall_Bottom) {
				myCameraManager->CameraY += (myPlayerManager->player_movement_increment*2) * myCameraManager->zoom;
			}
		}
		else {
			myPlayerManager->jump_counter = 0;
			myPlayerManager->jump_peak_counter = 0;
			if (distance_remaining_bottom > 0) {
				myPlayerManager->PlayerGameCoordY += distance_remaining_bottom;
			}
		}

		if (myInputManager->inputFlag_Left && !myInputManager->inputFlag_Right) {
			int distance_left = -1;
			MapObjs_myIter = myMapManager->myActiveMapObjects.begin();
			while (MapObjs_myIter != MapObjs_myEnd) {
				distance_left = isWalkingPlayerCollidingLeftMapObject((*MapObjs_myIter));
				if (distance_left != -1) {
					is_colliding_left = true;
					myMapManager->mark_collided((*MapObjs_myIter));
					if (distance_remaining_left == -1) {
						distance_remaining_left = distance_left;
					}
					else if (distance_left < distance_remaining_left) {
						distance_remaining_left = distance_left;
					}
				}
				++MapObjs_myIter;
			}
			if (is_colliding_left == false) {
				myPlayerManager->PlayerGameCoordX -= myPlayerManager->player_movement_increment;
				if (((myPlayerManager->PlayerGameCoordX * myCameraManager->zoom) - myCameraManager->CameraX) <= myCameraManager->ScreenWall_Left) {
					myCameraManager->CameraX -= myPlayerManager->player_movement_increment * myCameraManager->zoom;
				}
			}
			else if (distance_remaining_left > 0) {
				myPlayerManager->PlayerGameCoordX -= distance_remaining_left;
			}
		}
		else if (myInputManager->inputFlag_Right && !myInputManager->inputFlag_Left) {
			int distance_right = -1;
			MapObjs_myIter = myMapManager->myActiveMapObjects.begin();
			while (MapObjs_myIter != MapObjs_myEnd) {
				distance_right = isWalkingPlayerCollidingRightMapObject((*MapObjs_myIter));
				if (distance_right != -1) {
					is_colliding_right = true;
					myMapManager->mark_collided((*MapObjs_myIter));
					if (distance_remaining_right == -1) {
						distance_remaining_right = distance_right;
					}
					else if (distance_right < distance_remaining_right) {
						distance_remaining_right = distance_right;
					}
				}
				++MapObjs_myIter;
			}
			if (is_colliding_right == false) {
				myPlayerManager->PlayerGameCoordX += myPlayerManager->player_movement_increment;
				if ((((myPlayerManager->PlayerGameCoordX * myCameraManager->zoom)+myCameraManager->PlayerSize_X) - myCameraManager->CameraX) >= myCameraManager->ScreenWall_Right) {
					myCameraManager->CameraX += myPlayerManager->player_movement_increment * myCameraManager->zoom;
				}
			}
			else if (distance_remaining_right > 0) {
				myPlayerManager->PlayerGameCoordX += distance_remaining_right;
			}
		}
		return;
	}

	if( myInputManager->inputFlag_Left == true && myInputManager->inputFlag_Right == false ) { //Logic for moving left
		//1) Check that rect against the map objects IN THIS SECTOR and adjacent sectors
		bool is_colliding_left = false;
		int distance_remaining = -1;
		std::list<int>::iterator MapObjs_myIter = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while( MapObjs_myIter != MapObjs_myEnd ) {
			int distance = isWalkingPlayerCollidingLeftMapObject((*MapObjs_myIter));
			if (distance != -1) {
				is_colliding_left = true;
				myMapManager->mark_collided((*MapObjs_myIter));
				if (distance_remaining == -1) {
					distance_remaining = distance;
				} else if(distance < distance_remaining) {
					distance_remaining = distance;
				}
			}
			++MapObjs_myIter;
		}

		//2) Conditionally apply.
		if (is_colliding_left == false) {
			myPlayerManager->PlayerGameCoordX -= myPlayerManager->player_movement_increment;
			if (((myPlayerManager->PlayerGameCoordX * myCameraManager->zoom) - myCameraManager->CameraX) <= myCameraManager->ScreenWall_Left) {
				myCameraManager->CameraX -= myPlayerManager->player_movement_increment * myCameraManager->zoom;
			}
		} else if (distance_remaining > 0) {
			myLogger->log(distance_remaining);
			myPlayerManager->PlayerGameCoordX -= distance_remaining;
		}
		return;
	}
	else if( myInputManager->inputFlag_Left == false && myInputManager->inputFlag_Right == true ) { //Logic for moving right
		//2) Check that rect against the map objects IN THIS SECTOR and adjacent sectors
		bool is_colliding_right = false;
		int distance_remaining = -1;
		std::list<int>::iterator MapObjs_myIter = myMapManager->myActiveMapObjects.begin();
		std::list<int>::iterator MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
		while (MapObjs_myIter != MapObjs_myEnd) {
			int distance = isWalkingPlayerCollidingRightMapObject((*MapObjs_myIter));
			if (distance != -1) {
				is_colliding_right = true;
				myMapManager->mark_collided((*MapObjs_myIter));
				if (distance_remaining == -1) {
					distance_remaining = distance;
				} else if (distance < distance_remaining) {
					distance_remaining = distance;
				}
			}
			++MapObjs_myIter;
		}

		if (is_colliding_right == false) {
			myPlayerManager->PlayerGameCoordX += myPlayerManager->player_movement_increment;
			if ((((myPlayerManager->PlayerGameCoordX * myCameraManager->zoom)+myCameraManager->PlayerSize_X) - myCameraManager->CameraX) >= myCameraManager->ScreenWall_Right) {
				myCameraManager->CameraX += myPlayerManager->player_movement_increment * myCameraManager->zoom;
			}
		} else if (distance_remaining > 0) {
			myPlayerManager->PlayerGameCoordX += distance_remaining;
		}
		return;
	}
}

/*
Enemy Collisions
*/
inline int CollisionManager::isFallingEnemyCollidingDownMapObject(Enemy* EnemyPtr, int MapObjectID) {
	//1) Get the collision box of the enemy.
	int myAssetID = EnemyPtr->AssetID;
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[0]->myRect_src;
	int EnemyEdge_Right = EnemyPtr->EnemyGameCoordX + myEnemyCollisionBox.w;
	int EnemyEdge_Bottom = EnemyPtr->EnemyGameCoordY + myEnemyCollisionBox.h;
	int EnemyEdge_Left = EnemyPtr->EnemyGameCoordX;
	int EnemyEdge_Top = EnemyPtr->EnemyGameCoordY + 12;
	int EnemyEdge_BottomAfterMovement = EnemyEdge_Bottom + 14;

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Left = myMapObject->XPos;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;

	//3)Determine if Enemy, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(EnemyEdge_Top, MapObjectEdge_Top) < std::min(EnemyEdge_BottomAfterMovement, MapObjectEdge_Bottom)) {
		if (std::max(EnemyEdge_Left, MapObjectEdge_Left) < std::min(EnemyEdge_Right, MapObjectEdge_Right)) {
			return MapObjectEdge_Top - EnemyEdge_Bottom;
		}
	}
	return -1;
}

inline int CollisionManager::isFlyingEnemyCollidingUpMapObject(Enemy* EnemyPtr, int MapObjectID) {
	//1) Get the collision box of the enemy.
	int myAssetID = EnemyPtr->AssetID;
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[0]->myRect_src;
	int EnemyEdge_Right = EnemyPtr->EnemyGameCoordX + myEnemyCollisionBox.w;
	int EnemyEdge_Bottom = EnemyPtr->EnemyGameCoordY + myEnemyCollisionBox.h;
	int EnemyEdge_Left = EnemyPtr->EnemyGameCoordX;
	int EnemyEdge_Top = EnemyPtr->EnemyGameCoordY - 12;
	int EnemyEdge_TopAfterMovement = EnemyEdge_Top - 14;

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Left = myMapObject->XPos;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;

	//3)Determine if Enemy, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(EnemyEdge_Bottom, MapObjectEdge_Bottom) < std::min(EnemyEdge_TopAfterMovement, MapObjectEdge_Top)) {
		if (std::max(EnemyEdge_Left, MapObjectEdge_Left) < std::min(EnemyEdge_Right, MapObjectEdge_Right)) {
			return MapObjectEdge_Top - EnemyEdge_Bottom;
		}
	}
	return -1;
}

inline int CollisionManager::isWalkingEnemyCollidingLeftMapObject(Enemy* EnemyPtr, int MapObjectID) {
	//1) Get the collision box of the enemy.
	int myAssetID = EnemyPtr->AssetID;
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[0]->myRect_src;
	int EnemyEdge_Right = EnemyPtr->EnemyGameCoordX + myEnemyCollisionBox.w;
	int EnemyEdge_Bottom = EnemyPtr->EnemyGameCoordY + myEnemyCollisionBox.h;
	int EnemyEdge_Left = EnemyPtr->EnemyGameCoordX;
	int EnemyEdge_Top = EnemyPtr->EnemyGameCoordY;
	int EnemyEdge_LeftAfterMovement = EnemyEdge_Left - 7;

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Left = myMapObject->XPos;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;

	//3)Determine if Enemy, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(EnemyEdge_Top, MapObjectEdge_Top) < std::min(EnemyEdge_Bottom, MapObjectEdge_Bottom)) {
		if (std::max(EnemyEdge_LeftAfterMovement, MapObjectEdge_Left) < std::min(EnemyEdge_Right, MapObjectEdge_Right)) {
			return EnemyEdge_Left - MapObjectEdge_Right;
		}
	}
	return -1;
}

inline int CollisionManager::isWalkingEnemyCollidingRightMapObject(Enemy* EnemyPtr, int MapObjectID) {
	//1) Get the collision box of the enemy.
	int myAssetID = EnemyPtr->AssetID;
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[0]->myRect_src;
	int EnemyEdge_Right = EnemyPtr->EnemyGameCoordX + myEnemyCollisionBox.w;
	int EnemyEdge_Bottom = EnemyPtr->EnemyGameCoordY + myEnemyCollisionBox.h;
	int EnemyEdge_Left = EnemyPtr->EnemyGameCoordX;
	int EnemyEdge_Top = EnemyPtr->EnemyGameCoordY;
	int EnemyEdge_RightAfterMovement = EnemyEdge_Right + 7;

	//2) Get the collision box of the map object.
	MapObject* myMapObject = myMapManager->getMapObject(MapObjectID);
	SDL_Rect myMapObjectCollisionBox = myAssetFactory->myStaticAssets[myMapObject->myAssetID]->myRect_src;
	int MapObjectEdge_Left = myMapObject->XPos;
	int MapObjectEdge_Top = myMapObject->YPos;
	int MapObjectEdge_Bottom = myMapObject->YPos + myMapObjectCollisionBox.h;
	int MapObjectEdge_Right = myMapObject->XPos + myMapObjectCollisionBox.w;

	//3)Determine if Enemy, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(EnemyEdge_Top, MapObjectEdge_Top) < std::min(EnemyEdge_Bottom, MapObjectEdge_Bottom)) {
		if (std::max(EnemyEdge_Left, MapObjectEdge_Left) < std::min(EnemyEdge_RightAfterMovement, MapObjectEdge_Right)) {
			return MapObjectEdge_Left - EnemyEdge_Right;
		}
	}
	return -1;
}

void CollisionManager::doEnemyCollisions ( void ) {
	std::list<Enemy>::iterator myEnemyIter = myEnemyManager->myEnemies.begin(), myEnemyEnd = myEnemyManager->myEnemies.end();
	while (myEnemyIter != myEnemyEnd) {
		switch ((*myEnemyIter).AssetID) {
		case 5:
			doAntCollisions(&(*myEnemyIter));
			break;
		case 7:
			doGatorCollisions(&(*myEnemyIter));
			break;
		}
		++myEnemyIter;
	}
}

void CollisionManager::doAntCollisions(Enemy* EnemyPtr) {
	std::list<int>::iterator MapObjs_myIter = myMapManager->myActiveMapObjects.begin(), MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
	bool is_colliding_down = false, is_colliding_left = false, is_colliding_right = false;
	int smallest_down_distance = -1, smallest_left_distance = -1, smallest_right_distance = -1;
	while (MapObjs_myIter != MapObjs_myEnd) {
		//1) Test for enemy falling.
		int down_distance = isFallingEnemyCollidingDownMapObject(EnemyPtr, (*MapObjs_myIter));
		if (down_distance != -1) {
			is_colliding_down = true;
			if (smallest_down_distance == -1) {
				smallest_down_distance = down_distance;
			}
			else {
				if (down_distance < smallest_down_distance) {
					smallest_down_distance = down_distance;
				}
			}
		}

		//2) Test for enemy colliding left.
		if (EnemyPtr->isFacingLeft) {
			int left_distance = isWalkingEnemyCollidingLeftMapObject(EnemyPtr, (*MapObjs_myIter));
			if (left_distance != -1) {
				is_colliding_left = true;
				if (smallest_left_distance == -1) {
					smallest_left_distance = left_distance;
				}
				else {
					if (left_distance < smallest_left_distance) {
						smallest_left_distance = left_distance;
					}
				}
			}
		}

		//3) Test for enemy colliding right.
		if (!EnemyPtr->isFacingLeft) {
			int right_distance = isWalkingEnemyCollidingRightMapObject(EnemyPtr, (*MapObjs_myIter));
			if (right_distance != -1) {
				is_colliding_right = true;
				if (smallest_right_distance == -1) {
					smallest_right_distance = right_distance;
				}
				else {
					if (right_distance < smallest_right_distance) {
						smallest_right_distance = right_distance;
					}
				}
			}
		}

		++MapObjs_myIter;
	}

	//Apply gravity effect.
	if (is_colliding_down) {
		EnemyPtr->EnemyGameCoordY += smallest_down_distance;
	}
	else {
		EnemyPtr->EnemyGameCoordY += 14;
	}

	//Apply moving left.
	if (EnemyPtr->isFacingLeft) {
		if (is_colliding_left) {
			EnemyPtr->EnemyGameCoordX -= smallest_left_distance;
			if (smallest_left_distance == 0) {
				EnemyPtr->isFacingLeft = false;
			}
		}
		else {
			EnemyPtr->EnemyGameCoordX -= 7;
		}
	}

	//Apply moving right.
	if (!EnemyPtr->isFacingLeft) {
		if (is_colliding_right) {
			EnemyPtr->EnemyGameCoordX += smallest_right_distance;
			if (smallest_right_distance == 0) {
				EnemyPtr->isFacingLeft = true;
			}
		}
		else {
			EnemyPtr->EnemyGameCoordX += 7;
		}
	}
}

void CollisionManager::doGatorCollisions(Enemy* EnemyPtr) {
	std::list<int>::iterator MapObjs_myIter = myMapManager->myActiveMapObjects.begin(), MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
	bool is_colliding_down = false, is_colliding_up = false;
	int smallest_down_distance = -1, smallest_up_distance = -1;

	while (MapObjs_myIter != MapObjs_myEnd) {
		//1) Test for enemy falling.
		if (!EnemyPtr->isFlyingUp) {
			int down_distance = isFallingEnemyCollidingDownMapObject(EnemyPtr, (*MapObjs_myIter));
			if (down_distance != -1) {
				is_colliding_down = true;
				if (smallest_down_distance == -1) {
					smallest_down_distance = down_distance;
				}
				else {
					if (down_distance < smallest_down_distance) {
						smallest_down_distance = down_distance;
					}
				}
			}
		}
		else if (EnemyPtr->isFlyingUp) {
			int up_distance = isFlyingEnemyCollidingUpMapObject(EnemyPtr, (*MapObjs_myIter));
			if (up_distance != -1) {
				is_colliding_down = true;
				if (smallest_up_distance == -1) {
					smallest_up_distance = up_distance;
				}
				else {
					if (up_distance < smallest_up_distance) {
						smallest_up_distance = up_distance;
					}
				}
			}
		}
		++MapObjs_myIter;
	}

	//Apply flying down.
	if (!EnemyPtr->isFlyingUp) {
		if (EnemyPtr->EnemyGameCoordY >= (EnemyPtr->StartPosY + 130)) {
			EnemyPtr->isFlyingUp = true;
		}
		else if (is_colliding_down) {
			EnemyPtr->EnemyGameCoordY += smallest_down_distance;
			EnemyPtr->isFlyingUp = true;
		}
		else {
			EnemyPtr->EnemyGameCoordY += 7;
		}
	}
	else {
		//Apply flying up.
		if (EnemyPtr->EnemyGameCoordY <= (EnemyPtr->StartPosY - 130)) {
			EnemyPtr->isFlyingUp = false;
		}
		else if (is_colliding_up) {
			EnemyPtr->EnemyGameCoordY -= smallest_up_distance;
			EnemyPtr->isFlyingUp = false;
		}
		else {
			EnemyPtr->EnemyGameCoordY -= 7;
		}
	}
}

/*
Player/Enemycollisions
*/
void CollisionManager::doPlayerEnemyCollisions(void) {
	std::list<Enemy>::iterator myIter = myEnemyManager->myEnemies.begin(), myEnd = myEnemyManager->myEnemies.end();
	bool is_player_colliding_down = false, is_player_colliding_not_down = false;
	while (myIter != myEnd) {
		if ((*myIter).AssetID != 6) {
			if (!(!myInputManager->inputFlag_Jumping && myPlayerManager->jump_counter > 0)) {
				int isGettingHitByEnemy = isWalkingPlayerCollidingEnemy(&(*myIter));
				if (isGettingHitByEnemy != -1) {
					myPlayerManager->isHitByEnemy = true;
				}
			}
			else if (myPlayerManager->isHitByEnemy == false) {
				int isSmooshingEnemy = isFallingPlayerCollidingDownEnemy(&(*myIter));
				if (isSmooshingEnemy != -1) {
					Enemy* myEnemy = &(*myIter);
					myEnemyManager->doAddEnemy(6, myEnemy->EnemyGameCoordX, myEnemy->EnemyGameCoordY);
					myEnemyManager->myEnemies.erase(myIter);
				}
			}
			else {
				myPlayerManager->isHitByEnemy = false;
			}
		}
		++myIter;
	}
}

inline int CollisionManager::isFallingPlayerCollidingDownEnemy(Enemy* EnemyPtr) {
	//1) Get the collision box of the player.
	SDL_Rect myPlayerCollisionBox = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0]->myRect_src;
	int PlayerEdge_Right = myPlayerManager->PlayerGameCoordX + myPlayerCollisionBox.w;
	int PlayerEdge_Bottom = myPlayerManager->PlayerGameCoordY + myPlayerCollisionBox.h;
	int PlayerEdge_Left = myPlayerManager->PlayerGameCoordX;
	int PlayerEdge_Top = myPlayerManager->PlayerGameCoordY + 12;
	int PlayerEdge_BottomAfterMovement = PlayerEdge_Bottom + (myPlayerManager->player_movement_increment * 2);

	//1) Get the collision box of the enemy.
	int myAssetID = EnemyPtr->AssetID;
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[0]->myRect_src;
	int EnemyEdge_Right = EnemyPtr->EnemyGameCoordX + myEnemyCollisionBox.w;
	int EnemyEdge_Bottom = EnemyPtr->EnemyGameCoordY + myEnemyCollisionBox.h;
	int EnemyEdge_Left = EnemyPtr->EnemyGameCoordX;
	int EnemyEdge_Top = EnemyPtr->EnemyGameCoordY;
	//int EnemyEdge_RightAfterMovement = EnemyEdge_Right + 7;

	//3)Determine if player, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(PlayerEdge_Top, EnemyEdge_Top) < std::min(PlayerEdge_BottomAfterMovement, EnemyEdge_Bottom)) {
		if (std::max(PlayerEdge_Left, EnemyEdge_Left) < std::min(PlayerEdge_Right, EnemyEdge_Right)) {
			return EnemyEdge_Top - PlayerEdge_Bottom;
		}
	}
	return -1;
}

inline int CollisionManager::isWalkingPlayerCollidingEnemy(Enemy* EnemyPtr) {
	//1) Get the collision box of the player.
	SDL_Rect myPlayerCollisionBox = myAssetFactory->myAnimatedAssets[0]->myStaticAssets[0]->myRect_src;
	int PlayerEdge_Right = myPlayerManager->PlayerGameCoordX + myPlayerCollisionBox.w;
	int PlayerEdge_Top = myPlayerManager->PlayerGameCoordY + 10;
	int PlayerEdge_Bottom = myPlayerManager->PlayerGameCoordY + myPlayerCollisionBox.h;
	int PlayerEdge_Left = myPlayerManager->PlayerGameCoordX + 10;

	//1) Get the collision box of the enemy.
	int myAssetID = EnemyPtr->AssetID;
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[myAssetID]->myStaticAssets[0]->myRect_src;
	int EnemyEdge_Right = EnemyPtr->EnemyGameCoordX + myEnemyCollisionBox.w;
	int EnemyEdge_Bottom = EnemyPtr->EnemyGameCoordY + myEnemyCollisionBox.h;
	int EnemyEdge_Left = EnemyPtr->EnemyGameCoordX;
	int EnemyEdge_Top = EnemyPtr->EnemyGameCoordY;
	//int EnemyEdge_RightAfterMovement = EnemyEdge_Right + 7;

	//3)Determine if player, after movement, will collide with object. If it will, return distance between the two.
	if (std::max(PlayerEdge_Top, EnemyEdge_Top) < std::min(PlayerEdge_Bottom, EnemyEdge_Bottom)) {
		if (std::max(PlayerEdge_Left, EnemyEdge_Left) < std::min(PlayerEdge_Right, EnemyEdge_Right)) {
			return EnemyEdge_Left - PlayerEdge_Right;
		}
	}
	return -1;
}