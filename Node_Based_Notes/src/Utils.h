#pragma once
/*
 * A group of miscellaneous utilities relevant to this project. Most of these functions are functionalities for SDL which
 * the library is missing. Many of these are overloads.
 * 
 * Author: Connor Kamrowski
 */

/** 
 * Change SDL_Renderer color using an SDL_Color instead of an array. This should be in the library, it's a common complaint...
 * 
 * \param renderer: The SDL_Renderer* being used
 * \param color: The SDL_Color* to change the drawing color to
 */
inline void CustomSDL_setRenderDrawColor(SDL_Renderer* renderer, const SDL_Color* color) { SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a); }

/**
 * Draw a rectangle in an SDL window using the renderer parameter. 
 * The library requires an SDL_Rect, this just takes the dimensions of an SDL_Rect as parameters.
 *
 * \param renderer: The SDL_Renderer* being used
 * \param x: The x coordinate for the top left corner of the rectangle
 * \param y: The y coordinate for the top left corner of the rectangle
 * \param w: The width of the rectangle
 * \param h: The height of the rectangle
 */
inline void Custom_SDLDrawRect(SDL_Renderer* renderer, int x, int y, int width, int height) {
	static SDL_Rect toDraw;
	toDraw = { x, y, width, height };
	SDL_RenderFillRect(renderer, &toDraw);
}

/**
 * An overload of the above Custom_SDLDrawRect. Any changes are noted below.
 * This differs by accepting a color for the rectangle. The renderer's draw color is preserved by this function.
 *
 * \param color: The color of the rectangle
 */
inline void Custom_SDLDrawRect(SDL_Renderer* renderer, const SDL_Color* color, int x, int y, int width, int height) {
	static Uint8 r;
	static Uint8 g;
	static Uint8 b;
	static Uint8 a;

	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

	CustomSDL_setRenderDrawColor(renderer, color);

	//Now, draw a square using the base function
	Custom_SDLDrawRect(renderer, x, y, width, height);

	//set the color back to previous color
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

/**
 * An overload of the above Custom_SDLDrawRect. Any changes are noted below.
 * This differs by accepting an SDL_Rect* instead of x,y and width, height.
 * It also accepts an SDL_Color* to change the renderer color when drawing the rectangle.
 *
 * \param rect: An object representing the x,y coordinates and the width, height desired for the rectangle
 */
inline void Custom_SDLDrawRect(SDL_Renderer* renderer, const SDL_Color* color, SDL_Rect* rect) {
	Custom_SDLDrawRect(renderer, color, rect->x, rect->y, rect->w, rect->h);
}

/**
 * An overload of the above Custom_SDLDrawRect. Any changes are noted below.
 * This differs by accepting an SDL_Rect* instead of x,y and width, height
 *
 * \param rect: An object representing the x,y coordinates and the width, height desired for the rectangle
 */
inline void Custom_SDLDrawRect(SDL_Renderer* renderer, SDL_Rect* rect) {
	Custom_SDLDrawRect(renderer, rect->x, rect->y, rect->w, rect->h);
}

/**
 * Using Custom_SDLDrawRect, draw a square, then a slightly smaller one. This creates a border of border_width 
 * and border_color that surrounds a rectangle of rect_color. The x/y/w/h is for the whole shape, meaning the
 * rectangle PLUS the border.
 *
 * \param renderer: The SDL_Renderer* being used
 * \param x: The x coordinate for the top left corner of the rectangle
 * \param y: The y coordinate for the top left corner of the rectangle
 * \param w: The width of the rectangle
 * \param h: The height of the rectangle
 * \param border_width: The width of the border
 * \param rect_color: The color of the inner rectangle
 * \param border_color: the color of the border
 */
inline void DrawRectWithBorder(SDL_Renderer* renderer, int x, int y, int w, int h, int border_width, const SDL_Color* rect_color, const SDL_Color* border_color) {
	Custom_SDLDrawRect(renderer, border_color, x, y, w, h);
	Custom_SDLDrawRect(renderer, rect_color, x + border_width, y + border_width, w - border_width * 2, h - border_width * 2);
}

/**
 * An overload of the above DrawRectWithBorder. Any changes are noted below.
 * This differs by accepting an SDL_Rect* instead of x,y and width, height
 *
 * \param rect: An object representing the x,y coordinates and the width, height desired for the rectangle
 */
inline void DrawRectWithBorder(SDL_Renderer* renderer, SDL_Rect* rect, int border_width, const SDL_Color* rect_color, const SDL_Color* border_color) {
	DrawRectWithBorder(renderer, rect->x, rect->y, rect->w, rect->h, border_width, rect_color, border_color);
}

/**
 * Given an x,y coordinate, determine if it is within the bounding box x, y and width, height given.
 *
 * \param x: the x position of the coordinate
 * \param y: the y position of the coordinate
 * \param box_x: the leftmost coordinate of the bounding box
 * \param box_y: the top coordinate of the bounding box
 * \param box_w: the width of the box
 * \param box_h: the height of the box
 * \return True if the coordinates are within the box
 */
inline bool isWithin(int x, int y, int box_x, int box_y, int box_w, int box_h) {
	return (x > box_x && y > box_y && x < box_x + box_w && y < box_y + box_h);

}
