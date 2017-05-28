#include "permanentgui.h"

#include "../core/global.h"

/*!
 * @author kovlev
 */

PermanentGUI::PermanentGUI() : PermanentGUI(30, 100) {}

PermanentGUI::PermanentGUI(int uH, int lH) {
	upperHeight = uH;
	lowerHeight = lH;
	
	heightLeftForMap = Global::screenHeight - upperHeight - lowerHeight;
	texture = Global::resourceHandler->getATexture(TT::GUI, "permabg");
	goldTexture = Global::resourceHandler->getATexture(TT::GUI, "gold");
	renderDebugTickInfo = false;
	
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
	
	//QuestLister
	QuestLister* tempQuestLister = new QuestLister(tempX + tempW * 3 / 4, tempY + tempH / 10, tempW / 4, tempH * 9 / 10);
	guiQuests->addPart(tempQuestLister);
	Global::player->setQuestLister(tempQuestLister);
	
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
	
	//Minimap
	Minimap* tempMinimap = new Minimap(tempX + tempH / 20, tempY + tempH / 10, tempW * 9 / 10, tempH * 9 / 10);
	guiMinimap->addPart(tempMinimap);
	Global::minimap = tempMinimap;
	
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
	
	
}

void PermanentGUI::initButtonCallbacks() {
	//Lambda magic
	//There may be a better way of passing anonymus functions as callbacks
	buttons[0]->setOnClick([] {
		if (Global::permaGUI->getClickedEntry() == ClickedEntry::QUESTS) {
			Global::guiHandler->clear();
			Global::permaGUI->setClickedEntry(ClickedEntry::NONE);
		} else {
			Global::guiHandler->clear();
			Global::guiHandler->setGUI(Global::permaGUI->getGuiQuests());
			Global::permaGUI->setClickedEntry(ClickedEntry::QUESTS);
		}
	});
	
	buttons[1]->setOnClick([] {
		if (Global::permaGUI->getClickedEntry() == ClickedEntry::ARMY) {
			Global::guiHandler->clear();
			Global::permaGUI->setClickedEntry(ClickedEntry::NONE);
		} else {
			Global::guiHandler->clear();
			Global::guiHandler->setGUI(Global::permaGUI->getGuiArmy());
			Global::permaGUI->setClickedEntry(ClickedEntry::ARMY);
		}
	});
	
	buttons[2]->setOnClick([] {
		if (Global::permaGUI->getClickedEntry() == ClickedEntry::MINIMAP) {
			Global::guiHandler->clear();
			Global::permaGUI->setClickedEntry(ClickedEntry::NONE);
		} else {
			Global::guiHandler->clear();
			Global::guiHandler->setGUI(Global::permaGUI->getGuiMinimap());
			Global::permaGUI->setClickedEntry(ClickedEntry::MINIMAP);
		}
	});
}

PermanentGUI::~PermanentGUI() {
	stdex::clear_ptr_vec(buttons);
	
	delete guiQuests;
	delete guiArmy;
	delete guiMinimap;
}

void PermanentGUI::render() {
	SDL_Rect destinationRect;
	
	//lower
	destinationRect.x = 0;
	destinationRect.y = 0;
	destinationRect.w = Global::screenWidth;
	destinationRect.h = upperHeight;
	texture->render(destinationRect);
	
	//upper
	destinationRect.y = Global::screenHeight - lowerHeight;
	destinationRect.h = lowerHeight;
	texture->render(destinationRect);
	
	//buttons
	for (unsigned int i = 0; i < buttons.size(); i++) {
		buttons[i]->render();
	}
	
	//gold indicator for player
	ATexture* goldText = Global::resourceHandler->getTextTexture(std::to_string(Global::player->getGold()), Global::resourceHandler->getColor("gold"));
	Dimension d = goldText->getDimensions();
	d *= upperHeight;
	d /= Global::defaultFontSize;
	destinationRect.x = Global::screenWidth / 2 - (d.W() / 2 + upperHeight / 2) + upperHeight;
	//Small corrigation
	destinationRect.y = -upperHeight / 5;
	destinationRect.w = d.W();
	destinationRect.h = d.H();
	goldText->render(destinationRect);
	
	destinationRect.x = Global::screenWidth / 2 - (d.W() / 2 + upperHeight / 2);
	destinationRect.y = 0;
	destinationRect.w = upperHeight;
	destinationRect.h = upperHeight;
	goldTexture->render(destinationRect);
	
	if (renderDebugTickInfo) {
		//Renders current gameticks to the top-left corner of the screen
		ATexture* gameTicksText = Global::resourceHandler->getTextTexture(std::to_string(Global::tickHandler->getGameTicks()), Global::resourceHandler->getColor("gold"));
		d = gameTicksText->getDimensions();
		d *= upperHeight;
		d /= Global::defaultFontSize;
		destinationRect.x = 0;
		//Small corrigation
		destinationRect.y = -upperHeight / 5;
		destinationRect.w = d.W();
		destinationRect.h = d.H();
		gameTicksText->render(destinationRect);
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
	return clickedEntry;
}

SDL_Rect PermanentGUI::getDim() {
	return possibleWholeScreenGUIDimensions;
}

bool PermanentGUI::getRenderDebugTickInfo() {
	return renderDebugTickInfo;
}

WholeScreenGUI* PermanentGUI::getGuiQuests() {
	return guiQuests;
}

WholeScreenGUI* PermanentGUI::getGuiArmy() {
	return guiArmy;
}

WholeScreenGUI* PermanentGUI::getGuiMinimap() {
	return guiMinimap;
}

void PermanentGUI::setUpperHeight(int newUH) {
	upperHeight = newUH;
}

void PermanentGUI::setLowerHeight(int newLH) {
	lowerHeight = newLH;
}

void PermanentGUI::setClickedEntry(ClickedEntry newClickedEntry) {
	clickedEntry = newClickedEntry;
}

void PermanentGUI::setRenderDebugTickInfo(bool newRenderTickInfo) {
	renderDebugTickInfo = newRenderTickInfo;
}
