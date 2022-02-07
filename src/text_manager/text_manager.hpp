#ifndef _TEXT_MANAGER_
#define _TEXT_MANAGER_

#include "../logger/logger.hpp"
#include "../camera_manager/CameraManager.hpp"

#include <SDL.h>
#include <string>
#include <array>

class screen_text_entity {
public:
	screen_text_entity(std::string in_name, int* in_ptr_to_int);
	screen_text_entity(std::string in_name, float* in_ptr_to_float);
	screen_text_entity(void);
	std::string name;
	int* ptr_to_int;
	float* ptr_to_float;
};

class screen_text_entities {
public:
	screen_text_entities();
	void add_screen_text_entity(std::string in_name, int* in_ptr_to_int);
	void add_screen_text_entity(std::string in_name, float* in_ptr_to_float);
	std::array<screen_text_entity,6> entities;
	int length;
};

class TextManager {
public:
	TextManager(SDL_Renderer* inRen, Logger* inLogger, CameraManager* inCameraManager);
	void load_text_atlas(void);
	void draw_text(int posX, int posY, std::string text);
	void draw_number(int posX, int posY, int int_value);
	void draw_number(int posX, int posY, float float_value);

	int get_letter_source_position(char& character);
	int get_digit_source_position(char& digit);

	void add_text_entity(std::string name, int* in_ptr_to_int, int screen_corner);
	void add_text_entity(std::string name, float* in_ptr_to_float, int screen_corner);
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