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
	
	//Setters
	void addQuest(Quest* questToAdd);
	void setPadding(int newPadding);
private:
	std::vector<Quest*> questsToDisplay;
	
	int headerSize;
	int padding;
	int displayableQuestCount;
	int selectedElementPos;
};
