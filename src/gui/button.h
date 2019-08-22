#pragma once

#include <SDL.h>
#include <string>

#include "basicgui.h"
#include "../util/point.h"
#include "../core/animatabletexture.h"
#include "../util/pivot.h"

/*!
 * @author kovlev
 */


/*!
 * @class Button
 * A clickable button that support text over it
 * Clicking is done with a callback function and function pointers
 * @note This can be inconvinient to use, so a better alternative might should be designed
 */
class Button : public BasicGUI {
public:
	//https://stackoverflow.com/questions/5859455/how-to-implement-a-generic-button-class-that-executes-different-functions-on-cli
	//Magic stuff that makes the callback onclick function work
	typedef void(*eventFunction)();
	
	//Constructor that should be only used when creating dynamic positioned buttons
	Button();
	Button(int xp, int yp, int wp, int hp);
	
	//Button's texture
	ATexture* texture;
	
	//Renders the button
	void render();
	
	//Getters
	std::string getText();
	int getFontSize();
	
	//Setters
	void setText(std::string newText);
	void setFontSize(int newFontSize);
	
	//Callback function
	void click();
	
	//Sets a new function to be executed on click
	void setOnClick(eventFunction newOnClick);
private:
	eventFunction functionPointer;
	
	//The string to display on button
	std::string text;
	
	//The text's fontsize
	int fontSize;
};
