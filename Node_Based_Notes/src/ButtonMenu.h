#pragma once
#include "Config.h"
#include "GraphManager.h"

class ButtonMenu {
public:
	ButtonMenu(int screen_w, int screen_h, int menu_w, const char* menu_message, std::vector<std::string> button_messages, bool add_exit = true);

	//As things are now, no deconstructor
	~ButtonMenu() {}

	int waitEvent(SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer);

private:
	std::vector<std::string> buttons;
	SDL_Rect* button_shape;

	//The SDL_Rect for the menu
	SDL_Rect* menu_shape;

	//The SDL_Rect for the message
	SDL_Rect* message_shape;

	//The message to be displayed
	std::string message;

	//The width of content, determined by the padding and menu width
	int content_width;

	//Indicates if the mouse button has been released. Stops clicks from previous buttons from impacting this one
	bool mouseWasUp = false;

};
