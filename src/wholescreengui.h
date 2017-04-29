#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "transientgui.h"
#include "guipart.h"

class WholeScreenGUI : public TransientGUI {
public:
	WholeScreenGUI(int xp, int yp, int wp, int hp);
	WholeScreenGUI(SDL_Rect dimensionRect);
	~WholeScreenGUI();
	
	//Renders the gui and the parts in it
	void render();
	
	//Adds a part to the gui
	void addPart(GUIPart* part);
	
	//Getters
	std::string getHeaderText();
	
	//Setters
	void setHeaderText(std::string newHeaderText);
	
	//The coordinates are based on the GUI's position
	//0, 0 is at the top legt corner of our GUI
	void handleMousePressEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	std::vector<GUIPart*> parts;
	
	//Th height of the top part of the gui
	int headerSize;
	
	//Base GUI textures
	SDL_Texture* headerT;
	
	//Name
	std::string headerText;
};
