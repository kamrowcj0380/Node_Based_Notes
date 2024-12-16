# Roadmap

## Index
- [Current Version](#current)
- [Versions](#versions)

## <a id="current"></a> Current Version
The current version is 0.2. The project is in early stages, although basic funcitonality has been defined and implemented.

## <a id="versions"></a> Versions
Currently, the version is below 1.0. This is a hard initial development stage, and it's extremely fresh, so most key funcitonality is missing.
When version 1.0 is completed, a proper release will be created.

### Version 1.0
- [ ] Make a successful, full build available in a release.

### Version x...
A few versions will take place leading up to 1.0. This is a placeholder for those.

### Version 0.5
A future update for this project. Create depth in the existing functionality. Implement connections, and GUI updates based on connections and the target node.

### Version 0.4
A future update for this project. Update all of the GUI to be user-friendly and reliable, and create different styles of nodes.
- [ ] Add "exit" buttons
    - [ ] text menus, to cancel entering text
    - [ ] main menu should allow the user to close the program
    - [ ] options within the main menu should allow the user to return to the main menu instead of requiring a choice
- [ ] Make Node utilities more visible
- [ ] Resolve text overflow on most text inputs
    - [ ] Naming a graph / node in text menus
    - [ ] Implement a scroll bar for the text editor
- [ ] Create multiple node types
    - [ ] Topic
    - [ ] Header Node
    - [ ] Sub Node
- [ ] Implement click and drag to navigate the graph
- [ ] More TBD

### Version 0.3
The next update for this project. This is the graph file update - the graphs will load and save with more definition.
- [x] Node positions will be saved to a file, and read upon loading a graph
    - [x] To handle errors caused by missing node coordinates, make a default node position around 100, 100 
- [x] Fix loading menu, where closing the program causes an error
- [ ] Massive refactor of event code, calling one function for processing any events. Determine if this is feasible and necessary
- [ ] More TBD

### Version 0.2
This is the menu update, where the focus is to implement menus for different tasks.
- [x] Menu for renaming a Node, or deleting a Node (accessible by clicking the node's name in the text editor)
- [x] Remove "TODO" list from the bottom of this document, instead moving the tasks to "issues" section in GitHub repository.
- [x] Create a menu header and .cpp
    - [x] A menu that offers text input
    - [x] A menu that offers a choice between two buttons (there may be no need for more than this)
- [x] Menu for creating a new graph
- [x] Menu for naming a new graph
- [x] Menu for opening a graph ~~naming a graph to open~~
- [x] Menu for naming a new Node

### Version 0.1
A base upload of the functionality. Since I was able to complete the basic requirements, I've uploaded my code to a public repository for version control and project safety (should my PC fail, I don't want to lose this...).




