#include "iteminfo.h"

#include "global.h"

ItemInfo::ItemInfo(int xp, int yp, int wp, int hp) : GUIPart(xp, yp, wp, hp) {
	selectedItem = NULL;
	bgTexture = Global::resourceHandler->guiTextures["iteminfobg"];
	descriptionSize = 24;
}

void ItemInfo::render() {
	//Setting rectangle
	SDL_Rect destinationRect = {x, y, w, h};
	SDL_RenderCopy(Global::renderer, bgTexture, NULL, &destinationRect);
	if (selectedItem != NULL) {
		//Renders the item on the right position
		selectedItem->render(x + w / 2 - Global::player->getInventory()->getSlotSize() / 2,
							 y + h / 4 - Global::player->getInventory()->getSlotSize() / 2,
							 Global::player->getInventory()->getSlotSize(),
							 Global::player->getInventory()->getSlotSize(),
							 false);
		for (unsigned int i = 0; i < selectedItem->getDescription().size(); i++) {
			//Renders description line by line
			SDL_Texture* descText = Global::resourceHandler->getTextTexture(Text(selectedItem->getDescription()[i], Global::resourceHandler->colors["iteminfo-desc"]));
			int tw, th;
			SDL_QueryTexture(descText, NULL, NULL, &tw, &th);
			tw = tw / Global::defaultFontSize * descriptionSize;
			th = th / Global::defaultFontSize * descriptionSize;
			destinationRect.x = x + w / 2 - tw / 2;
			destinationRect.y = y + h / 2 - th / 2 + i * th;
			destinationRect.w = tw;
			destinationRect.h = th;
			SDL_RenderCopy(Global::renderer, descText, NULL, &destinationRect);
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
