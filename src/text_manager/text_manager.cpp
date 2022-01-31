#include "text_manager.hpp"
#include "../logger/logger.hpp"
#include "../camera_manager/CameraManager.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <sstream>

screen_text_entity::screen_text_entity(void) {
	ptr_to_value = nullptr;
}

screen_text_entity::screen_text_entity(std::string in_name, int* in_ptr_to_value) {
	name.assign(in_name);
	ptr_to_value = in_ptr_to_value;
}

screen_text_entities::screen_text_entities() {
	length = 0;
}

void screen_text_entities::add_screen_text_entity(std::string in_name, int* in_ptr_to_value) {
	entities[length] = screen_text_entity(in_name, in_ptr_to_value);
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

void TextManager::add_text_entity(std::string in_name, int* in_ptr_to_value, int screen_corner) {
	screen_text_entities_container[screen_corner].add_screen_text_entity(in_name, in_ptr_to_value);
}

void TextManager::draw_text_entities() {
	for (int corner = 0; corner < 4; corner++) {
		for (int text_entity = 0; text_entity < screen_text_entities_container[corner].length; text_entity++) {
			draw_text_entity(&screen_text_entities_container[corner].entities[text_entity],corner,text_entity);
		}
	}
}

void TextManager::draw_text_entity(screen_text_entity* ptr_to_screen_text_entity, int corner_pos, int stack_pos) {
	int text_x_position, text_y_position, int_x_position, int_y_position;
	int text_length = ptr_to_screen_text_entity->name.length()*20;
	int integer = *(ptr_to_screen_text_entity->ptr_to_value);
	std::string text_integer = std::to_string(integer);
	int integer_length = text_integer.length()*20;

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

	draw_number(int_x_position, int_y_position, integer);
	draw_text(text_x_position, text_y_position, ptr_to_screen_text_entity->name);
}