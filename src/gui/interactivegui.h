#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <utility>

#include "basicgui.h"
#include "../animatabletexture.h"
#include "wholescreengui.h"
#include "button.h"
#include "../item.h"

/*!
 * @author kovlev
 */


/*!
 * @class InteractiveGUI A type of gui, which consist of multiple wholexscreenguis
 * The default template for this type is:
 * 	A small meno on the left side for selecting which WholeScreenGUI to display
 * 	The current displayed WholeScreenGUI on the right side
 * 	Selection is done by mouse press events
 */
class InteractiveGUI : public BasicGUI {
public:
	InteractiveGUI(int xp, int yp, int wp, int hp);
	InteractiveGUI(SDL_Rect dimensionRect);
	~InteractiveGUI();
	
	ATexture* chooserText;
	
	void render();
	
	//Getters
	int getPartChooserWidth();
	int getPadding();
	SDL_Rect getRemainingDim();
	
	void addPart(std::pair<std::string, WholeScreenGUI*> newPart);
	
	//Event handling
	void handleMousePressEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	std::vector< std::pair<Button*, WholeScreenGUI*> > parts;
	
	int partChooserWidth;
	int padding;
	unsigned int currentPartIndex;
};


/*!
 * @class TestGUIPart temporary gui class used on testing
 */
class TestGUIPart : public WholeScreenGUI {
public:
	TestGUIPart(InteractiveGUI* parent);
};


/*!
 * @class ItemBuyingMenu WIP class
 * We will able to buy and sell items
 * Now it is only capable of displaying a list of items
 */
class ItemBuyingMenu : public BasicGUI {
public:
	ItemBuyingMenu(int xp, int yp, int wp, int hp);
	ItemBuyingMenu(SDL_Rect dimensionRect);
	
	ATexture* itemBgText;
	
	//Renders the gui and the currently visible parts of the list
	void render();
	
	//Getters, setters for items
	Item* getItem(unsigned int index);
	void addItem(Item* itemToAdd);
	
	//Event handling
	void handleMousePressEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	std::vector<Item*> itemsToSell;
	
	//Item's height is itemSlotHeight - 2 * padding
	int padding;
	int itemSlotHeight;
	int numberOfDisplayableItems;
	unsigned int currentItemPosition;
};
