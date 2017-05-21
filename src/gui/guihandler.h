#pragma once

#include <queue>

#include "basicgui.h"
#include "../core/battle.h"
#include "../util/stdextension.h"

/*!
 * @author kovlev
 */


/*!
 * @class GUIHandler manages the current visible gui
 * We can give it a gui item, and it displays it and possibly disables map interactions
 * Some gui's are harder to exit, for example we can not switch to our inventory during the battle
 * This is implemented by locking levels
 */
class GUIHandler {
public:
	GUIHandler();
	~GUIHandler();
	
	//Getters
	bool isLocked();
	bool isHardlocked();
	BasicGUI* getGUI();
	Battle* getBattle();
	
	//Setters
	void setGUI(BasicGUI* newGUI);
	void setBattle(Battle* newBattle);
	
	//Clears persisiting gui elements if we do not have any popups left
	void clear();
	//Removes all current persisting gui elements
	void hardClear();
	
	//Renders the current GUI
	void render();
	
	void addPopup(Popup* popupToAdd);
private:
	BasicGUI* currentGUI;
	Battle* currentBattle;
	
	//If we have a gui currently
	bool hasGUI;
	//If we can not leave the current gui (for example on battle)
	bool hardlocked;
	
	//Do we have current non popup gui in the guiqueue
	bool hasGUIInQueue;
	
	std::queue<BasicGUI*> popups;
};
