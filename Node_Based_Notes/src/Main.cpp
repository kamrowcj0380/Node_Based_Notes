#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "GraphManager.h"
#include <iostream>

GraphManager* graph = nullptr;

//TODO
// Put all of this in roadmap.md when pushed to repository. This will happen soon after the repository is created.
// 
// text is currently placed randomly. Use the dimensions of the container to space it evenly
// insane memory leak when the text editor is open. Something has to be rendering that isn't disappearing...
// // It appears to be a memory leak based in the TTF Library... eesh. Find a way to reduce the number of times the
// //	screen renders, avoiding rendering on events that don't change content on the screen. Unfortunately, this includes
// //	key presses, but it can at least be mitigated if the key presses don't result in actions.
// when a graph is closed, it should also save the coordinates of the nodes
// make menus, including graph loading, creation, node naming, etc.
// text is constantly rendered - set it to only render after a MEANINGFUL event, not just any
// nodes are not connected in the slightest - big TODO
// remove public variables from GraphManager
// on GraphManager constructior, add error checking for SDL initializations
// on ~GraphManager(), if there is a target node, save the text editor
// existence of "Graphs/" is only verified during startup. Move this to function where graphs are loaded
// thinkin about above, reformat the function where graphs are loaded. It's messy
// debate how graphs are saved, considering a custom graph file extension like .nbg (node based graph)
// in trying out Doxygen, use \ref and \anchor, like so:
//		\anchor GraphManager_addNode
//		\ref <GraphManager_addNode> ["(See definition in header file.)"]
// add "spatial buffer" to HEADER_HEIGHT in the text_editor render function, so that scroll wheel can be implemented


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
