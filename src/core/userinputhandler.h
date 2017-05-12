#pragma once

#include <SDL2/SDL.h>

#include "../util/point.h"

/*!
 * @author kovlev
 */


/*!
 * @class UserInputHandler contains static methods which are used in eventhandling
 * Handles map events right here
 * Passes events to the current gui when possible
 */
class UserInputHandler {
public:
	//The exact moment when the user pushes the key down
	static void handleKeyPressEvent(SDL_Event e);
	
	//The whole time while the key is being down
	static void handleKeyDownEvent(const Uint8* keyboardState);
	
	//When the mouse button is pressed
	static void handleMousePressEvent(SDL_Event e);
	
	//When the mouse button is released
	static void handleMouseReleaseEvent(SDL_Event e);
	
	//When the mouse is moved
	static void handleMouseMotionEvent(int x, int y);
	
	//When the mouse wheel is triggered
	static void handleMouseWheelEvent(bool up);
};
