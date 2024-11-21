# Roadmap

## Index
- [Versions](#versions)
- [TODO List](#todo_list)

## <a id="versions"></a> Versions
Currently, the version is below 1.0. This is a hard initial development stage, and it's extremely fresh, so most key funcitonality is missing.
When version 1.0 is completed, a proper release will be created.

### Version 0.3
A future update for this project. This is the graph file update - the graphs will load and save with more definition.
- [ ] Node positions will be saved to a file, and read upon loading a graph
- [ ] More TBD

### Version 0.2
The next update of this project, and funcitonality required for me to consider this "version 0.2". This is the menu update, where the focus is to implement menus for different tasks.
- [ ] Menu for renaming a Node, or deleting a Node
- [x] Create a menu header and .cpp
    - [x] A menu that offers text input
    - [x] A menu that offers a choice between two buttons (there may be no need for more than this)
- [x] Menu for creating a new graph
- [x] Menu for naming a new graph
- [x] Menu for opening a graph ~~naming a graph to open~~
- [x] Menu for naming a new Node

### Version 0.1
A base upload of the functionality. Since I was able to complete the basic requirements, I've uploaded my code to a public repository for version control and project safety (should my PC fail, I don't want to lose this...).


## <a id="todo_list"></a>TODO List
Everything missing from the project, excluding a few tasks in my personal notes. This may be unclear, and innacurate as time goes on. It's better to refer to the [versions](#versions) section for information about current tasks.

- text is currently rendered in the top left corner, with a slight offset from the left. Use the dimensions of the container to space it evenly within its container instead
- insane memory leak when the text editor is open, and events happen. This may be due to memory issues in SDL or TTF, but either way, optimize the rendering so that it only renders when the text changes
- make a save file system
    - when the graph is closed, it should store the x,y position of each Node
    - it should also store connections (when connections are implemented...)
- make a menu header and cpp file
    - decide between loading and creating a new graph
    - use when naming a graph or selecting the graph to open
    - use when creating a new node (to name it)
- nodes cannot be connected. Add functionality to draw lines between nodes to show the idea map structure intended by this project
- GraphManager has public variables right now; verify that it is necessary (I don't believe it is), or put them into private and add getter functions
- when GraphManager is constructed, add error checking for SDL_INIT, TTF_INIT, window creation, etc. Quit if any of these fail, and give error information
- when GraphManager is deconstructed, make sure the target node's information is saved before closing. As custom files are created, make sure the whole file is saved, and the text editor is saved and closed first
- existence of "Graphs/" is verified on startup, but should instead be verified whenever a graph is being opened, as it relies on this to function. Error handling in case the folder is deleted by the user during runtime would also be appropriate
- reformat GraphManager::openGraph(), it's a bit of a mess intended to be 'just good enough' while other features were implemented and tested
- consider how graphs are saved, and potentially create a custom graph file extension for use in the program
    - potential: .nbg, for 'node based graph'
    - text files could still exist, but a .nbg file would define node positions, and be read on startup, then saved to on program completion
- in TextEditor::render(), the y position is determined only by HEADER_HEIGHT. Instead, add a variable buffer so that a scroll wheel can be implemented
    - when scroll wheel input occurs, this will be increased/decreased within bounds, showing text that is currently displayed off screen.
- the program Doxygen seems fairly common, so I've formatted my header file comments to fit this
    - generate documentation using Doxygen
    - use anchor and ref in order to create links in the documentation. Use this to connect .cpp function comments to their header's comments, which are more descriptive

