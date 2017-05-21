#include "questdisplay.h"

#include "../core/global.h"

QuestLister::QuestLister(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	headerSize = 35;
	//-1 due to the header
	displayableQuestCount = h / headerSize - 1;
	padding = 5;
	selectedElementPos = 0;
	defaultBGTexture = Global::resourceHandler->getATexture(TT::GUI, "questlistdefaultbg");
	headBGTexture = Global::resourceHandler->getATexture(TT::GUI, "questlistheadbg");
	selectedBGTexture = Global::resourceHandler->getATexture(TT::GUI, "questlistselectedbg");
}

void QuestLister::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, headerSize};
	headBGTexture->render(destinationRect);
	
	ATexture* headerText = Global::resourceHandler->getTextTexture("Active quests", Global::resourceHandler->colors["iteminfo-desc"]);
	Dimension d = headerText->getDimensions();
	d *= headerSize * 3 / 4;
	d /= Global::defaultFontSize;
	SDL_Rect textDestRect = destinationRect;
	textDestRect.x += padding;
	textDestRect.y += padding;
	textDestRect.w = d.W();
	textDestRect.h = d.H();
	headerText->render(textDestRect);
	
	for (int i = 0; i < displayableQuestCount; i++) {
		destinationRect.y += headerSize;
		defaultBGTexture->render(destinationRect);
		
		Quest* currentQuest = getQuest(i);
		if (currentQuest != NULL) {
			ATexture* headerText = Global::resourceHandler->getTextTexture(currentQuest->getName(), Global::resourceHandler->colors["iteminfo-desc"]);
			Dimension d = headerText->getDimensions();
			d *= headerSize * 2 / 3;
			d /= Global::defaultFontSize;
			textDestRect = destinationRect;
			textDestRect.x += padding;
			textDestRect.y += padding;
			textDestRect.w = d.W();
			textDestRect.h = d.H();
			headerText->render(textDestRect);
		}
	}
}

Quest* QuestLister::getQuest(unsigned int index) {
	//NULL checking
	if (index >= questsToDisplay.size()) {
		return NULL;
	}
	return questsToDisplay[index];
}

int QuestLister::getHeaderSize() {
	return headerSize;
}

int QuestLister::getPadding() {
	return padding;
}

void QuestLister::addQuest(Quest* questToAdd) {
	questsToDisplay.push_back(questToAdd);
}

void QuestLister::setPadding(int newPadding) {
	padding = newPadding;
}
