#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "util/point.h"
#include "item.h"
#include "animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @class Cursor has a position and a texture
 * We do not use the os's default cursor
 * The cursor can store an item sometimes
 */
class Cursor {
public:
	//Init
	Cursor(std::string text);
	Cursor(ATexture* text);
	
	ATexture* texture;
	
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
