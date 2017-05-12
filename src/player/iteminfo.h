#pragma once

#include <SDL2/SDL.h>

#include "../gui/basicgui.h"
#include "item.h"
#include "../core/animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @class BasicGUI a gui element which displays information of a given item
 * List all the items' stats
 */
class ItemInfo : public BasicGUI {
public:
	ItemInfo(int xp, int yp, int wp, int hp);
	
	ATexture* bgTexture;
	
	void render();
	
	//Getters
	Item* getItem();
	int getDescriptionSize();
	
	//Setters
	void setItem(Item* newItem);
private:
	//The current item renderend on the iteminfo
	Item* selectedItem;
	
	int descriptionSize;
};
