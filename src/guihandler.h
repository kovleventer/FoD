#pragma once

#include "transientgui.h"
#include "battle.h"

class GUIHandler {
public:
	GUIHandler();
	
	//Getters
	bool isLocked();
	bool isHardlocked();
	TransientGUI* getGUI();
	Battle* getBattle();
	
	//Setters
	void setGUI(TransientGUI* newGUI);
	void setBattle(Battle* newBattle);
	
	void clear();
	
	//Renders the current GUI
	void render();
private:
	TransientGUI* currentGUI;
	Battle* currentBattle;
	
	//If we have a gui currently
	bool hasGUI;
	//If we can not leave the current gui (for example on battle)
	bool hardlocked;
};
