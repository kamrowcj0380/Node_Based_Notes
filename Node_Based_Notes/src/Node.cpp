//Author: Connor Kamrowski. See header file for more information.
#include "Node.h"

/**
 * The Node constructor. Given a title, filename, and x,y coordinates, create a new Node object.
 */
Node::Node(std::string node_title, std::string file_name, int x_pos, int y_pos) {
	title = node_title;
	fname = file_name;

	//Initialize the shape SDL_Rect
	shape = new SDL_Rect();
	shape->x = x_pos - NODE_WIDTH / 2;
	shape->y = y_pos - NODE_WIDTH / 2;
	shape->w = NODE_WIDTH;
	shape->h = NODE_WIDTH;

}//END OF Node()

//A base deconstructor, without any functionality
Node::~Node() {}

/**
 * Given the SDL_Renderer*, render the node. This changes based on the status of the Node.
 */
void Node::render(SDL_Renderer* renderer) {
	//Set the render draw color to a constant SDL_Color based on the status
	if (is_selected) {
		CustomSDL_setRenderDrawColor(renderer, &NODE_SELECTED_COLOR);
	}
	else if (hovered_over) {
		CustomSDL_setRenderDrawColor(renderer, &NODE_HOVER_COLOR);
	}
	else {
		CustomSDL_setRenderDrawColor(renderer, &NODE_BASE_COLOR);
	}

	//If the Node is selected or hovered, increase the size by a constant
	int diff = 0;
	if (is_selected || hovered_over) {
		diff = NODE_SIZE_INCREASE;
	}

	//Draw the rectangle to the renderer
	Custom_SDLDrawRect(renderer, shape->x - diff, shape->y - diff, shape->w + diff * 2, shape->h + diff * 2);

}//END OF render()


/**
 * A simple setter function; change the x,y coordinates of the Node to be the x,y parameters.
 * The x,y position is relative to the center of the Node, NOT the top-left corner.
 */
void Node::setPos(int x, int y) {
	shape->x = x-shape->w/2;
	shape->y = y-shape->h/2;
}

/**
 * A complicated setter function.
 * Change the title of the node, as well as the file path associated with the node.
 */
void Node::setTitle(std::string new_title) {

	//get the index of the title in 'fname' string
	int start = fname.find(title);
	int end = start + title.length();

	//change the file path to reflect the new title
	std::string old_path = fname;
	fname.replace(start, end, new_title);

	fname += ".txt";

	std::cout << "Old: " << old_path << std::endl;
	std::cout << "New: " << fname << std::endl;

	//change the file
	if (std::rename(old_path.c_str(), fname.c_str()) != 0) {
		std::cout << "Issue: Node's file was not renamed." << std::endl;
	}

	//change the title
	title = new_title;

}