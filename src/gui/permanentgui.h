#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "button.h"
#include "wholescreengui.h"
#include "../inventory.h"
#include "../iteminfo.h"
#include "../unitinfo.h"
#include "../army.h"
#include "../minimap.h"
#include "../animatabletexture.h"


/*!
 * @enum ClickedEntry Which of the three entries are clicked on the permanent gui
 */
enum class ClickedEntry {
	NONE,
	QUESTS,
	ARMY,
	MINIMAP
};


/*!
 * @class PermanentGUI this class represents the alaways visible parts of the gui
 * The brown stripes at the top and bottom of the screen are supposed to be this
 * Has buttons, and player info is available by these
 * Displays current gold
 */
class PermanentGUI {
public:
	PermanentGUI();
	PermanentGUI(int uH, int lH);
	
	//Deletes the buttons
	~PermanentGUI();
	
	ATexture* texture;
	
	ATexture* goldTexture;
	
	//Renders the permagui
	void render();
	
	//Getters
	int getUpperHeight();
	int getLowerHeight();
	int getHeightLeftForMap();
	std::vector<Button*> getButtons();
	static ClickedEntry getClickedEntry();
	SDL_Rect getDim();
	
	//Setters
	void setUpperHeight(int newUH);
	void setLowerHeight(int newLH);
private:
	int upperHeight;
	int lowerHeight;
	
	int heightLeftForMap;
	SDL_Rect possibleWholeScreenGUIDimensions;
	
	//Initializes everything
	void init();
	
	std::vector<Button*> buttons;
	
	static void clickQuests();
	static void clickArmy();
	static void clickMinimap();
	
	static WholeScreenGUI* guiQuests;
	static WholeScreenGUI* guiArmy;
	static WholeScreenGUI* guiMinimap;
	
	static ClickedEntry clickedEntry;
};
