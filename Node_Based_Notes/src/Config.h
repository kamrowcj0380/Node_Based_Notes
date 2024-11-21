/*
 * A config header file which includes constants used through this project. No constants are defined in the private
 * variables of a class, so any desired cosmetic modifications can be made using this file.
 * 
 * This also includes any common #includes, such as vector and string. The general rule of thumb is that some files will
 * have unique #includes, but if more than one has it I tried to locate those here instead. Every file in this project
 * should include "Config.h", unless there's no utility to be gained.
 *
 * Author: Connor Kamrowski
 */

#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#include "vector"
#include "string"
#include <fstream>
#include <iostream>
#include <chrono>
#include <thread>

//A made up version I attributed to this code.
const std::string VERSION = "0.1";

/*** Global / Unaffiliated ***/
//The width of a border created by DrawRectWithBorder in Utils.h
const int BORDER_WIDTH = 4;

//A basic full white color using SDL_Color
const SDL_Color WHITE = { 255, 255, 255, 255 };

//A basic full black color using SDL_Color. Often used for rendering text black
const SDL_Color BLACK = { 0, 0, 0, 255 };

/*** GraphManager ***/
//The nominator and denominator for the window width's scaling. Screen width is multiplied by nominator, 
// divided by denominator, and the result is the width of the application window. If nom=2 and denom=3,
// the width of the application window will be 2/3 of the user's screen width.
const int WINDOW_WIDTH_NOM = 2;
const int WINDOW_WIDTH_DENOM = 3;

//The window height nominator and denominator, reflecting the same concept as the constants above this.
const int WINDOW_HEIGHT_NOM = 2;
const int WINDOW_HEIGHT_DENOM = 3;

//The background color of the graph manager
const SDL_Color GRAPH_BACKGROUND_COLOR = { 200, 200, 200, 255 };

/*** TextEditor ***/
//The screen width is divided by this integer to determine the width of the text editor
const int TEXT_EDITOR_WIDTH_FACTOR = 3;

//The height of the header in the text editor
const int HEADER_HEIGHT = 40;

//The font size of the header in the text editor
const int HEADER_FONT_SIZE = 28;

//The font size of text file content in the text editor
const int CONTENT_FONT_SIZE = 20;

//A buffer placed to the left of rendered text (to provide padding from the edge of the screen)
const int LEFT_TEXT_BUFFER = 5;

//The background color of the header in the text editor
const SDL_Color HEADER_BACKGROUND_COLOR = { 200, 150, 200, 255 };

/*** Node ***/
//The base color of a node, when not selected or hovered over
const SDL_Color NODE_BASE_COLOR = { 0, 0, 0, 255 };

//The color of a node if it is hovered over (and not selected)
const SDL_Color NODE_HOVER_COLOR = { 40, 10, 40, 255 };

//The color of a node if it is selected
const SDL_Color NODE_SELECTED_COLOR = { 100, 10, 100, 255 };

//The number of pixels a node will grow if hovered over or while selected
const int NODE_SIZE_INCREASE = 8;

//The base width of a node (when not hovered/selected)
const int NODE_WIDTH = 25; //Note: node width and height are same, they are squares.

//The path that all graphs will be stored in this program
const std::string GRAPH_PATH = "Graphs/";

/*** PopupMenu and Children ***/
//The basic color of menus
const SDL_Color MENU_BACKGROUND_COLOR = NODE_SELECTED_COLOR;

//The left and right padding for buttons in ButtonMenu
const int MENU_CONTENT_PADDING = 20;

//The maximum message height
const int MENU_MESSAGE_HEIGHT = 200;

//The size of the message in any menu
const int MENU_MESSAGE_FONT_SIZE = 28;

//The inverse of the ratio of menu width to the screen size. If denom=3, then menus will be 1/3 the screen width.
const int MENU_WIDTH_DENOM = 2;

//The font size of buttons in ButtonMenu
const int MENU_BUTTON_FONT_SIZE = 24;

//The height of buttons in ButtonMenu
const int MENU_BUTTON_HEIGHT = MENU_BUTTON_FONT_SIZE + 10;

//The vertical space between buttons in ButtonMenu
const int MENU_BUTTON_SPACING = 10;

//The vertical space between buttons in TextMenu
const int MENU_TEXT_INPUT_FONT_SIZE = 20;

//The vertical space between buttons in TextMenu
const int MENU_TEXT_INPUT_HEIGHT = MENU_TEXT_INPUT_FONT_SIZE + 10;

