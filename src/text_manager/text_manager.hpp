#ifndef _TEXT_MANAGER_
#define _TEXT_MANAGER_

#include "../logger/logger.hpp"
#include "../camera_manager/CameraManager.hpp"

#include <SDL.h>
#include <string>
#include <array>

class screen_text_entity {
public:
	screen_text_entity(std::string in_name, int* in_ptr_to_value);
	screen_text_entity(void);
	std::string name;
	int* ptr_to_value;
};

class screen_text_entities {
public:
	screen_text_entities();
	void add_screen_text_entity(std::string in_name, int* in_ptr_to_value);
	std::array<screen_text_entity,6> entities;
	int length;
};

class TextManager {
public:
	TextManager(SDL_Renderer* inRen, Logger* inLogger, CameraManager* inCameraManager);
	void load_text_atlas(void);
	void draw_text(int posX, int posY, std::string text);
	void draw_number(int posX, int posY, int value);

	void draw_player_position();
	void draw_player_screen_position();
	void draw_mouse_coords();

	int get_letter_source_position(char& character);
	int get_digit_source_position(char& digit);

	void add_text_entity(std::string name, int* in_ptr_to_value, int screen_corner);
	void draw_text_entities();
	void draw_text_entity( screen_text_entity* ptr_to_screen_text_entity, int corner_pos, int stack_pos );
private:
	SDL_Renderer* myRen;
	Logger* myLogger;
	CameraManager* myCameraManager;

	SDL_Texture* myTextAtlas;
	SDL_Texture* text_to_draw;

	std::array<screen_text_entities, 4> screen_text_entities_container;
};

#endif