#pragma once
#include "Config.h"
#include "GraphManager.h"

class TextMenu {
public:
	TextMenu(int screen_w, int screen_h, int menu_w, const char* menu_message);

	std::string* waitEvent(SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer);

private:
	SDL_Rect* text_box_shape;

	std::string text_input;

	//The SDL_Rect for the menu
	SDL_Rect* menu_shape;

	//The SDL_Rect for the message
	SDL_Rect* message_shape;

	//The message to be displayed
	std::string message;

	//The width of content, determined by the padding and menu width
	int content_width;

};
