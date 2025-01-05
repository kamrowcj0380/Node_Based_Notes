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

/**
 * Constructor for Graphmanager. Loads SDL, TTF, requirements like window and renderer, and sets the application's
 *	window size. Also creates the Graph directory if it doesn't already exist.
 */
GraphManager::GraphManager() {
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL Initialized" << std::endl;
	}
	else {
		std::cout << "SDL could not be loaded. Error info: " << SDL_GetError() << std::endl;
		exit(0);
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

	//Initialize the SDL_Window
	window = SDL_CreateWindow("Node Notes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_shape->w, window_shape->h, 0);
	if (window == NULL) {
		std::cout << "Window could not be created. Error info: " << SDL_GetError() << std::endl;
		exit(0);
	}

	//Initialize the SDL_Renderer using the window
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL) {
		std::cout << "Renderer could not be created. Error info: " << SDL_GetError() << std::endl;
		exit(0);
	}

	//Set the renderer to blend mode, which allows for transparent graphics by modifying the alpha value of colors
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	//Initialize TTF, the text library for SDL2
	if (TTF_Init() != 0) {
		std::cout << "TTF could not be loaded. Error info: " << TTF_GetError() << std::endl;
		exit(0);
	}

	//Start receiving SDL text input, allowing keyboard typing to be input
	SDL_StartTextInput();

	//Initialize the font with a base size of 10. This is changed each time text is rendered, so it isn't important
	font = TTF_OpenFont("Assets/Fonts/GloriousChristmas-BLWWB.ttf", 10);
	if (font == NULL) {
		std::cout << "TTF Font could not be loaded." << std::endl;
		exit(0);
	}

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

	//If a graph is open, save it before exiting
	if (graph_open) {
		closeGraph();

		//And, if there is a target, the text editor was active. Save the contents of the file
		if (target != nullptr) {
			text_editor->close(target);
		}
	}

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
		createNewGraph();
	}
	//Delete an existing graph
	else if (choice == 2) {
		deleteGraph();

		//return to the start - only opening a graph leaves this function
		openGraph();
	}

	//If the above logic flagged the end of this function, return
	if (!active) {
		return;
	}

	//Make sure it's a file path
	if (graph_file_path.back() != '/') {
		graph_file_path += "/";
	}

	//Load the graph

	//Map of node metadata, i.e. names as key, and metadata as value
	std::unordered_map<std::string, std::string> node_data = loadMetadataFromNBG();

	//For each entry in the directory 'graph_file_path', open the file
	for (const auto& entry : fs::directory_iterator(graph_file_path)) {
		//If it isn't a .txt file, then skip this iteration
		if (entry.path().extension().string() != ".txt") {
			continue;
		}

		//Create a string for the name including only the name of the .txt file
		std::string name = entry.path().string().substr( graph_file_path.size() );
		
		//Remove the '.txt' from the name
		name = name.substr(0, name.size()-4);

		//Create a string to store the file path
		std::string outfile_path = entry.path().string();

		int x = 100;
		int y = 100;

		//if the value exists in the node_metadata map
		if (node_data.find(name) != node_data.end()) {
			//get the metadata (currently just the position)
			std::string pos = node_data[name];
			x = std::stoi( pos.substr(0, pos.find(",")) );
			y = std::stoi( pos.substr(pos.find(",")+1) );
		}

		//Add a node to the manager's vector to represent the file
		addNodeToVector(name, outfile_path, x, y);

	}

	graph_open = true;

} //END OF openGraph()

//TODO: Comment
void GraphManager::createNewGraph() {
	while (true) {
		std::string* graph_name = runTextMenu("Name the new graph.");

		//if the name was a null pointer, exit the program
		if (graph_name == nullptr) { 
			active = false;
			return;
		}

		//store the graph name as the graph_file_path
		graph_file_path = *graph_name;

		fixGraphFilePath();

		//if the file doesn't exist
		if (!fs::exists(graph_file_path)) {
			//create the new graph
			fs::create_directory(graph_file_path);
			break;
		}
		//The file already exists
		else if (runButtonMenu("The graph already exists. Load the graph?", { "Load the graph", "Use a new name" }) == 0) {
			break;
		}
	}
}

//TODO: Comment
void GraphManager::deleteGraph() {
	//If a graph is selected, and the user confirms deletion
	if (promptGraphSelection("Which graph would you like to DELETE?") == 0 &&
		runButtonMenu("Are you sure you would like to delete " + graph_file_path + "?", { "Yes", "No" }) == 0) {
		
		//Make path string into a filesystem::path
		fs::path to_delete = graph_file_path;

		//If the graph wasn't deleted
		if (!fs::remove_all(to_delete)) {
			//Alert in the command line
			std::cout << "Something went wrong deleting the directory. Path: " + graph_file_path;

			//And quit
			exit(0);
		}
	}
}

//TODO: Comment on this
void GraphManager::handleEvents() {
	SDL_Event event;
	//Wait for an event to happen before proceeding. Mouse movement, keyboard input, closing the program are all events
	SDL_WaitEvent(&event);

	//if the red x was clicked, close the program. Strangely, this must be programmed in...
	if (event.type == SDL_QUIT) {
		active = false;
		return;
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
			if (choice == 1) {
				closeGraph();
				openGraph();
			}
			else if (choice == 2) {
				closeGraph();
				active = false;
			}
			break;
		}
		case (SDLK_F5):
			//Fullscreen the program. This is a temporary feature that will probably be removed.
			std::cout << "f5" << std::endl;
			if (fullscreen) {
				SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			}
			else {
				SDL_SetWindowFullscreen(window, 0);
			}
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
	if (current_target != nullptr) {
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
		//If the key was delete, ask to delete this Node
		else if (event->key.keysym.sym == SDLK_DELETE) {
			promptDeleteNode();
		}
		else {
			//handle the key press
			text_editor->handleKeyPress(event);
		}

		//There was an event
		return true;
	}//Else if the mouse was inside of the text editor, consider it a text editor event and return true
	else if (isWithin(mousex, mousey, 0, 0, text_editor->getShape()->w, text_editor->getShape()->h)) {

		//If a mouse button was pressed, and the mouse was within the coordinates for the header
		if (event->type == SDL_MOUSEBUTTONDOWN && isWithin(mousex, mousey, 0, 0, text_editor->getShape()->w, HEADER_HEIGHT)) {
			//ask to rename, delete, or cancel
			int result = runButtonMenu("What would you like to do to the node?", { "rename", "delete", "cancel" });

			//rename the node
			if (result == 0) {
				std::string new_title = ensureUniqueNodeName(*runTextMenu("Rename the Node."), "Rename the Node.");
				target->setTitle(new_title);
			}
			//delete the node
			else if (result == 1) {
				promptDeleteNode();
			}
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

	//Make sure the text file doesn't exist, or you'll be overwriting it!
	std::string response = ensureUniqueNodeName(*runTextMenu("Name the new node."), "Name the new node.");
	std::string file_path = graph_file_path + response + ".txt";

	//Open an out file stream at the file_path
	std::ofstream outfile(file_path);

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

/*
 * Prompt the user for text input using the given message.
 */
std::string* GraphManager::runTextMenu(std::string message) {
	if (!active) {
		return nullptr;
	}

	//render the background again
	render();

	TextMenu* text_menu = new TextMenu(window_shape->w, window_shape->h, window_shape->w / MENU_WIDTH_DENOM, message.c_str());
	std::string* response = text_menu->waitEvent(renderer);

	if (response == nullptr) {
		//quit the program
		active = false;
	}

	return response;
}

/*
 * Create a button menu, and prompt the user with the given message and button options.
 */
int GraphManager::runButtonMenu(std::string message, std::vector<std::string> buttons) {

	if (!active) {
		return -1;
	}
	//render the background
	render();

	//create a new button menu
	ButtonMenu* button_menu = new ButtonMenu(window_shape->w, window_shape->h, window_shape->w / MENU_WIDTH_DENOM, message.c_str(), buttons);
	int result = button_menu->waitEvent(renderer);

	if (result == -1) {
		active = false;
	}

	return result;
}

void GraphManager::fixGraphFilePath() {
	graph_file_path = GRAPH_PATH + graph_file_path;
}

void GraphManager::closeGraph() {
	std::string nbg_path = graph_file_path + GRAPH_DATA_PATH;

	//Open this object's nbg file
	std::ofstream outfile(nbg_path, std::ofstream::trunc);

	//If it opened successfully, save the position of each node
	if (outfile.good()) {

		//For each node, write the title, x, and y to the file followed by a line break
		for (int i = 0; i < nodes.size(); i++) {
			Node* temp = nodes.at(i);
			outfile << temp->getTitle() << " at " << temp->getShape()->x << "," << temp->getShape()->y << std::endl;
		}

	}
	else {
		std::cout << "ERROR: The Node metadata could not be saved upon exiting." << std::endl;
	}

	//close the file
	outfile.close();

	//Clear the node vector for future use
	nodes.clear();

	graph_open = false;
}

int GraphManager::promptGraphSelection(std::string message) {
	std::vector<std::string> graph_directories;

	for (const auto& entry : fs::directory_iterator(GRAPH_PATH)) {
		//convert the path to a string
		std::string path = entry.path().u8string();
		//and add to the vector of paths
		graph_directories.push_back(path);
	}

	//If there are no existing graphs
	if (graph_directories.size() == 0) {
		//Alert the user that there are no existing graphs
		runButtonMenu("Sorry! There aren't any graphs available.", { "Okay" });
		return 1;
	}

	int dir_index = runButtonMenu(message, graph_directories);
	
	graph_file_path = graph_directories.at(dir_index);

	return 0;

}

void GraphManager::promptDeleteNode() {

	//If they don't want to delete the node, then return
	if (runButtonMenu("Are you sure you would like to delete " + target->getTitle() + "?", { "Yes", "No" }) == 1) {
		return;
	}


	//delete the node from the vector
	nodes.erase(std::remove(nodes.begin(), nodes.end(), target), nodes.end());

	//delete the file
	fs::path to_delete = target->getFName();
	bool result = fs::remove(to_delete);
	if (!result) {
		std::cout << "Something went wrong deleting the directory. Path: " + target->getFName();
		exit(0);
	}

	//Close the text editor without saving (as the file is deleted)
	text_editor->close();

	//Make the target null, as it doesn't exist anymore
	target = nullptr;

}

/*
 * Ensure that the given node_name is unique, i.e. doesn't exist in the current Graph directory.
 * If it isn't unique, loop until a unique name is given. Return the unique node name.
 */
std::string GraphManager::ensureUniqueNodeName(std::string node_name, std::string message) {

	//While a node exists in the graph directory with that name
	while (fs::exists(graph_file_path + node_name + ".txt")) {
		//Prompt the user for a new name for the node
		node_name = *runTextMenu("That node already exists.\n" + message);
	}

	return node_name;

}

//commentodo
std::unordered_map<std::string, std::string> GraphManager::loadMetadataFromNBG() {
	std::ifstream node_file;
	std::string line;

	std::unordered_map<std::string, std::string> data;

	//Open the nbg file for this graph
	node_file.open(graph_file_path + GRAPH_DATA_PATH);

	//If opened successfully
	if (node_file.is_open()) {

		//While there are lines left in node_file
		while (node_file.good()) {

			//Copy the next line of text
			std::getline(node_file, line);

			//Quit if the line is empty
			if (line.empty()) { break; }

			//Separate the name and metadata of the node
			std::string name = line.substr(0, line.find(" at "));
			std::string metadata = line.substr(line.find(" at ") + 4);

			data[name] = metadata;

		}

	}
	else {
		std::cout << "Couldn't open graph data. Using default positions." << std::endl;
	}

	node_file.close();

	return data;

}

