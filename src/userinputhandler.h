#pragma once

#include <SDL2/SDL.h>

#include "point.h"

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
	
	//NOTE might need mousedrag event
};
