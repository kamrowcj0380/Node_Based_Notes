/*
 * TextureManager is a collection of static functions for loading something into an SDL_Texture.
 * This should most likely be merged with Utils.h, I just haven't gotten around to that.
 *
 * Author: Connor Kamrowski
 */
#pragma once
#include "Config.h"

class TextureManager {
public:
	/**
	 * Display text using the given SDL_Renderer.
	 *
	 * \param renderer: The SDL_Renderer* being used
	 * \param font: The TTF_Font* being used
	 * \param message_texture: The destination for the texture created by this function
	 * \param text: The text to be loaded into a texture
	 * \param color: The color of the text
	 * \param x: The x coordinate for the top-left corner of the text
	 * \param y: The y coordinate for the top-left corner of the text
	 * \param font_size: The font size of the text
	 */
	static void loadText(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* message_texture, const char* message, const SDL_Color* color, int x, int y, int font_size);

	/**
	 * Display text using the given SDL_Renderer. This text will wrap to the next line if it exceeds container_width.
	 *
	 * \param renderer: The SDL_Renderer* being used
	 * \param font: The TTF_Font* being used
	 * \param message_texture: The destination for the texture created by this function
	 * \param text: The text to be loaded into a texture
	 * \param color: The color of the text
	 * \param x: The x coordinate for the top-left corner of the text
	 * \param y: The y coordinate for the top-left corner of the text
	 * \param container_width: The maximum width of the text before wrapping to a new line
	 * \param font_size: The font size of the text
	 */
	static void loadWrappedText(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* message_texture, const char* message, const SDL_Color* color, int x, int y, int container_width, int font_size);
};

