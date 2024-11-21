//Author: Connor Kamrowski. As of 11/17/2024, there aren't any other contributors.
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "GraphManager.h"
#include <iostream>

GraphManager* graph = nullptr;

int main(int argc, char* argv[]) {

	//initialize graphmanager
	graph = new GraphManager();

	//Render the blank graph
	graph->render();

	std::cout << "Welcome to version " + VERSION + " of node based notes, a note organization tool." << std::endl;

	//Select graph to load, or create new
	graph->openGraph();
	
	//while the graph is active, i.e. hasn't recieved any input to quit
	while (graph->isActive()) {

		//handle events
		graph->handleEvents();

		//update
		graph->render();

	}

	//Deconstruct the graph manager, closing all processes before main() returns
	graph->~GraphManager();

	return 0;
}
