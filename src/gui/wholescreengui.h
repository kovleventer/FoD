#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

#include "basicgui.h"
#include "../core/animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @class WholeScreenGUI A type of gui, which have several parts and a header
 * A header contains the title of the gui
 * Simple parts are added to the gui and are not destroyed on WholeScreenGUI deletion
 * Temporary parts are destroyed
 */
class WholeScreenGUI : public BasicGUI {
public:
	WholeScreenGUI(int xp, int yp, int wp, int hp);
	WholeScreenGUI(SDL_Rect dimensionRect);
	~WholeScreenGUI();
	
	//Renders the gui and the parts in it
	void render();
	
	//Adds a part to the gui
	void addPart(BasicGUI* part);
	
	//Adds a temporary part to the gui
	void addTempPart(BasicGUI* part);
	
	//Getters
	std::string getHeaderText();
	int getHeaderSize();
	
	//Setters
	void setHeaderText(std::string newHeaderText);
	
	//The coordinates are NOT based on the GUI's position
	//0, 0 is at the top left corner of THE SCREEN
	void handleMousePressEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
protected:
	std::vector<BasicGUI*> parts;
	
	//Temporary parts are deleted on gui deletion
	std::vector<BasicGUI*> tempParts;
	
	//Th height of the top part of the gui
	int headerSize;
	
	//Base GUI textures
	ATexture* headerT;
	
	//Name
	std::string headerText;
};
