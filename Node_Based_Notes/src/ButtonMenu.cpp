#include "ButtonMenu.h"

ButtonMenu::ButtonMenu(int screen_w, int screen_h, int menu_w, const char* menu_message, std::vector<std::string> button_messages, bool add_exit) {
	//If there is an exit button, add space for it
	int num_extra_buttons = 0;
	add_exit ? num_extra_buttons = 1 : num_extra_buttons = 0;

	//The total height the message will take up, including the padding above and below
	int message_total_height = MENU_MESSAGE_HEIGHT + MENU_CONTENT_PADDING * 2;
	int menu_total_height = message_total_height + (button_messages.size() + num_extra_buttons) * (MENU_BUTTON_HEIGHT + MENU_BUTTON_SPACING) + MENU_CONTENT_PADDING;

	//initialize the SDL_Rect*
	menu_shape = new SDL_Rect();
	message_shape = new SDL_Rect();
	button_shape = new SDL_Rect();

	//Set the menu dimensions
	menu_shape->w = menu_w;
	menu_shape->h = menu_total_height;
	menu_shape->x = screen_w / 2 - menu_w / 2;
	menu_shape->y = screen_h / 2 - menu_total_height / 2;

	//Set the message dimensions relative to the menu dimensions
	message_shape->x = menu_shape->x + MENU_CONTENT_PADDING;
	message_shape->y = menu_shape->y + MENU_CONTENT_PADDING;
	message_shape->h = MENU_MESSAGE_HEIGHT;
	message_shape->w = menu_shape->w - MENU_CONTENT_PADDING * 2;

	//Set the menu message
	message = menu_message;

	//set the width of the content
	content_width = menu_shape->w - MENU_CONTENT_PADDING * 2;

	//Set the button dimensions
	button_shape->x = menu_shape->x + MENU_CONTENT_PADDING;
	button_shape->y = menu_shape->y + message_total_height;
	button_shape->w = content_width;
	button_shape->h = MENU_BUTTON_HEIGHT;

	//save the buttons to this object
	for (int i = 0; i < button_messages.size(); i++) {
		buttons.push_back(button_messages.at(i));
	}

	if (add_exit) {
		//Add an "exit" button to the button menu
		buttons.push_back("Return");
	}

}

int ButtonMenu::waitEvent(SDL_Renderer* renderer) { 

	render(renderer);

	int result = -1;
	int mousex;
	int mousey;
	SDL_Event event;

	//while no result is attained
	while (result == -1) {
		//wait for an event to happen
		SDL_WaitEvent(&event);

		//if the mouse hasn't been released since the button menu was created, don't proceed
		if (!mouseWasUp) {
			//check if it is currently up
			if (!SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_LMASK){
				mouseWasUp = true;
			}
			continue;
		}

		//if the left mouse wasn't clicked, pass this iteration and wait again
		//AAAAATODO: add graphics here, hovering over button makes it darker
		if (event.type != SDL_MOUSEBUTTONDOWN && event.button.button != SDL_BUTTON_LEFT) {
			if (event.type == SDL_QUIT) {
				return -1;
			}

			continue;
		}
		else if (event.type == SDL_MOUSEMOTION) {
			continue;
		}

		//Get the mouse position
		SDL_GetMouseState(&mousex, &mousey);

		//reset the button_shape y
		button_shape->y = menu_shape->y + MENU_MESSAGE_HEIGHT + MENU_CONTENT_PADDING * 2;

		//for each button
		for (int i = 0; i < buttons.size(); i++) {

			//if the mouse coordinates are inside of the boundaries
			if (mousex > button_shape->x && mousex < button_shape->x + button_shape->w
				&& mousey > button_shape->y && mousey < button_shape->y + button_shape->h) {
				//clear the renderer
				SDL_RenderClear(renderer);
				//return the index
				return i;
			}

			//increment the button Y position
			button_shape->y += MENU_BUTTON_SPACING + MENU_BUTTON_HEIGHT;
		}
	}

	//Nothing was hovered over, return -1. This will never be reached thanks to the while loop, but it's required
	return -1;
}

void ButtonMenu::render(SDL_Renderer* renderer) {

	//render the base of the menu
	Custom_SDLDrawRect(renderer, &MENU_BACKGROUND_COLOR, menu_shape);

	//draw the background for the message
	Custom_SDLDrawRect(renderer, &WHITE, message_shape);

	//render the message
	TextureManager::loadWrappedText(renderer, GraphManager::font, message.c_str(), &BLACK, menu_shape->x + MENU_CONTENT_PADDING, menu_shape->y + MENU_CONTENT_PADDING, content_width, MENU_MESSAGE_FONT_SIZE, true);

	//render each of the buttons
	for (int i = 0; i < buttons.size(); i++) {
		//draw the background for the button
		Custom_SDLDrawRect(renderer, &WHITE, button_shape);

		//then the text over top
		TextureManager::loadWrappedText(renderer, GraphManager::font, buttons.at(i).c_str(), &BLACK, button_shape->x, button_shape->y, content_width, MENU_BUTTON_FONT_SIZE, true);

		//and increment the button height
		button_shape->y += MENU_BUTTON_SPACING + MENU_BUTTON_HEIGHT;
	}

	SDL_RenderPresent(renderer);

}