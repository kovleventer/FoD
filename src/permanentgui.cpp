#include "permanentgui.h"

#include "global.h"

PermanentGUI::PermanentGUI() {
	upperHeight = 30;
	lowerHeight = 100;
	init();
}

PermanentGUI::PermanentGUI(int uH, int lH) {
	upperHeight = uH;
	lowerHeight = lH;
	init();
}

PermanentGUI::~PermanentGUI() {
	for (unsigned int i = 0; i < buttons.size(); i++) {
		delete buttons[i];
	}
	
	delete PermanentGUI::guiQuests;
	delete PermanentGUI::guiArmy;
	delete PermanentGUI::guiMinimap;
}

void PermanentGUI::render() {
	SDL_Rect destinationRect;
	
	//lower
	destinationRect.x = 0;
	destinationRect.y = 0;
	destinationRect.w = Global::screenWidth;
	destinationRect.h = upperHeight;
	SDL_RenderCopy(Global::renderer, texture, NULL, &destinationRect);
	
	//upper
	destinationRect.y = Global::screenHeight - lowerHeight;
	destinationRect.h = lowerHeight;
	SDL_RenderCopy(Global::renderer, texture, NULL, &destinationRect);
	
	//buttons
	for (unsigned int i = 0; i < buttons.size(); i++) {
		buttons[i]->render();
	}
}

int PermanentGUI::getUpperHeight() {
	return upperHeight;
}

int PermanentGUI::getLowerHeight() {
	return lowerHeight;
}

int PermanentGUI::getHeightLeftForMap() {
	return heightLeftForMap;
}

std::vector<Button*> PermanentGUI::getButtons() {
	return buttons;
}

ClickedEntry PermanentGUI::getClickedEntry() {
	return PermanentGUI::clickedEntry;
}

SDL_Rect PermanentGUI::getDim() {
	return possibleWholeScreenGUIDimensions;
}

void PermanentGUI::setUpperHeight(int newUH) {
	upperHeight = newUH;
}

void PermanentGUI::setLowerHeight(int newLH) {
	lowerHeight = newLH;
}

void PermanentGUI::init() {
	heightLeftForMap = Global::screenHeight - upperHeight - lowerHeight;
	texture = Global::resourceHandler->guiTextures["permabg"];
	
	int tempX, tempY, tempW, tempH;
	if (heightLeftForMap > Global::screenWidth) {
		//If the screen's width is the smaller dimension
		tempX = 0;
		tempY = (heightLeftForMap - Global::screenWidth) / 2 + upperHeight;
		tempW = Global::screenWidth;
		tempH = Global::screenWidth;
	} else {
		//If the screen's height (-guisize) is the smaller dimension
		tempX = (Global::screenWidth - heightLeftForMap) / 2;
		tempY = upperHeight;
		tempW = heightLeftForMap;
		tempH = heightLeftForMap;
	}
	
	possibleWholeScreenGUIDimensions = {tempX, tempY, tempW, tempH};
	
	guiQuests = new WholeScreenGUI(tempX, tempY, tempW, tempH);
	guiArmy = new WholeScreenGUI(tempX, tempY, tempW, tempH);
	guiMinimap = new WholeScreenGUI(tempX, tempY, tempW, tempH);
	
	guiQuests->setHeaderText("Quests");
	guiArmy->setHeaderText("Player Army");
	guiMinimap->setHeaderText("Map");
	
	//Player's inventory (5*5)
	Inventory* tempInventory = new Inventory(tempX + tempW / 5, tempY + tempH / 10, tempW * 2 / 5, tempH * 2 / 5, 5, 10);
	guiArmy->addPart(tempInventory);
	Global::player->setInventory(tempInventory);
	
	ItemInfo* tempItemInfo = new ItemInfo(tempX + tempW * 3 / 5, tempY + tempH / 10, tempW * 2 / 5, tempH * 2 / 5);
	guiArmy->addPart(tempItemInfo);
	tempInventory->setItemInfo(tempItemInfo);
	
	//Player's army(5s*2)
	Army* tempArmy = new Army(tempX + tempW / 5, tempY + tempH / 2, tempW * 4 / 5, tempH / 2, 5, 2, false);
	guiArmy->addPart(tempArmy);
	Global::player->setArmy(tempArmy);
	
	UnitInfo* tempUnitInfo = new UnitInfo(tempX, tempY + tempH / 10, tempW / 5, tempH * 9 / 10);
	guiArmy->addPart(tempUnitInfo);
	tempArmy->setUnitInfo(tempUnitInfo);
	
	//Buttons init
	int buttonCount = 3;
	//Vertical and horizontal padding
	int paddingV = lowerHeight / 6;
	int paddingH = Global::screenWidth / (buttonCount * 2);
	int buttonHeight = lowerHeight * 2 / 3;
	int buttonWidth = (Global::screenWidth - (buttonCount + 1) * paddingH) / buttonCount;
	
	for (int i = 0; i < buttonCount; i++) {
		buttons.push_back(new Button(paddingH * (i + 1) + buttonWidth * i, Global::screenHeight - lowerHeight + paddingV, buttonWidth, buttonHeight));
	}
	
	buttons[0]->setOnClick(&PermanentGUI::clickQuests);
	buttons[1]->setOnClick(&PermanentGUI::clickArmy);
	buttons[2]->setOnClick(&PermanentGUI::clickMinimap);
}

void PermanentGUI::clickQuests() {
	if (PermanentGUI::clickedEntry == ClickedEntry::QUESTS) {
		Global::guiHandler->clear();
		clickedEntry = ClickedEntry::NONE;
	} else {
		Global::guiHandler->setGUI(PermanentGUI::guiQuests);
		clickedEntry = ClickedEntry::QUESTS;
	}
}

void PermanentGUI::clickArmy() {
	if (PermanentGUI::clickedEntry == ClickedEntry::ARMY) {
		Global::guiHandler->clear();
		clickedEntry = ClickedEntry::NONE;
	} else {
		Global::guiHandler->setGUI(PermanentGUI::guiArmy);
		clickedEntry = ClickedEntry::ARMY;
	}
}

void PermanentGUI::clickMinimap() {
	if (PermanentGUI::clickedEntry == ClickedEntry::MINIMAP) {
		Global::guiHandler->clear();
		clickedEntry = ClickedEntry::NONE;
	} else {
		Global::guiHandler->setGUI(PermanentGUI::guiMinimap);
		clickedEntry = ClickedEntry::MINIMAP;
	}
}

//static initializations
WholeScreenGUI* PermanentGUI::guiQuests = NULL;
WholeScreenGUI* PermanentGUI::guiArmy = NULL;
WholeScreenGUI* PermanentGUI::guiMinimap = NULL;
ClickedEntry PermanentGUI::clickedEntry = ClickedEntry::NONE;
