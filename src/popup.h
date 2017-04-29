#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "transientgui.h"
#include "button.h"
#include "point.h"
#include "item.h"
#include "animatabletexture.h"

enum class PopupType {
	POPUP_OK,
	POPUP_YESNO
};

class Popup : public TransientGUI {
public:
	//Custom position popups
	Popup(int xp, int yp, int wp, int hp, PopupType type);
	Popup(SDL_Rect dimensionRect, PopupType type);
	//Auto-center popupss
	Popup(int width, int height, PopupType type);
	Popup(Point dimension, PopupType type);
	~Popup();
	
	void render();
	
	ATexture* backgroundT;
	ATexture* foregroundT;
	
	//Getters
	PopupType getPopupType();
	std::string getText();
	Item* getItem(int index);
	
	//Setters
	//NOTE dont use this
	void setPopupType(PopupType newPopupType);
	void setText(std::string newText);
	void addItem(Item* itemToAdd);
	
	//Never use all of them
	Button* buttonOK;
	Button* buttonYES;
	Button* buttonNO;
private:
	//The width and the height of the used buttons
	Point buttonDimensions;
	
	//Padding of the text area
	int padding;
	
	//The type of the popup
	PopupType popupType;
	
	std::string text;
	
	std::vector<Item*> items;
};
