//Author: Connor Kamrowski. See header file for more information.
#include "TextEditor.h"

/**
 * The TextEditor constructor. This initializes the position and shape of the text editor, and stores
 * the font passed from the graph manager.
 */
TextEditor::TextEditor(int window_width, int window_height, TTF_Font* input_font) {
	//Initialize the shape
	shape = new SDL_Rect();

	//Set the position to 0,0 and determine the shape of the text editor
	shape->x = shape->y = 0;
	shape->w = window_width / TEXT_EDITOR_WIDTH_FACTOR;
	shape->h = window_height;

	//store the font to be used for editing text
	font = input_font;

}//END OF TextEditor()


/**
 * The TextEditor deconstructor. This simply destroys the textures stored by the editor.
 */
TextEditor::~TextEditor() {}

/**
 * Load a new Node into the text editor. This reads the file data at the file path stored by Node* target, and
 * stores it in local memory for easy access.
 */
void TextEditor::open(Node* target) {
	std::ifstream node_file;
	std::string line;

	//Open the file that target represents
	node_file.open(target->getFName());

	//If node_file opened successfully
	if (node_file.is_open()) {
		//While there are lines left in node_file
		while (node_file.good()) {

			//Copy the next line of text
			std::getline(node_file, line);

			//Quit if the line is empty
			if (line.empty()) { break; }

			//Append the line to file_data
			file_data += line;
		}

		//Add a placeholder "cursor" for the text editor
		file_data += "|";
	}
	else {
		std::cout << "Couldn't open file" << std::endl;

		//This doesn't make good use of C++ error handling, but just toss it out the window if this happens...
		exit(0);
	}

	node_file.close();

}//END OF open()

/**
 * Close the text editor. This takes the Node* target, finds the file it represents, and dumps the file_data back
 * into it to save any changes made while it was open.
 */
void TextEditor::close(Node* target) {
	//Get the file path from target
	std::string outfile_path = target->getFName();

	//Remove the cursor from the block of text
	file_data.pop_back();

	//Open an output file stream using trunc, which writes over the previously stored information
	std::ofstream outfile(outfile_path, std::ofstream::trunc);

	//If it opened successfully, dump the data
	if (outfile.good()) {
		outfile << file_data;
	}
	else {
		std::cout << "ERROR: The Node contents could not be saved upon exiting." << std::endl;
	}

	outfile.close();

	//Reset file data to empty
	file_data = "";

}//END OF close()

/**
 * Render the text editor. This includes the header, and all of file_data.
 */
void TextEditor::render(SDL_Renderer* renderer, Node* target) {

	//Draw the text editor background
	DrawRectWithBorder(renderer, shape, BORDER_WIDTH, &WHITE, &BLACK);

	//Draw the text editor header
	DrawRectWithBorder(renderer, 0, 0, shape->w, HEADER_HEIGHT, BORDER_WIDTH, &HEADER_BACKGROUND_COLOR, &BLACK);

	//Render the header text
	//AAAAATODO: Handle text overflow 
	TextureManager::loadText(renderer, font, target->getTitle().c_str(), &BLACK, LEFT_TEXT_BUFFER, 0, HEADER_FONT_SIZE);

	//Render file_data
	TextureManager::loadWrappedText(renderer, font, file_data.c_str(), &BLACK, LEFT_TEXT_BUFFER, HEADER_HEIGHT, shape->w, 20);

}//END OF render()

/**
 * If the event was a keyboard button, handle that event for the text editor. This includes backspace, enter, and
 * typing more text into the file.
 */
void TextEditor::handleKeyPress(SDL_Event* event) {

	//If the event was a key pressed down
	if (event->type == SDL_KEYDOWN) {
		//If it was a backspace, and there is data left to delete
		if (event->key.keysym.sym == SDLK_BACKSPACE && file_data.length() > 1) {
			//Delete the cursor, displayed as '|'
			file_data.pop_back();

			//Delete the character 
			file_data.pop_back();

			//Put the cursor back onto the end
			file_data += "|";
		}//Else if it was the enter/return key
		else if (event->key.keysym.sym == SDLK_RETURN) {
			//Remove the cursor
			file_data.pop_back();

			//Add a line break and replace the cursor
			file_data += "\n|";
		}
	}

	//If the event was a text input from the keyboard
	if (event->type == SDL_TEXTINPUT) {

		//Remove the cursor
		file_data.pop_back();

		//Add the text from the event
		file_data += event->text.text;

		//Replace the cursor
		file_data += "|";
	}

}//END OF handleKeyPress()
