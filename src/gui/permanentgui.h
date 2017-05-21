#pragma once

#include <SDL.h>
#include <vector>

#include "button.h"
#include "wholescreengui.h"
#include "../player/inventory.h"
#include "../player/iteminfo.h"
#include "../player/unitinfo.h"
#include "../player/army.h"
#include "../map/minimap.h"
#include "../core/animatabletexture.h"
#include "../util/stdextension.h"


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
	
	//Must run after the constructor is done
	void initButtonCallbacks();
	
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
	ClickedEntry getClickedEntry();
	SDL_Rect getDim();
	bool getRenderDebugTickInfo();
	WholeScreenGUI* getGuiQuests();
	WholeScreenGUI* getGuiArmy();
	WholeScreenGUI* getGuiMinimap();
	
	//Setters
	void setUpperHeight(int newUH);
	void setLowerHeight(int newLH);
	void setClickedEntry(ClickedEntry newClickedEntry);
	void setRenderDebugTickInfo(bool newRenderTickInfo);
private:
	int upperHeight;
	int lowerHeight;
	
	int heightLeftForMap;
	SDL_Rect possibleWholeScreenGUIDimensions;
	
	std::vector<Button*> buttons;
	
	//Debug allowing boolean
	bool renderDebugTickInfo;
	
	static void clickQuests();
	static void clickArmy();
	static void clickMinimap();
	
	WholeScreenGUI* guiQuests;
	WholeScreenGUI* guiArmy;
	WholeScreenGUI* guiMinimap;
	
	ClickedEntry clickedEntry;
};
