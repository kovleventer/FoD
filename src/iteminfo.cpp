#include "iteminfo.h"

#include "global.h"

ItemInfo::ItemInfo(int xp, int yp, int wp, int hp) : GUIPart(xp, yp, wp, hp) {
	selectedItem = NULL;
	bgTexture = Global::resourceHandler->getATexture(TT::GUI, "iteminfobg");
	descriptionSize = 24;
}

void ItemInfo::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, h};
	bgTexture->render(destinationRect);
	if (selectedItem != NULL) {
		//Renders the item on the right position
		selectedItem->render(x + w / 2 - Global::player->getInventory()->getSlotSize() / 2,
							 y + h / 4 - Global::player->getInventory()->getSlotSize() / 2,
							 Global::player->getInventory()->getSlotSize(),
							 Global::player->getInventory()->getSlotSize(),
							 false);
		for (unsigned int i = 0; i < selectedItem->getDescription().size(); i++) {
			//Renders description line by line
			ATexture* descText = Global::resourceHandler->getTextTexture(selectedItem->getDescription()[i], Global::resourceHandler->colors["iteminfo-desc"]);
			Dimension d = descText->getDimensions();
			d *= descriptionSize;
			d /= Global::defaultFontSize;
			destinationRect.x = x + w / 2 - d.W() / 2;
			destinationRect.y = y + h / 2 - d.H() / 2 + i * d.H();
			destinationRect.w = d.W();
			destinationRect.h = d.H();
			descText->render(destinationRect);
		}
	}
}

Item* ItemInfo::getItem() {
	return selectedItem;
}

int ItemInfo::getDescriptionSize() {
	return descriptionSize;
}

void ItemInfo::setItem(Item* newItem) {
	selectedItem = newItem;
}
