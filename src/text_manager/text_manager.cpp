#include "text_manager.hpp"
#include "../logger/logger.hpp"
#include "../camera_manager/CameraManager.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <sstream>

screen_text_entity::screen_text_entity(void) {
	ptr_to_int = nullptr;
	ptr_to_float = nullptr;
}

screen_text_entity::screen_text_entity(std::string in_name, int* in_ptr_to_int) {
	name.assign(in_name);
	ptr_to_float = nullptr;
	ptr_to_int = in_ptr_to_int;
}

screen_text_entity::screen_text_entity(std::string in_name, float* in_ptr_to_float) {
	name.assign(in_name);
	ptr_to_int = nullptr;
	ptr_to_float = in_ptr_to_float;
}

screen_text_entities::screen_text_entities() {
	length = 0;
}

void screen_text_entities::add_screen_text_entity(std::string in_name, int* in_ptr_to_int) {
	entities[length] = screen_text_entity(in_name, in_ptr_to_int);
	length++;
}

void screen_text_entities::add_screen_text_entity(std::string in_name, float* in_ptr_to_float) {
	entities[length] = screen_text_entity(in_name, in_ptr_to_float);
	length++;
}

TextManager::TextManager(SDL_Renderer* inRen, Logger* inLogger, CameraManager* inCameraManager) {
	myRen = inRen;
	myLogger = inLogger;
	myCameraManager = inCameraManager;

	load_text_atlas();
}

void TextManager::load_text_atlas(void) {
	std::stringstream myStringstreamConv;
	myStringstreamConv << "media/text_atlas.png";
	std::string myConv = myStringstreamConv.str();

	myLogger->log(std::string("Loading " + myConv ));
	SDL_Surface* myTextAtlasSurface;
	myTextAtlasSurface = IMG_Load(myConv.c_str());
	myTextAtlas = SDL_CreateTextureFromSurface(myRen, myTextAtlasSurface);
	SDL_FreeSurface(myTextAtlasSurface);
}

int TextManager::get_letter_source_position(char& character) {
	//Lower case
	if (character >= 65 && character <= 90) {
		return (character - 65) * 20;
	} //Upper case
	else if (character >= 97 && character <= 122) {
		return (character - 97) * 20;
	}
}

void TextManager::draw_text(int posX, int posY, std::string text) {
	for (int i = 0; i<text.length(); i++) {
		int letter_source_pos = get_letter_source_position(text[i]);

		SDL_Rect Letter_Source;
		Letter_Source.y = 0;
		Letter_Source.h = 20;
		Letter_Source.w = 20;
		Letter_Source.x = letter_source_pos;

		SDL_Rect Letter_Dest;
		Letter_Dest.w = 20;
		Letter_Dest.h = 20;
		Letter_Dest.x = posX + (i * 20);
		Letter_Dest.y = posY;

		SDL_RenderCopy(
			myRen,
			myTextAtlas,
			&Letter_Source,
			&Letter_Dest
		);
	}
}

int TextManager::get_digit_source_position(char& digit) {
	if (digit == '0') {
		return 0;
	} else if (digit == '1') {
		return 20;
	} else if (digit == '2') {
		return 40;
	} else if (digit == '3') {
		return 60;
	} else if (digit == '4') {
		return 80;
	} else if (digit == '5') {
		return 100;
	} else if (digit == '6') {
		return 120;
	} else if (digit == '7') {
		return 140;
	} else if (digit == '8') {
		return 160;
	} else if (digit == '9') {
		return 180;
	} else if ((int)digit == 45) {
		return 200;
	} else if ((int)digit == 46) {
		return 220;
	}
}

void TextManager::draw_number(int posX, int posY, int value) {
	std::string text = std::to_string(value);
	for (int i = 0; i < text.length(); i++) {
		int digit_source_pos = get_digit_source_position(text[i]);

		SDL_Rect Digit_Source;
		Digit_Source.y = 20;
		Digit_Source.h = 20;
		Digit_Source.w = 20;
		Digit_Source.x = digit_source_pos;

		SDL_Rect Digit_Dest;
		Digit_Dest.w = 20;
		Digit_Dest.h = 20;
		Digit_Dest.x = posX + (i * 20);
		Digit_Dest.y = posY;

		SDL_RenderCopy(
			myRen,
			myTextAtlas,
			&Digit_Source,
			&Digit_Dest
		);
	}
}

void TextManager::draw_number(int posX, int posY, float value) {
	std::string text = std::to_string(value);
	for (int i = 0; i < 4; i++) {
		int digit_source_pos = get_digit_source_position(text[i]);

		SDL_Rect Digit_Source;
		Digit_Source.y = 20;
		Digit_Source.h = 20;
		Digit_Source.w = 20;
		Digit_Source.x = digit_source_pos;

		SDL_Rect Digit_Dest;
		Digit_Dest.w = 20;
		Digit_Dest.h = 20;
		Digit_Dest.x = posX + (i * 20);
		Digit_Dest.y = posY;

		SDL_RenderCopy(
			myRen,
			myTextAtlas,
			&Digit_Source,
			&Digit_Dest
		);
	}
}

void TextManager::add_text_entity(std::string in_name, int* in_ptr_to_value, int screen_corner) {
	screen_text_entities_container[screen_corner].add_screen_text_entity(in_name, in_ptr_to_value);
}

void TextManager::add_text_entity(std::string in_name, float* in_ptr_to_float, int screen_corner) {
	screen_text_entities_container[screen_corner].add_screen_text_entity(in_name, in_ptr_to_float);
}

void TextManager::draw_text_entities() {
	for (int corner = 0; corner <= 3; corner++) {
		for (int text_entity = 0; text_entity < screen_text_entities_container[corner].length; text_entity++) {
			draw_text_entity(&screen_text_entities_container[corner].entities[text_entity],corner,text_entity);
		}
	}
}

void TextManager::draw_text_entity(screen_text_entity* ptr_to_screen_text_entity, int corner_pos, int stack_pos) {
	int text_x_position, text_y_position, int_x_position, int_y_position;
	int text_length = ptr_to_screen_text_entity->name.length()*20;
	std::string text_value;
	int integer_length;
	if (ptr_to_screen_text_entity->ptr_to_int != nullptr) {
		int myInteger = *(ptr_to_screen_text_entity->ptr_to_int);
		text_value = std::to_string(myInteger);
		integer_length = text_value.length()*20;
	}
	else if (ptr_to_screen_text_entity->ptr_to_float != nullptr) {
		float myFloat = *(ptr_to_screen_text_entity->ptr_to_float);
		text_value = std::to_string(myFloat);
		integer_length = 4 * 20;
	}

	if (corner_pos % 2 == 0) {
		text_x_position = 0;
		int_x_position = text_length+20;
	}
	else {
		int_x_position = myCameraManager->ScreenWidth - integer_length;
		text_x_position = int_x_position - (text_length+20);
	}

	if (corner_pos < 2) {
		text_y_position = stack_pos * 20;
		int_y_position = text_y_position;
	}
	else {
		text_y_position = myCameraManager->ScreenHeight - ((stack_pos+1) * 20);
		int_y_position = text_y_position;
	}

	if (ptr_to_screen_text_entity->ptr_to_int != nullptr) {
		int myInteger = *(ptr_to_screen_text_entity->ptr_to_int);
		draw_number(int_x_position, int_y_position, myInteger);
	}
	else if (ptr_to_screen_text_entity->ptr_to_float != nullptr) {
		float myFloat = *(ptr_to_screen_text_entity->ptr_to_float);
		draw_number(int_x_position, int_y_position, myFloat);
	}
	draw_text(text_x_position, text_y_position, ptr_to_screen_text_entity->name);
}