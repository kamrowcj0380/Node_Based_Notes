//Author: Connor Kamrowski. See header file for more information.
#include "GraphManager.h"
#include "TextEditor.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>
namespace fs = std::filesystem;

//Initialize static public variables found in GraphManager
SDL_Renderer* GraphManager::renderer = nullptr;
TTF_Font* GraphManager::font = nullptr;

ButtonMenu* button_menu = nullptr;
TextMenu* text_menu = nullptr;

/**
 * Constructor for Graphmanager. Loads SDL, TTF, requirements like window and renderer, and sets the application's
 *	window size. Also creates the Graph directory if it doesn't already exist.
 */
GraphManager::GraphManager() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL Initialized" << std::endl;
	}

	//get the window size
	SDL_DisplayMode DM;
	SDL_GetCurrentDisplayMode(0, &DM);

	//initialize window_shape
	window_shape = new SDL_Rect();

	//Constants used from Config.h
	window_shape->w = DM.w / WINDOW_WIDTH_DENOM * WINDOW_WIDTH_NOM;
	window_shape->h = DM.h / WINDOW_HEIGHT_DENOM * WINDOW_HEIGHT_NOM;
	window_shape->x = window_shape->y = 0; //set pos to 0,0

	std::cout << "Width of window: " << window_shape->w << std::endl;
	std::cout << "Height of window: " << window_shape->h << std::endl;

	//Initialize the SDL_Window
	window = SDL_CreateWindow("Node Notes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_shape->w, window_shape->h, 0);

	//Initialize the SDL_Renderer using the window
	renderer = SDL_CreateRenderer(window, -1, 0);

	//Set the renderer to blend mode, which allows for transparent graphics by modifying the alpha value of colors
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//Initialize TTF, the text library for SDL2
	TTF_Init();

	//Start receiving SDL text input, allowing keyboard typing to be input
	SDL_StartTextInput();

	//Initialize the font with a base size of 10. This is changed each time text is rendered, so it isn't important
	font = TTF_OpenFont("Assets/Fonts/GloriousChristmas-BLWWB.ttf", 10);

	//Initialize the text editor scaled to the window's size, and pass it the font pointer
	text_editor = new TextEditor(window_shape->w, window_shape->h, font);

	//Create path to graphs in local directory if it doesn't exist
	if (!fs::exists(GRAPH_PATH)) {
		fs::create_directory(GRAPH_PATH);
	}

}//END OF GraphManager()

/**
 * Deconstructor for Graphmanager. Destroy the text editor, stop all SDL/TTF processes and destroy relevant objects,
 * then quit SDL and TTF.
 */
GraphManager::~GraphManager() {
	std::cout << "Cleaning GraphManager and closing program..." << std::endl;

	text_editor->~TextEditor();
	SDL_StopTextInput();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(font);
	SDL_Quit();
	TTF_Quit();

	std::cout << "All Done." << std::endl;

}//END OF ~GraphManager()

/**
 * Open a graph. Currently rudimentary, this just takes command line input and verifies that the directory exists.
 * Then, it creates a node for each text file in that directory. It doesn't yet have the option to create a new 
 * directory. That will be added AFTER a menu system has been created, as it will rely on the ouptut from that
 * menu to function. I could implement it now, but I'm one person, and this is my chosen priority.
 * 
 * NOTE: This is really sloppy, and one of the top items on the TODO list is to recreate it. It serves as a 
 *	basic process only so other functionality can be developed and tested.
 */
void GraphManager::openGraph() {
	struct stat info;

	int choice = runButtonMenu("Welcome to Node Based Graphs. What would you like to do?",
		{ "Load an existing graph", "Create a new graph", "Delete a graph" });

	std::string message = "";
	//Load an existing graph
	if (choice == 0) {
		//prompt a button menu to select a graph from the directory
		int result = promptGraphSelection("What graph would you like to open?");

		if (result > 0) {
			openGraph();
		}
	}
	//Create a new graph
	else if (choice == 1) {
		while (true) {
			std::string* new_graph_file_path = runTextMenu("Name the new graph.");

			if (new_graph_file_path == nullptr) {
				active = false;
				break;
			}

			graph_file_path = *new_graph_file_path;
			fixGraphFilePath();

			if (!fs::exists(graph_file_path)) {
				fs::create_directory(graph_file_path);
				break;
			}
			else if (runButtonMenu("The graph already exists. Load the graph?", { "Load the graph", "Use a new name" }) == 0) {
				break;
			}
		}
	}
	//Delete an existing graph
	else if (choice == 2) {
		//prompt a button menu to select a graph from the directory
		if (promptGraphSelection("Which graph would you like to DELETE?") == 0 && 
			runButtonMenu("Are you sure you would like to delete " + graph_file_path + "?", { "Yes", "No" }) == 0) {
			fs::path to_delete = graph_file_path;
			bool result = fs::remove_all(to_delete);
			if (!result) {
				std::cout << "Something went wrong deleting the directory. Path: " + graph_file_path;
				exit(0);
			}
		}

		//return to the start - only opening a graph leaves this function
		openGraph();
	}

	if (!active) {
		return;
	}

	//Load the graph

	//This is a bit sloppy, but directory iterator is new to me.
	//For each entry in the directory 'graph_file_path', open the file
	int i = 0;
	for (const auto& entry : fs::directory_iterator(graph_file_path)) {
		//If it isn't a .txt file, then skip this iteration
		if (entry.path().extension().string() != ".txt") {
			continue;
		}

		//Create a string for the name including only the name of the .txt file
		std::string name = entry.path().string().substr( graph_file_path.size()+1 );
		
		//Remove the '.txt' from the name
		name = name.substr(0, name.size()-4);

		//Create a string to store the file path
		std::string outfile_path = entry.path().string();

		//Add a node to the manager's vector to represent the file
		addNodeToVector(name, outfile_path, 100 + 50*i, 100);

		i++;
	}

	//Make sure it's a file path
	if (graph_file_path.back() != '/') {
		graph_file_path += "/";
	}

} //END OF openGraph()

/**
 * Open a graph. Currently rudimentary, this just takes command line input and verifies that the directory exists.
 * Then, it creates a node for each text file in that directory. It doesn't yet have the option to create a new
 * directory. That will be added AFTER a menu system has been created, as it will rely on the ouptut from that
 * menu to function. I could implement it now, but I'm one person, and this is my chosen priority.
 *
 * NOTE: This is really sloppy, and one of the top items on the TODO list is to recreate it. It serves as a
 *	basic process only so other functionality can be developed and tested.
 */
void GraphManager::handleEvents() {
	SDL_Event event;
	//Wait for an event to happen before proceeding. Mouse movement, keyboard input, closing the program are all events
	SDL_WaitEvent(&event);

	//if the red x was clicked, close the program. Strangely, this must be programmed in...
	if (event.type == SDL_QUIT) {
		active = false;
	}
	
	//if target is not null, then the text editor is open
	//if handleTextEditorEvent returns false, the text editor wasn't the target for interaction
	if (target == nullptr || !handleTextEditorEvent(&event)) {
		//in either case, check graph for events now
		handleGraphEvent(&event);
	}

	//if target was deselected somehow, update its selection status
	if (target != nullptr) {
		target->updateSelectStatus(true);
	}

}//END OF handle_events()

/**
 * Handle events that relate to the graph, as opposed to the text editor.
 */
void GraphManager::handleGraphEvent(SDL_Event* event) {

	//If the event was a key being pressed down
	if (event->type == SDL_KEYDOWN) {

		//Switch to a task based on the key that was pressed
		switch (event->key.keysym.sym) {
		case (SDLK_ESCAPE): {
			//Exit the program. In the future, this will instead open a main menu
			std::cout << "Escape" << std::endl;
			int choice = runButtonMenu("Graph Editor Paused.",
				{"Unpause", "Return to Menu", "Save and Exit"});
			if (choice == 0) {
				//Do nothing, just let this pass.
			}
			else if (choice == 1) {
				//AAAAATODO: This doesn't work outright. Delete the previous graph first.
				clearGraph();
				openGraph();
			}
			else if (choice == 2) {
				active = false;
			}
			break;
		}
		case (SDLK_F5):
			//Fullscreen the program. This is a temporary feature that will probably be removed.
			std::cout << "f5" << std::endl;
			SDL_SetWindowFullscreen(window, fullscreen);
			fullscreen = !fullscreen;
			break;
		default:
			break;
		}

		//Return to avoid rest of function
		return;
	}

	//Get mouse location
	int mouse_flags = SDL_GetMouseState(&mousex, &mousey);

	//Test if hovering over an object
	Node* current_target = detectNodeUnderMouse(mousex, mousey);

	//Update graphics of hovered and selected nodes
	//If current target is not null, then you are hovering over a node
	if (current_target != nullptr) { // && mouse_flags != 1    ---> AAAAATODO: DOESN'T WORK?
		hover_target = current_target;
		hover_target->updateHoverStatus(true);
	}//if current target is null, then there is nothing to hover over
	else if (hover_target != nullptr) {
		//Update the hover target's graphics to show it isn't hovered over
		hover_target->updateHoverStatus(false);

		//Remove access to the expired hover target
		hover_target = nullptr;
	}

	//If a mouse button was pressed, and it was the left button
	if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
		//If the current target is not null
		if (current_target != nullptr) {
			//and the target is not the current target
			if (target != current_target) {
				//If there was a target previously
				if (target != nullptr) {
					//Update the old target, which is no longer selected, and save the file
					deselectTargetNode();
				}
				
				//Open text editor for the new target
				openTargetNode(current_target);
			}

			//print out the title of the target node when clicked
			std::cout << "Node clicked. Title: " << target->getTitle() << std::endl;
		}//The current target was null
		else if (event->button.clicks == 2) { //If there were two clicks
			//Make a new node
			std::cout << "Double click, no collision? Make a new node!" << std::endl;

			//Create a new node for this graph centered on the mouse position
			createNode(mousex, mousey);

		}//It was a single click hovering over empty space
		else {
			//Nothing was under the mouse
			std::cout << "Nothing under mouse on click." << std::endl;

			//If there was a target, remove it
			if (target != nullptr) {
				deselectTargetNode();
			}
		}
	}//Else if a mouse button was released, and it was the left mouse button
	else if (event->type = SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT) {
		//If there is a target
		if (target != nullptr) {
			//Move the target to where the mouse is
			target->setPos(mousex, mousey);
		}
	}

}//END OF handleGraphEvent()

/**
 * Handle events that relate to the text editor, as opposed to the graph.
 * Returns true if a text editor related event occurred, otherwise returns false
 */
bool GraphManager::handleTextEditorEvent(SDL_Event* event) {

	//get mouse location
	int mouse_flags = SDL_GetMouseState(&mousex, &mousey);

	//If a key was pressed, or text input was detected
	if (event->type == SDL_KEYDOWN || event->type == SDL_TEXTINPUT) {

		//If the key was escape, deselect the target opened by the text editor
		if (event->key.keysym.sym == SDLK_ESCAPE) {
			deselectTargetNode();
		}
		else {
			//handle the key press
			text_editor->handleKeyPress(event);
		}

		//There was an event
		return true;
	}//Else if the mouse was inside of the text editor, consider it a text editor event and return true
	else if (mousey > 0 && mousey < text_editor->getShape()->h && mousex > 0 && mousex < text_editor->getShape()->w) {

		//If the mouse was clicked, say so. That's all this does for now
		if (event->type == SDL_MOUSEBUTTONDOWN) {
			std::cout << "Clicked on text editor area" << std::endl;
		}

		//There was an event
		return true;
	}

	//Base case, return false if none of the above applied
	return false;

}//END OF handleTextEditorEvent()

/**
 * Render the graph, and the text editor if a target is currently selected.
 */
void GraphManager::render() {

	//Clear the text from the renderer
	SDL_RenderClear(renderer);

	//Render graph background
	Custom_SDLDrawRect(renderer, &GRAPH_BACKGROUND_COLOR, window_shape->x, window_shape->y, window_shape->w, window_shape->h);

	//Render all nodes in the vector
	for (int i = 0; i < nodes.size(); i++) {
		nodes.at(i)->render(renderer);
	}

	//If there is a target
	if (target != nullptr) {
		//Render the text editor
		text_editor->render(renderer, target);
	}

	//Present the renderer after everything was loaded in
	SDL_RenderPresent(renderer);

}//END OF render()

/**
 * Creates a Node object, and adds it to the vector of nodes active in this graph. All details for the Node are
 * passed in as parameters.
 */
void GraphManager::addNodeToVector(std::string title, std::string file_name, int x_pos, int y_pos, bool creating_new_node) {
	//Create a temporary node defined by parameters
	Node* temp = new Node(title, file_name, x_pos, y_pos);

	//Add the node to the vector
	nodes.push_back(temp);

	//If this Node was created during runtime
	if (creating_new_node) {
		//Open the node, displaying the text editor
		openTargetNode(temp);

		//This node is also being hovered over, as it was just created under the mouse
		hover_target = temp;
	}

}//END OF addNode()

void GraphManager::createNode(int x_pos, int y_pos) {
	text_menu = new TextMenu(window_shape->w, window_shape->h, window_shape->w / MENU_WIDTH_DENOM, "Name the new node:");
	std::string response = *text_menu->waitEvent(renderer);
	std::string file_path = graph_file_path + response + ".txt";

	//Open an out file stream at the file_path
	std::ofstream outfile(file_path);

	//outfile << "EMPTY" << std::endl;

	//Close the output file
	outfile.close();


	addNodeToVector(response, file_path, x_pos, y_pos, true);

}


/**
 * Find a node at the position of the mouse coordinates.
 * Returns nullptr if no such Node exists.
 */
Node* GraphManager::detectNodeUnderMouse(int mousex, int mousey) {

	//For every Node in the vector
	for (int i = 0; i < nodes.size(); i++) {
		//Get the SDL_Rect for the Node
		SDL_Rect* node_shape = nodes.at(i)->getShape();

		//Long story short, if the mouse coordinates are inside of the Node's boundaries
		if (mousex > node_shape->x && mousex < node_shape->x + node_shape->w
			&& mousey > node_shape->y && mousey < node_shape->y + node_shape->h) {
			//Return that Node
			return nodes.at(i);
		}
	}

	//No node was found, return nullptr
	return nullptr;

}//END OF detectNodeUnderMouse()

/**
 * Deselect the target node, closing the text editor aswell
 */
void GraphManager::deselectTargetNode() {
	//Close the text editor, saving data to file stored in target
	text_editor->close(target);

	//Deselect target (graphics)
	target->deselect();

	//Set the target to be null, now that it's closed
	target = nullptr;

}

/**
 * Open a target node, along with the text editor
 */
void GraphManager::openTargetNode(Node* new_target) {
	target = new_target;

	//Open the text editor, loading the file stored in the Node object
	text_editor->open(target);

}

std::string* GraphManager::runTextMenu(std::string message) {
	if (!active) {
		return nullptr;
	}

	//render the background again //AAAAATODO: Is this necessary? Probably not
	render();

	text_menu = new TextMenu(window_shape->w, window_shape->h, window_shape->w / MENU_WIDTH_DENOM, message.c_str());
	std::string* response = text_menu->waitEvent(renderer);

	if (response == nullptr) {
		//quit the program
		active = false;
	}

	return response;
}

//AAAAATODO: comment on this
int GraphManager::runButtonMenu(std::string message, std::vector<std::string> buttons) {
	if (!active) {
		return -1;
	}
	//render the background
	render();

	//create a new button menu
	button_menu = new ButtonMenu(window_shape->w, window_shape->h, window_shape->w / MENU_WIDTH_DENOM, message.c_str(), buttons);

	int result = button_menu->waitEvent(renderer);

	if (result == -1) {
		active = false;
	}

	return result;
}

void GraphManager::fixGraphFilePath() {
	graph_file_path = GRAPH_PATH + graph_file_path;
}

void GraphManager::clearGraph() {
	nodes.clear();
}

int GraphManager::promptGraphSelection(std::string message) {
	std::vector<std::string> graph_directories;

	for (const auto& entry : fs::directory_iterator(GRAPH_PATH)) {
		//convert the path to a string
		std::string path = entry.path().u8string();
		//and add to the vector of paths
		graph_directories.push_back(path);
	}

	//AAAAATODO: make a scroll wheel for the button menu if it is taller than the screen!
	//If there are existing graphs
	if (graph_directories.size() > 0) {
		int dir_index = runButtonMenu(message, graph_directories);
		graph_file_path = graph_directories.at(dir_index);
		return 0;
	}
	else {
		runButtonMenu("Sorry! There aren't any graphs available.", { "Okay" });
		//restart the function
		return 1;
	}

}
