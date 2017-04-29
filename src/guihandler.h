#pragma once

#include "transientgui.h"

class GUIHandler {
public:
	GUIHandler();
	
	//Getters
	bool isLocked();
	bool isHardlocked();
	TransientGUI* getGUI();
	
	//Setters
	void setGUI(TransientGUI* newGUI);
	void setHardlocked(bool newHardlocked);
	
	void clear();
	
	//Renders the current GUI
	void render();
private:
	TransientGUI* currentGUI;
	
	//If we have a gui currently
	bool hasGUI;
	//If we can not leave the current gui (for example on battle)
	bool hardlocked;
};
