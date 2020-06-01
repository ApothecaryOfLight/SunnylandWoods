//InputManager.hpp

#ifndef _INPUT_MANAGER_
#define _INPUT_MANAGER_

#include <SDL2/SDL.h>

class InputManager {
public:
	InputManager ( void );
	void doProcessInput ( SDL_Event * inEvent );
	bool isQuit;
	bool isPressed_LeftArrowKey, isPressed_RightArrowKey, isPressed_SpaceBar, isPressed_DownArrowKey;
	bool isChangeAnimationAlert;

	bool inputFlag_Jumping, inputFlag_Crouching, inputFlag_Left, inputFlag_Right;

	bool isPlayerFacingLeft;

	bool isResized;
	int newWidth, newHeight;
private:
};

#endif


