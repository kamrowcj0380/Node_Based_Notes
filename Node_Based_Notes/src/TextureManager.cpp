#include "TextureManager.h"

//Display text in SDL using the given renderer.
void TextureManager::loadText(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* message_texture, const char* text, const SDL_Color* color, int x, int y, int font_size) {
	SDL_Surface* temp_surface;
	SDL_Rect temp;

	//set the font to the desired size
	TTF_SetFontSize(font, font_size);

	//create a surface using the text input
	temp_surface = TTF_RenderText_Blended(font, text, *color);

	//set the dimensions of the temporary rect to match the surface, which scales in width based on the text input
	temp.w = temp_surface->w;
	temp.h = temp_surface->h;
	temp.x = x;
	temp.y = y;

	//create the texture, storing it in the texture pointer parameter
	message_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);

	//free the temporary surface from memory
	SDL_FreeSurface(temp_surface);

	//display the texture to the renderer (so that it appears when the renderer is presented)
	SDL_RenderCopy(renderer, message_texture, NULL, &temp);
}

//Display text in SDL using the given renderer. The text will wrap around if it exceeds the container_width.
void TextureManager::loadWrappedText(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* message_texture, const char* text, const SDL_Color* color, int x, int y, int container_width, int font_size) {
	SDL_Surface* temp_surface;
	SDL_Rect temp;

	//set the font to the desired size
	TTF_SetFontSize(font, font_size);

	//create a surface using the text input
	temp_surface = TTF_RenderText_Blended_Wrapped(font, text, *color, container_width);

	//set the dimensions of the temporary rect to match the surface, which scales in width and height based on the text input
	temp.w = temp_surface->w;
	temp.h = temp_surface->h;
	temp.x = x;
	temp.y = y;

	//create the texture, storing it in the texture pointer parameter
	message_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);

	//free the temporary surface from memory
	SDL_FreeSurface(temp_surface);

	//display the texture to the renderer (so that it appears when the renderer is presented)
	SDL_RenderCopy(renderer, message_texture, NULL, &temp);
}
