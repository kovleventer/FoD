#pragma once

#include <SDL2/SDL.h>

#include "guipart.h"
#include "item.h"

class ItemInfo : public GUIPart {
public:
	ItemInfo(int xp, int yp, int wp, int hp);
	
	SDL_Texture* bgTexture;
	
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
