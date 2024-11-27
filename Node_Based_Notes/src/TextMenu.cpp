#include "TextMenu.h"

TextMenu::TextMenu(int screen_w, int screen_h, int menu_w, const char* menu_message) {
	//The total height the message will take up, including the padding above and below
	int message_total_height = MENU_MESSAGE_HEIGHT + MENU_CONTENT_PADDING * 2;
	int menu_total_height = message_total_height + MENU_TEXT_INPUT_HEIGHT + MENU_CONTENT_PADDING;

	//initialize the SDL_Rect*
	menu_shape = new SDL_Rect();
	message_shape = new SDL_Rect();
	text_box_shape = new SDL_Rect();

	//Set the menu dimensions
	menu_shape->w = menu_w;
	menu_shape->h = menu_total_height;
	menu_shape->x = screen_w / 2 - menu_w / 2;
	menu_shape->y = screen_h / 2 - menu_total_height / 2;

	//set the width of the content
	content_width = menu_shape->w - MENU_CONTENT_PADDING * 2;

	//Set the message dimensions relative to the menu dimensions
	message_shape->x = menu_shape->x + MENU_CONTENT_PADDING;
	message_shape->y = menu_shape->y + MENU_CONTENT_PADDING;
	message_shape->h = MENU_MESSAGE_HEIGHT;
	message_shape->w = content_width;

	//Set the text box dimensions
	text_box_shape->x = message_shape->x;
	text_box_shape->y = menu_shape->y + message_total_height;
	text_box_shape->w = content_width;
	text_box_shape->h = MENU_TEXT_INPUT_HEIGHT;

	//Set the menu message
	message = menu_message;

	text_input = "|";

}

std::string* TextMenu::waitEvent(SDL_Renderer* renderer) {

	render(renderer);

	SDL_Event event;

	//while no result is attained
	while (true) {
		//wait for an event to happen
		SDL_WaitEvent(&event);

		//Handle the key press

		//TODO: This code is used multiple times. Clean it up, and make it a function that both can access. AND FIX THE CURSOR!!!
		if (event.type == SDL_QUIT) {
			return nullptr;
		}
		else if (event.type == SDL_KEYDOWN && text_input.length() > 1) {
			if (event.key.keysym.sym == SDLK_BACKSPACE) {
				//Delete a character
				text_input.pop_back();
				text_input.pop_back();
				text_input += "|";
			}
			else if (event.key.keysym.sym == SDLK_RETURN) {
				//Finish with the text menu and return
				SDL_RenderClear(renderer);

				text_input.pop_back();
				return &text_input;
			}
		}
		else if (event.type == SDL_TEXTINPUT) {
			text_input.pop_back();
			text_input += event.text.text;
			text_input += "|";
		}
		else {
			continue;
		}

		render(renderer);

	}

	//This should never happen
	return nullptr;
}

void TextMenu::render(SDL_Renderer* renderer) {
	//render the base of the menu
	Custom_SDLDrawRect(renderer, &MENU_BACKGROUND_COLOR, menu_shape);

	//draw the background for the message
	Custom_SDLDrawRect(renderer, &WHITE, message_shape);

	//render the message
	TextureManager::loadWrappedText(renderer, GraphManager::font, message.c_str(), &BLACK, menu_shape->x + MENU_CONTENT_PADDING, menu_shape->y + MENU_CONTENT_PADDING, content_width, MENU_MESSAGE_FONT_SIZE);

	//render the text box
	Custom_SDLDrawRect(renderer, &WHITE, text_box_shape);

	//render the text in the text box
	TextureManager::loadText(renderer, GraphManager::font, text_input.c_str(), &BLACK, text_box_shape->x, text_box_shape->y, MENU_TEXT_INPUT_FONT_SIZE);

	SDL_RenderPresent(renderer);

}