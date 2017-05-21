#pragma once

#include <SDL.h>
#include <string>

#include "basicgui.h"
#include "button.h"
#include "../util/point.h"
#include "../player/item.h"
#include "../core/animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @enum PopupType offers two opion on popup types
 * One has only one button while the other has two
 * The yes-no popup offers no special functionality, so it is never used yet
 */
enum class PopupType {
	POPUP_OK,
	POPUP_YESNO
};


/*!
 * @class Popup displays a popup which is closed by clicking the OK button
 * Supports displaying text and item textures
 */
class Popup : public BasicGUI {
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
	void setItemList(std::vector<Item*> newItemList);
	
	//Never use all of them
	Button* buttonOK;
	Button* buttonYES;
	Button* buttonNO;
	
	//Event handling
	void handleLeftClickEvent(int xp, int yp);
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
