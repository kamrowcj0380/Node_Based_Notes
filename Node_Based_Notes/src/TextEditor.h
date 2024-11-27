/*
 * TextEditor is an object that is displayed when a node is selected. It stores the data from the file, allows
 * editing of the text within, and saves the data to the node's respective .txt file when closed.
 *
 * Author: Connor Kamrowski
 */
#pragma once
#include "Config.h"
#include "Utils.h"

#include "TextureManager.h"
#include "Node.h"


class TextEditor {
public:
	/**
	 * TextEditor constructor. This initializes a few private variables specific to this class given parameters.
	 *
	 * \param window_width: The width of the application window
	 * \param window_height: The height of the application window
	 * \param input_font: The font used to display text
	 */
	TextEditor(int window_width, int window_height, TTF_Font* input_font);

	/**
	 * TextEditor deconstructor. This frees a few textures stored by the TextEditor.
	 */
	~TextEditor();

	/**
	 * Open the text editor, loading the relevant data from a file and into private variable 'file_data'. 
	 * The file's path is stored in the target node.
	 *
	 * \param target: The node to load information for
	 */
	void open(Node* target);

	/**
	 * A respective function to open(). Save the text stored in private variable 'file_data' to the path stored in
	 * the target node.
	 *
	 * \param target: The node to load information for
	 */
	void close(Node* target);

	/**
	 * Display the graphics relevant to the text editor. This includes the editor's background, the header, the header's
	 * text, and the text stored in 'file_data'.
	 *
	 * \param renderer: The SDL_Renderer* being used
	 * \param target: The node to pull a title from
	 */
	void render(SDL_Renderer* renderer, Node* target);

	/**
	 * If a keyboard button was pressed, or a text input was created, edit the text stored in 'file_data' appropriately.
	 *
	 * \param event: The SDL_Event* that describes the keys pressed
	 */
	void handleKeyPress(SDL_Event* event);

	/**
	 * \return The SDL_Rect* that describes the x, y, width, and height of this text editor
	 */
	SDL_Rect* getShape() { return shape; }

private:
	//The text in the target node's file, to be edited by this text editor
	std::string file_data;

	//The x,y and shape of the text editor
	SDL_Rect* shape;

	//A pointer to the TTF_Font used by the text editor
	TTF_Font* font;

};
