#pragma once

#include <SDL2/SDL.h>
#include <vector>

#include "button.h"
#include "wholescreengui.h"
#include "../player/inventory.h"
#include "../player/iteminfo.h"
#include "../player/unitinfo.h"
#include "../player/army.h"
#include "../map/minimap.h"
#include "../core/animatabletexture.h"


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
	bool getRenderDebugTickInfo();
	
	//Setters
	void setUpperHeight(int newUH);
	void setLowerHeight(int newLH);
	void setRenderDebugTickInfo(bool newRenderTickInfo);
private:
	int upperHeight;
	int lowerHeight;
	
	int heightLeftForMap;
	SDL_Rect possibleWholeScreenGUIDimensions;
	
	//Initializes everything
	void init();
	
	std::vector<Button*> buttons;
	
	//Debug allowing boolean
	bool renderDebugTickInfo;
	
	static void clickQuests();
	static void clickArmy();
	static void clickMinimap();
	
	static WholeScreenGUI* guiQuests;
	static WholeScreenGUI* guiArmy;
	static WholeScreenGUI* guiMinimap;
	
	static ClickedEntry clickedEntry;
};
