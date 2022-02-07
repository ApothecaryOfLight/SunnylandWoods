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
	SDL_Rect myPlayerDrawnCollisionBox = myStaticAssetPtr->myRect_src;
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
		SDL_Rect myCollisionBox = myAssetFactory->myStaticAssets[ MapObjectAssetID ]->myRect_src;
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
	doEnemyCollisions();
	doPlayerEnemyCollisions();
	//5) doCollectiblesLogicTick();
	//6) doClickablesLogicTick();
	//7) doInteractablesLogicTick();
}

/*
Determines if there is a collision for the player walking left.
Returns -1 if there is no collision.
Otherwise, returns the distance between the player and the colliding object.
*/
inline int CollisionManager::isColliding(CollisionBox ObjectA, CollisionBox ObjectB, vector2d ObjectAMovement) {
/*	if (std::max(ObjectA.y, ObjectB.y) < std::min(ObjectA.y + ObjectA.h, ObjectB.y + ObjectB.h)) {
		if (std::max(PlayerEdge_LeftAfterMovement, MapObjectEdge_Left) < std::min(PlayerEdge_Right, MapObjectEdge_Right)) {
			return PlayerEdge_Left - MapObjectEdge_Right;
		}
	}*/
	return -1;
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
	int PlayerEdge_Bottom = myPlayerManager->PlayerGameCoordY + myPlayerCollisionBox.h;
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
	if( myInputManager->inputFlag_Jumping == true && myPlayerManager->jump_counter < max_jump_height) { //jumping logic
		myPlayerManager->jump_counter++;

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
				distance_left = isWalkingPlayerCollidingLeftMapObject((*MapObjs_myIter));
			}
			else if (myInputManager->inputFlag_Right && !myInputManager->inputFlag_Left) {
				distance_right = isWalkingPlayerCollidingRightMapObject((*MapObjs_myIter));
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

		if (is_colliding_up == false) {
			myPlayerManager->PlayerGameCoordY -= myPlayerManager->player_movement_increment;
			if (((myPlayerManager->PlayerGameCoordY*myInputManager->zoom) - myCameraManager->CameraY) <= myCameraManager->ScreenWall_Top) {
				myCameraManager->CameraY -= (myPlayerManager->player_movement_increment*myInputManager->zoom);
			}
		}
		else if (distance_remaining_top > 0) {
			myPlayerManager->PlayerGameCoordY -= distance_remaining_top;
		}



		if (myInputManager->inputFlag_Left && !myInputManager->inputFlag_Right) {
			MapObjs_myIter = myMapManager->myActiveMapObjects.begin();
			while (MapObjs_myIter != MapObjs_myEnd) {
				int distance_left = -1;
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
				if (((myPlayerManager->PlayerGameCoordX*myInputManager->zoom) - myCameraManager->CameraX) <= myCameraManager->ScreenWall_Left) {
					myCameraManager->CameraX -= myPlayerManager->player_movement_increment * myInputManager->zoom;
				}
			}
			else if (distance_remaining_left > 0) {
				myPlayerManager->PlayerGameCoordX -= distance_remaining_left;
			}
		}
		else if (myInputManager->inputFlag_Right && !myInputManager->inputFlag_Left) {
			MapObjs_myIter = myMapManager->myActiveMapObjects.begin();
			while (MapObjs_myIter != MapObjs_myEnd) {
				int distance_right = -1;
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
				if (((myPlayerManager->PlayerGameCoordX * myInputManager->zoom) - myCameraManager->CameraX) >= myCameraManager->ScreenWall_Right) {
					myCameraManager->CameraX += myPlayerManager->player_movement_increment * myInputManager->zoom;
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
			if (((myPlayerManager->PlayerGameCoordY * myInputManager->zoom) - myCameraManager->CameraY) >= myCameraManager->ScreenWall_Bottom) {
				myCameraManager->CameraY += (myPlayerManager->player_movement_increment*2) * myInputManager->zoom;
			}
		}
		else {
			myPlayerManager->jump_counter = 0;
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
				if (((myPlayerManager->PlayerGameCoordX * myInputManager->zoom) - myCameraManager->CameraX) <= myCameraManager->ScreenWall_Left) {
					myCameraManager->CameraX -= myPlayerManager->player_movement_increment * myInputManager->zoom;
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
				if (((myPlayerManager->PlayerGameCoordX * myInputManager->zoom) - myCameraManager->CameraX) >= myCameraManager->ScreenWall_Right) {
					myCameraManager->CameraX += myPlayerManager->player_movement_increment * myInputManager->zoom;
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
			if (((myPlayerManager->PlayerGameCoordX * myInputManager->zoom) - myCameraManager->CameraX) <= myCameraManager->ScreenWall_Left) {
				myCameraManager->CameraX -= myPlayerManager->player_movement_increment * myInputManager->zoom;
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
			if (((myPlayerManager->PlayerGameCoordX * myInputManager->zoom) - myCameraManager->CameraX) >= myCameraManager->ScreenWall_Right) {
				myCameraManager->CameraX += myPlayerManager->player_movement_increment * myInputManager->zoom;
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
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[6]->myStaticAssets[0]->myRect_src;
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

inline int CollisionManager::isWalkingEnemyCollidingLeftMapObject(Enemy* EnemyPtr, int MapObjectID) {
	//1) Get the collision box of the enemy.
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[6]->myStaticAssets[0]->myRect_src;
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
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[6]->myStaticAssets[0]->myRect_src;
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
		if ((*myEnemyIter).AssetID != 6) {
			std::list<int>::iterator MapObjs_myIter = myMapManager->myActiveMapObjects.begin(), MapObjs_myEnd = myMapManager->myActiveMapObjects.end();
			bool is_colliding_down = false, is_colliding_left = false, is_colliding_right = false;
			int smallest_down_distance = -1, smallest_left_distance = -1, smallest_right_distance = -1;
			while (MapObjs_myIter != MapObjs_myEnd) {
				//1) Test for enemy falling.
				int down_distance = isFallingEnemyCollidingDownMapObject(&(*myEnemyIter), (*MapObjs_myIter));
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
				if (myEnemyIter->isFacingLeft) {
					int left_distance = isWalkingEnemyCollidingLeftMapObject(&(*myEnemyIter), (*MapObjs_myIter));
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
				if (!myEnemyIter->isFacingLeft) {
					int right_distance = isWalkingEnemyCollidingRightMapObject(&(*myEnemyIter), (*MapObjs_myIter));
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
				(*myEnemyIter).EnemyGameCoordY += smallest_down_distance;
			}
			else {
				(*myEnemyIter).EnemyGameCoordY += 14;
			}

			//Apply moving left.
			if (myEnemyIter->isFacingLeft) {
				if (is_colliding_left) {
					(*myEnemyIter).EnemyGameCoordX -= smallest_left_distance;
					if (smallest_left_distance == 0) {
						myEnemyIter->isFacingLeft = false;
					}
				}
				else {
					(*myEnemyIter).EnemyGameCoordX -= 7;
				}
			}

			//Apply moving right.
			if (!myEnemyIter->isFacingLeft) {
				if (is_colliding_right) {
					(*myEnemyIter).EnemyGameCoordX += smallest_right_distance;
					if (smallest_right_distance == 0) {
						myEnemyIter->isFacingLeft = true;
					}
				}
				else {
					(*myEnemyIter).EnemyGameCoordX += 7;
				}
			}
		}
		++myEnemyIter;
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

	/*if (!myInputManager->inputFlag_Left && !myInputManager->inputFlag_Right) {
		if (myInputManager->isPlayerFacingLeft) {
			PlayerEdge_Right -= 10;
		}
		else {
			PlayerEdge_Left += 10;
		}
	}*/

	//1) Get the collision box of the enemy.
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[6]->myStaticAssets[0]->myRect_src;
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
	SDL_Rect myEnemyCollisionBox = myAssetFactory->myAnimatedAssets[6]->myStaticAssets[0]->myRect_src;
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