/*
 * GraphManager is the core of this project. It contains all of the necessary functionality, and is the only file
 * that Main.cpp interacts with.
 * 
 * GraphManager: 
 *	* Loads all utilities, including SDL and TTF
 *  * Contains SDL tools including the renderer and font(s), and cleans them when the program is closed
 *  * Describes the life of the program (if !active, the program will end)
 *  * Manages the TextEditor, and contains pointers to all Nodes
 *  * Handles events during runtime (user interaction such as keyboard or mouse inputs)
 * 
 *
 * Author: Connor Kamrowski
 */
#pragma once
#include "Config.h"
#include "Utils.h"

#include "Node.h"
#include "TextureManager.h"
#include "TextEditor.h"


class GraphManager {
public:
	/**
	 * GraphManager constructor. The following tasks are performed:
	 * 1. Initialize SDL
	 * 2. Get the size of the screen, and create an SDL_window that is 2/3 the screen size
	 * 3. Initialize the SDL_Renderer using the window from previous step
	 * 4. Set SDL_Renderer to blend mode, to allow rendering of transparent objects
	 * 5. Initialize TTF
	 * 6. Start accepting SDL text input
	 * 7. Open the font and store within this object
	 * 8. Initialize the text_editor using the shape of the SDL_Window
	 * 9. Create a "Graphs/" directory if it doesn't already exist
	 */
	GraphManager();

	/**
	 * GraphManager deconstructor. The following tasks are performed:
	 * 1. Deconstruct the text_editor
	 * 2. Stop SDL text input
	 * 3. Destroy the SDL_Renderer
	 * 4. Destroy the SDL_Window
	 * 5. Close the font(s)
	 * 6. Quit SDL
	 * 7. Quit TTF
	 */
	~GraphManager();

	//The SDL renderer used by this program
	static SDL_Renderer* renderer;

	//The only TTF font used by this program
	static TTF_Font* font;

	/**
	 * Open a graph from the "Graphs/" directory. Given user input, load a specific directory to nodes displayed
	 * on the graph.
	 */
	void openGraph();

	/**
	 * Handle events, i.e. user inputs to the program during runtime.
	 */
	void handleEvents();

	/**
	 * Display the graphics from the ground up; show the background, then all nodes, then the text_editor if it's active.
	 */
	void render();

	/**
	 * \return the state of the program. False if the program was asked to complete.
	 */
	bool isActive() { return active; }

	/**
	 * Add a new Node to the vector of nodes stored by GraphManager.
	 * 
	 * \param title: The name of the Node to be added
	 * \param file_name: The name of the text file this node represents
	 * \param x_pos: The x position this node will be created at
	 * \param y_pos: The y position this node will be created at
	 * \creating_new_node: True if a node is being created during runtime
	 */
	void addNode(std::string title = "new node", std::string file_name = "NONE", int x_pos=0, int y_pos=0, bool creating_new_node = false);

	/**
	 * Add a new Node to the vector of nodes stored by GraphManager.
	 *
	 * \param mousex: the x position of the mouse
	 * \param mousey: the y position of the mouse
	 * \return the Node underneath the mouse. Returns nullptr if no such Node exists.
	 */
	Node* detectNodeUnderMouse(int mousex, int mousey);


	//TBD - unimplemented features
	//void deleteNode();
	//void connectNodes();

private:
	/**
	 * Mark the Node "target" as not selected, set target to nullptr, and close the text editor.
	 */
	void deselectTargetNode();

	/**
	 * Select a different Node to be the "target", and open the text editor for that Node.
	 * \param new_target: A Node pointer to replace "target"
	 */
	void openTargetNode(Node* new_target);

	/**
	 * Handle events specific to the Text Editor (as opposed to the graph).
	 * 
	 * \param event: The SDL_Event* describing a user input to the program
	 * \return true if there was an event handled for the text editor
	 */
	bool handleTextEditorEvent(SDL_Event* event);

	/**
	 * Handle events specific to the Graph (as opposed to the text editor).
	 * \param event: The SDL_Event* describing a user input to the program
	 */
	void handleGraphEvent(SDL_Event* event);

	//The x position of the mouse
	int mousex;

	//The y position of the mouse
	int mousey;

	//The path where this graph is stored
	std::string graph_file_path;

	//The SDL_Window used to render graphics
	SDL_Window* window;

	//The x,y and width, height of the SDL_Window
	SDL_Rect* window_shape;

	//Indicates whether the program is active or not
	bool active = true;

	//Indicates whether the program is fullscreen or not
	bool fullscreen = false;

	//The current target node, i.e. a node that was clicked with the left mouse button. If nullptr, there is no target node
	Node* target = nullptr;

	//The current node being hovered over. If nullptr, there is not a node under the mouse
	Node* hover_target = nullptr;

	//The vector of all Nodes displayed on this graph
	std::vector<Node*> nodes;

	//A pointer to the text editor object used to display and edit Node contents
	TextEditor* text_editor;
};