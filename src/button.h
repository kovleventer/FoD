#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "point.h"

class Button {
public:
	//https://stackoverflow.com/questions/5859455/how-to-implement-a-generic-button-class-that-executes-different-functions-on-cli
	//Magic stuff that makes the callback onclick function work
	typedef void(*eventFunction)();
	
	Button(int xp, int yp, int wp, int hp);
	
	SDL_Texture* texture;
	
	//Renders the button
	void render();
	
	//Getters
	int getX();
	int getY();
	int getW();
	int getH();
	std::string getText();
	
	//Setters
	void setX(int newX);
	void setY(int newY);
	void setW(int newW);
	void setH(int newH);
	void setText(std::string newText);
	
	//Checks whether the cursor is over the button
	bool isClicked(Point cursorPosition);
	
	//Callback function
	void click();
	
	//Sets a new function to be executed on click
	void setOnClick(eventFunction newOnClick);
private:
	int x, y, w, h;
	eventFunction functionPointer;
	
	std::string text;
};
