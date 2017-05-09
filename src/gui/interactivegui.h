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
#include "../iteminfo.h"
#include "../unit.h"
#include "../army.h"

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
 * @class TestGUIPart2 temporary gui class used on testing
 */
class TestGUIPart2 : public WholeScreenGUI {
public:
	TestGUIPart2(InteractiveGUI* parent);
};


/*!
 * @class TestGUIPart3 temporary gui class used on testing
 */
class TestGUIPart3 : public WholeScreenGUI {
public:
	TestGUIPart3(InteractiveGUI* parent);
};


class ItemCheckoutMenu; // Forward declaration


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
	ATexture* selectedItemBgText;
	
	//Renders the gui and the currently visible parts of the list
	void render();
	
	//Getters, setters for items
	Item* getItem(unsigned int index);
	int getItemPrice(unsigned int index);
	void addItem(Item* itemToAdd);
	void removeCurrentItem();
	
	//Getters
	int getFontSize();
	int getPadding();
	ItemInfo* getItemInfo();
	ItemCheckoutMenu* getItemCheckoutMenu();
	
	//Setters
	void setFontSize(int newFontSize);
	void setItemInfo(ItemInfo* newItemInfo);
	void setItemCheckoutMenu(ItemCheckoutMenu* newItemCheckoutMenu);
	
	//Event handling
	void handleMousePressEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	//An item and its price are packed into the std::pair
	std::vector< std::pair<Item*, int> > itemsToSell;
	
	//Item's height is itemSlotHeight - 2 * padding
	int padding;
	int itemSlotHeight;
	int numberOfDisplayableItems;
	int fontSize;
	
	unsigned int currentItemPosition;
	unsigned int selectedItemPosition;
	
	ItemInfo* itemInfo;
	ItemCheckoutMenu* itemCheckoutMenu;
};


/*!
 * @class ItemCheckoutMenu WIP class
 * Allows the actual buxing and selling of items
 */
class ItemCheckoutMenu : public BasicGUI {
public:
	ItemCheckoutMenu(int xp, int yp, int wp, int hp);
	ItemCheckoutMenu(SDL_Rect dimensionRect);
	~ItemCheckoutMenu();
	
	ATexture* bgText;
	ATexture* goldText;
	
	void render();
	
	//Getters
	Item* getCurrentItemToBuy();
	Item* getCurrentItemToSell();
	//NOTE Items and prices stored independently
	int getItemToBuyPrice();
	int getItemToSellPrice();
	int getPaddingH();
	int getPaddingV();
	int getRowHeight();
	ItemBuyingMenu* getItemBuyingMenu();
	
	//Setters
	void setCurrentItemToBuy(Item* newItemToBuy);
	void setCurrentItemToSell(Item* newItemToSell);
	void setItemToBuyPrice(int newItemToBuyPrice);
	void setItemToSellPrice(int newItemToBuyPrice);
	void setItemBuyingMenu(ItemBuyingMenu* newItemBuyingMenu);
	
	//Event handling
	void handleMousePressEvent(int xp, int yp);
private:
	Item* currentItemToBuy;
	Item* currentItemToSell;
	
	int horizontalPadding;
	int verticalPadding;
	//The remaining height for the three rows (calculated)
	int rowHeight;
	int fontSize;
	
	int itemToBuyPrice;
	int itemToSellPrice;
	
	Button* buyButton;
	Button* sellButton;
	
	ItemBuyingMenu* itemBuyingMenu;
};


/*!
 * @class UnitBuyingMenu handles unit buying options
 * Does NOT have an UnitCheckoutMenu, like its counterparts related to items has
 */
class UnitBuyingMenu : public BasicGUI {
public:
	UnitBuyingMenu(int xp, int yp, int wp, int hp);
	UnitBuyingMenu(SDL_Rect destinationRect);
	~UnitBuyingMenu();
	
	ATexture* bgText;
	
	void render();
	
	//Getters, setters for units
	Unit* getUnit(unsigned int index);
	int getUnitPrice(unsigned int index);
	void addUnit(Unit* unitToAdd);
	void removeUnit(unsigned int index);
	
	//Event handling
	void handleMousePressEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	std::vector< std::pair<Unit*, int> > unitsToSell;
	
	//The first rendered unit's index
	unsigned int currentUnitPosition;
	unsigned int numberOfDisplayableUnits;
	
	Button** buyButtons;
	
	//Recalculates gui element positions when needed
	void recalcPositions();
	int visibleUnitCount;
	int unitSize;
	int paddingH;
	int paddingV;
	
	//The price font size
	int fontSize;
};


/*!
 * @class Garrison if the player owes the world object he can place guard there
 * Stores units
 */
class Garrison : public BasicGUI {
public:
	Garrison(int xp, int yp, int wp, int hp);
	Garrison(SDL_Rect dimensionRect);
	~Garrison();
	
	void render();
	
	//Getters
	Army* getArmy();
	
	//Event handling
	void handleMousePressEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
private:
	Army* garrisonArmy;
};
