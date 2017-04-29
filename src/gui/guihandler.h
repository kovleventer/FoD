#pragma once

#include "basicgui.h"
#include "../battle.h"

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
	
	//Getters
	bool isLocked();
	bool isHardlocked();
	BasicGUI* getGUI();
	Battle* getBattle();
	
	//Setters
	void setGUI(BasicGUI* newGUI);
	void setBattle(Battle* newBattle);
	
	//Removes all current persisting gui elements
	void clear();
	
	//Renders the current GUI
	void render();
private:
	BasicGUI* currentGUI;
	Battle* currentBattle;
	
	//If we have a gui currently
	bool hasGUI;
	//If we can not leave the current gui (for example on battle)
	bool hardlocked;
};
