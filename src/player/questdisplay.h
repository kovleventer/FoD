#pragma once

#include <SDL.h>
#include <vector>

#include "../gui/basicgui.h"
#include "quest.h"
#include "../core/animatabletexture.h"
#include "../util/point.h"

/*!
 * @author kovlev
 */

class QuestDisplay;

/*!
 * @class QuestLister
 * Displays all current quests by listing them
 */
class QuestLister : public BasicGUI {
public:
	QuestLister(int xp, int yp, int wp, int hp);
	
	ATexture* defaultBGTexture;
	ATexture* headBGTexture;
	ATexture* selectedBGTexture;
	
	//Renders the quest lister
	void render();
	
	//Getters
	Quest* getQuest(unsigned int index);
	int getHeaderSize();
	int getPadding();
	QuestDisplay* getQuestDisplay();
	
	//Setters
	void addQuest(Quest* questToAdd);
	void setPadding(int newPadding);
	void setQuestDisplay(QuestDisplay* newQuestDisplay);
	
	//Event handlers
	void handleLeftClickEvent(int xp, int yp);
	void handleMouseWheelEvent(bool up);
private:
	std::vector<Quest*> questsToDisplay;
	QuestDisplay* questDisplay;
	
	int headerSize;
	int padding;
	int displayableQuestCount;
	int selectedElementPos;
};


/*!
 * @class QuestDisplay
 * Displays the properties of a selected Quest
 */
class QuestDisplay : public BasicGUI {
public:
	QuestDisplay(int xp, int yp, int wp, int hp);
	
	ATexture* bgTexture;
	
	void render();
	
	//Getters
	Quest* getQuest();
	
	//Setters
	void setQuest(Quest* newQuest);
private:
	Quest* currentQuest;
	
	int titleSize;
	int descSize;
	int padding;
};
