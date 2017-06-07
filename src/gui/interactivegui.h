#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include <utility>

#include "basicgui.h"
#include "../core/animatabletexture.h"
#include "wholescreengui.h"
#include "button.h"
#include "../player/item.h"
#include "../player/iteminfo.h"
#include "../player/unit.h"
#include "../player/army.h"
#include "../player/inventory.h"

class InteractiveWorldObject; // Forward declaration

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
	unsigned int getCurrentPartIndex();
	unsigned int getPartCount();
	WholeScreenGUI* getPart(unsigned int index);
	WholeScreenGUI* getCurrentPart();
	SDL_Rect getRemainingDim();
	InteractiveWorldObject* getParent();
	
	//Setters
	void setParent(InteractiveWorldObject* newParent);
	
	void addPart(std::pair<std::string, WholeScreenGUI*> newPart);
	
	//Event handling
	void handleLeftClickEvent(int xp, int yp);
	void handleRightClickEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	std::vector< std::pair<Button*, WholeScreenGUI*> > parts;
	
	int partChooserWidth;
	int padding;
	unsigned int currentPartIndex;
	InteractiveWorldObject* parent;
};


class ItemCheckoutMenu; // Forward declaration


/*!
 * @class ItemBuyingMenu
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
	
	//Refills items (copies itemsToSell to itemsToSellInvisible)
	void refill();
	
	//Getters, setters for items
	Item* getItem(unsigned int index);
	unsigned int getItemsToSellSize();
	void addItem(Item* itemToAdd);
	void removeCurrentItem();
	//Overwrites the current itemlist
	void setItemList(std::vector<Item*> newList);
	
	//Getters
	int getFontSize();
	int getPadding();
	ItemInfo* getItemInfo();
	ItemCheckoutMenu* getItemCheckoutMenu();
	
	//Setters
	void setFontSize(int newFontSize);
	void setItemInfo(ItemInfo* newItemInfo);
	void setItemCheckoutMenu(ItemCheckoutMenu* newItemCheckoutMenu);
	void setSelectedItemPosition(unsigned int newSelectedItemPosition);
	
	//Event handling
	void handleLeftClickEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	//An item contains its price value now
	std::vector<Item*> itemsToSell;
	std::vector<Item*> itemsToSellInvisible;
	
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
 * @class ItemCheckoutMenu
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
	int getPaddingH();
	int getPaddingV();
	int getRowHeight();
	ItemBuyingMenu* getItemBuyingMenu();
	
	//Setters
	void setCurrentItemToBuy(Item* newItemToBuy);
	void setCurrentItemToSell(Item* newItemToSell);
	void setItemBuyingMenu(ItemBuyingMenu* newItemBuyingMenu);
	
	//Event handling
	void handleLeftClickEvent(int xp, int yp);
private:
	Item* currentItemToBuy;
	Item* currentItemToSell;
	
	int horizontalPadding;
	int verticalPadding;
	//The remaining height for the three rows (calculated)
	int rowHeight;
	int fontSize;
	
	Button* buyButton;
	Button* sellButton;
	
	ItemBuyingMenu* itemBuyingMenu;
	
	//You ain't gonna sell the itemfor the same price as you bougth it
	double taxRatio;
};


/*!
 * @class UnitBuyingMenu handles unit buying options
 * Does NOT have an UnitCheckoutMenu, like its counterpart related to items has
 */
class UnitBuyingMenu : public BasicGUI {
public:
	UnitBuyingMenu(int xp, int yp, int wp, int hp);
	UnitBuyingMenu(SDL_Rect destinationRect);
	~UnitBuyingMenu();
	
	ATexture* bgText;
	
	void render();
	
	void refill();
	
	//Getters, setters for units
	Unit* getUnit(unsigned int index);
	unsigned int getUnitsToSellSize();
	void addUnit(Unit* unitToAdd);
	void removeUnit(unsigned int index);
	void setUnitList(std::vector<std::string> newList);
	
	//Event handling
	void handleLeftClickEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	std::vector<Unit*> unitsToSell;
	std::vector<std::string> unitsToSellInvisible;
	
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
	
	//Used when the structure's owner changes without a battle
	//In that case, the units in garrison are deleted
	void recreateGarrisonArmy();
	
	//Event handling
	void handleLeftClickEvent(int xp, int yp);
	void handleMouseMotionEvent(int xp, int yp);
private:
	Army* garrisonArmy;
};


/*!
 * @class ItemMarket holds all item buying related classes
 */
class ItemMarket : public WholeScreenGUI {
public:
	ItemMarket(InteractiveGUI* parent);
	
	//Getters
	ItemBuyingMenu* getItemBuyingMenu();
	ItemCheckoutMenu* getItemCheckoutMenu();
	ItemInfo* getItemInfo();
	Inventory* getInventory();
private:
	ItemBuyingMenu* itemBuyingMenu;
	ItemCheckoutMenu* itemCheckoutMenu;
	ItemInfo* itemInfo;
	//Player's inventory
	Inventory* inventory;
};


/*!
 * @class Barracks holds all unit buying classes
 */
class Barracks : public WholeScreenGUI {
public:
	Barracks(InteractiveGUI* parent);
	
	//Getters
	UnitBuyingMenu* getUnitBuyingMenu();
	Army* getArmy();
private:
	UnitBuyingMenu* unitBuyingMenu;
	//Player's army
	Army* army;
};


/*!
 * @class GarrisonWrapper holds the garrison and the player's army
 */
class GarrisonWrapper : public WholeScreenGUI {
public:
	GarrisonWrapper(InteractiveGUI* parent);
	
	//Getters
	Garrison* getGarrison();
	Army* getArmy();
private:
	Garrison* garrison;
	//Player's army
	Army* army;
};
