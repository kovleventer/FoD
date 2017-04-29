#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "point.h"
#include "item.h"

class Cursor {
public:
	//Init
	Cursor(std::string text);
	Cursor(SDL_Texture* text);
	
	SDL_Texture* texture;
	
	//Renders the cursor
	void render();
	
	//Updates the cursor's texture and position
	void update();
	
	//Getters
	Point getPosition();
	Item* getItem();
	
	//Setters
	void setItem(Item* newItem);
private:
	void init();
	
	//We do not have a setter for the position
	Point position;
	
	//If we are currently dragging an item
	Item* itemOnCursor;
};
