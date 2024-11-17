/*
 * Node is a core part of "Node Based Notes" for obvious reasons. Each node represents a text file in the Graph's directory.
 * Nodes can be accessed or modified by various files in this project.
 *
 * Author: Connor Kamrowski
 */
#pragma once
#include "Config.h"
#include "Utils.h"

class Node {
public:
	/**
	 * Node constructor. This initializes a few private variables specific to this class given parameters.
	 *
	 * \param window_width: The width of the application window
	 * \param window_height: The height of the application window
	 * \param input_font: The font used to display text
	 */
	Node(std::string title, std::string file_name, int x_pos, int y_pos);

	/**
	 * Node dconstructor. This doesn't do anything yet.
	 */
	~Node();

	/**
	 * Render the node using SDL, displaying any variation in size and color based on hover/select status.
	 * 
	 * \param renderer: The SDL_Renderer* being used
	 */
	void render(SDL_Renderer* renderer);

	/*** Getter Functions ***/

	/**
	 * \return The title of this node.
	 */
	std::string getTitle() { return title; }

	/**
	 * \return The x,y location and width,height of this Node.
	 */
	SDL_Rect* getShape() { return shape; }

	/**
	 * \return The filename relevant to this Node.
	 */
	std::string getFName() { return fname; }

	/**
	 * \return The Node's select status. True if the left mouse button was clicked on this node.
	 */
	bool getSelectStatus() { return is_selected; }

	/**
	 * \return The Node's hover status. True if mouse is hovering over this node.
	 */
	bool getHoverStatus() { return hovered_over; }

	/**
	 * Update the hover status of this Node. If true, the mouse is hovering over the node.
	 * \param new_status: The new boolean value of hovered_over.
	 */
	void updateHoverStatus(bool new_status) { hovered_over = new_status; }


	/*** Setter Functions ***/

	/**
	 * Update the selected status of this Node.
	 * \param new_status: The new boolean value of hovered_over.
	 */
	void updateSelectStatus(bool new_status) { is_selected = new_status; }

	/**
	 * Regardless of the Node's state, reset is_selected and hovered_over to false.
	 */
	void deselect() { is_selected = false; hovered_over = false; }

	/**
	 * Set the x,y position of the Node.
	 * \param x: The new x position.
	 * \param y: The new y position.
	 */
	void setPos(int x, int y);


	//TBD - unimplemented feature
	//void addConnection();


private:
	//The title of the Node
	std::string title;
	
	//The x,y and width, height of the Node
	SDL_Rect* shape;

	//The hovered state of the Node
	bool hovered_over = false;

	//The selected state of the Node
	bool is_selected = false;

	//The local path to the node's file
	std::string fname;


	//TBD - unimplemented feature
	//std::vector<Node*> connections;

};
