#include "questdisplay.h"

#include "../core/global.h"

//QuestLister stuff

QuestLister::QuestLister(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	headerSize = 35;
	//-1 due to the header
	displayableQuestCount = h / headerSize - 1;
	padding = 5;
	selectedElementPos = 0;
	defaultBGTexture = Global::resourceHandler->getATexture(TT::GUI, "questlistdefaultbg");
	headBGTexture = Global::resourceHandler->getATexture(TT::GUI, "questlistheadbg");
	selectedBGTexture = Global::resourceHandler->getATexture(TT::GUI, "questlistselectedbg");
	questDisplay = NULL;
}

void QuestLister::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, headerSize};
	headBGTexture->render(destinationRect);
	
	ATexture* headerText = Global::resourceHandler->getTextTexture("Active quests", Global::resourceHandler->getColor("iteminfo-desc"), headerSize * 3 / 4);
	Dimension d = headerText->getDimensions();
	SDL_Rect textDestRect = destinationRect;
	textDestRect.x += padding;
	textDestRect.y += padding;
	textDestRect.w = d.W();
	textDestRect.h = d.H();
	headerText->render(textDestRect);
	
	for (int i = 0; i < displayableQuestCount; i++) {
		destinationRect.y += headerSize;
		if (selectedElementPos == i) {
			selectedBGTexture->render(destinationRect);
		} else {
			defaultBGTexture->render(destinationRect);
		}
		
		Quest* currentQuest = getQuest(i);
		if (currentQuest != NULL) {
			ATexture* headerText = Global::resourceHandler->getTextTexture(currentQuest->getName(), Global::resourceHandler->getColor("iteminfo-desc"), headerSize * 2 / 3);
			Dimension d = headerText->getDimensions();
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

QuestDisplay* QuestLister::getQuestDisplay() {
	return questDisplay;
}

void QuestLister::addQuest(Quest* questToAdd) {
	questsToDisplay.push_back(questToAdd);
}

void QuestLister::setPadding(int newPadding) {
	padding = newPadding;
}

void QuestLister::setQuestDisplay(QuestDisplay* newQuestDisplay) {
	questDisplay = newQuestDisplay;
}

void QuestLister::handleLeftClickEvent(int xp, int yp) {
	selectedElementPos = (yp - y) / headerSize - 1;
	Quest* currentQuest = getQuest(selectedElementPos);
	questDisplay->setQuest(currentQuest);
}

void QuestLister::handleMouseWheelEvent(bool up) {
	//TODO implementation
}

//QuestDisplay stuff

QuestDisplay::QuestDisplay(int xp, int yp, int wp, int hp) : BasicGUI(xp, yp, wp, hp) {
	currentQuest = NULL;
	bgTexture = Global::resourceHandler->getATexture(TT::GUI, "questlistselectedbg");
	titleSize = 36;
	descSize = 22;
	padding = 12;
}

void QuestDisplay::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, h};
	bgTexture->render(destinationRect);
	
	if (currentQuest != NULL) {
		SDL_Rect textDestRect;
		
		ATexture* titleText = Global::resourceHandler->getTextTexture(currentQuest->getName(), Global::resourceHandler->getColor("iteminfo-desc"), titleSize);
		Dimension d = titleText->getDimensions();
		textDestRect = destinationRect;
		textDestRect.x += (textDestRect.w - d.W()) / 2;
		textDestRect.w = d.W();
		textDestRect.h = d.H();
		titleText->render(textDestRect);
		
		ATexture* descText = Global::resourceHandler->getTextTexture(currentQuest->getDescription(), Global::resourceHandler->getColor("iteminfo-desc"), descSize);
		d = descText->getDimensions();
		textDestRect = destinationRect;
		textDestRect.x += padding;
		textDestRect.y += textDestRect.h / 6;
		textDestRect.w = d.W();
		textDestRect.h = d.H();
		descText->render(textDestRect);
		
		int size = 100;
		
		ATexture* objText = NULL;
		ATexture* oText = NULL;
		switch(currentQuest->getQuestObjective()) {
			case QuestObjective::TALK_WITH_NPC:
				objText = Global::resourceHandler->getTextTexture("Talk to:", Global::resourceHandler->getColor("iteminfo-desc"), descSize);
				oText = currentQuest->getQOTalkTarget()->texture;
				break;
			case QuestObjective::KILL_NPC:
				objText = Global::resourceHandler->getTextTexture("Kill:", Global::resourceHandler->getColor("iteminfo-desc"), descSize);
				oText = currentQuest->getQOKillTarget()->texture;
				break;
			case QuestObjective::VISIT_STRUCTURE:
				objText = Global::resourceHandler->getTextTexture("Visit:", Global::resourceHandler->getColor("iteminfo-desc"), descSize);
				oText = currentQuest->getQOStructTarget()->texture;
				break;
		}
		d = objText->getDimensions();
		textDestRect = destinationRect;
		textDestRect.x += padding;
		textDestRect.y += textDestRect.h * 2 / 6;
		textDestRect.w = d.W();
		textDestRect.h = d.H();
		objText->render(textDestRect);
		oText->render({x + textDestRect.w + padding * 4, textDestRect.y, size, size});
		
		ATexture* rewardText = Global::resourceHandler->getTextTexture("Reward:", Global::resourceHandler->getColor("iteminfo-desc"), descSize);
		d = rewardText->getDimensions();
		textDestRect = destinationRect;
		textDestRect.x += padding;
		textDestRect.y += textDestRect.h * 3 / 6;
		textDestRect.w = d.W();
		textDestRect.h = d.H();
		rewardText->render(textDestRect);
		Global::resourceHandler->getATexture(TT::GUI, "gold")->render({x + textDestRect.w + padding * 4, textDestRect.y, size, size});
		ATexture* goldText = Global::resourceHandler->getTextTexture(std::to_string(currentQuest->getRewardGold()), Global::resourceHandler->getColor("gold"), descSize * 3);
		d = goldText->getDimensions();
		textDestRect.x += textDestRect.w + size + padding * 6;
		textDestRect.w = d.W();
		textDestRect.h = d.H();
		goldText->render(textDestRect);
		
		int rowLength = w / size;
		//int rows = (int)std::ceil((double)currentQuest->getRewardItemsSize() / rowLength);
		for (unsigned int i = 0; i < currentQuest->getRewardItemsSize(); i++) {
			destinationRect.x = x + (i % rowLength) * size;
			destinationRect.y = y + h * 4 / 6 + ((i / rowLength) * size);
			destinationRect.w = size;
			destinationRect.h = size;
			currentQuest->getRewardItem(i)->render(destinationRect, false);
		}
	}
}

Quest* QuestDisplay::getQuest() {
	return currentQuest;
}

void QuestDisplay::setQuest(Quest* newQuest) {
	currentQuest = newQuest;
}
