#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "basicgui.h"
#include "../util/point.h"
#include "../animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @class Button A clickable button that support text over it
 * Clicking is done with a callback function and function pointers
 * @note This can be inconvinient to use, so a better alternative might should be designed
 */
class Button : public BasicGUI {
public:
	//https://stackoverflow.com/questions/5859455/how-to-implement-a-generic-button-class-that-executes-different-functions-on-cli
	//Magic stuff that makes the callback onclick function work
	typedef void(*eventFunction)();
	
	Button(int xp, int yp, int wp, int hp);
	
	//Button's texture
	ATexture* texture;
	
	//Renders the button
	void render();
	
	//Getters
	std::string getText();
	
	//Setters
	void setText(std::string newText);
	
	//Callback function
	void click();
	
	//Sets a new function to be executed on click
	void setOnClick(eventFunction newOnClick);
private:
	eventFunction functionPointer;
	
	std::string text;
};
