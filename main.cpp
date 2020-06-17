#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <sstream>

#include "src/time_manager/TimeManager.hpp"
#include "src/interface_manager/InterfaceManager.hpp"

#define screenWIDTH 960
#define screenHEIGHT 624

const int SCREEN_FPS = 24;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

int main ( int argc, char *argv[] ) {
	//TODO: Have a proper usage invokation respond to command line arguemnts by saying just run it.
	if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Window *win = SDL_CreateWindow(  "Sunnyland Woods", 100, 100, screenWIDTH, screenHEIGHT, SDL_WINDOW_SHOWN );
	if (win == NULL){
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_Renderer *ren = SDL_CreateRenderer( win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
	if (ren == NULL){
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	SDL_SetWindowResizable( win, SDL_TRUE ); 
	SDL_Event mySDLEvent;

	TimeManager myTimeManager;
	std::stringstream timeText;

	InterfaceManager myInterfaceManager ( ren );

	while ( !myInterfaceManager.isQuit() ) {
                myTimeManager.doStart();

				myInterfaceManager.doProcessInput( &mySDLEvent );
				myInterfaceManager.doGameLogic();
				myInterfaceManager.doRenderFrame();

                int frameTicks = myTimeManager.getTicks();
                if( frameTicks < SCREEN_TICKS_PER_FRAME ) {
                    SDL_Delay( SCREEN_TICKS_PER_FRAME - frameTicks );
                }
	}

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );
	SDL_Quit();

	return 0;
}
