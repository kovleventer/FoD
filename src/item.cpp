#include "item.h"

#include "global.h"

Item::Item(std::string n, ItemRarity r, ItemType t) {
	name = n;
	itemRarity = r;
	itemType = t;
}

void Item::generateDescription() {
	description.push_back(name);
	
	if (itemRarity == ItemRarity::UTIL) {
		return;
	}
	
	//Add the rarity, type and every stat of the item to the description
	description.push_back("Rarity: " + Global::itemHandler->translateR(itemRarity));
	description.push_back("Type: " + Global::itemHandler->translateT(itemType));
	for(std::map<std::string, int>::const_iterator it = stats.begin(); it != stats.end(); ++it) {
		if (it->second != 0) {
			std::stringstream oss;
			oss << it->first << ": " << it->second;
			description.push_back(oss.str());
		}
	}
}

std::string Item::getName() {
	return name;
}

std::vector<std::string> Item::getDescription() {
	return description;
}

ItemRarity Item::getItemRarity() {
	return itemRarity;
}

ItemType Item::getItemType() {
	return itemType;
}

void Item::render(int x, int y, int w, int h, bool withIndicator) {
	SDL_Rect destinationRect = {x, y, w, h};
	render(destinationRect, withIndicator);
}

void Item::render(SDL_Rect destinationRect, bool withIndicator) {
	SDL_RenderCopy(Global::renderer, texture, NULL, &destinationRect);
	if (withIndicator) {
		//Rendering the colorful triangle
		std::string rarityString = Global::itemHandler->translateR(itemRarity);
		SDL_Texture* indicator = Global::resourceHandler->itemRarityIndicatorTextures[rarityString];
		int w, h;
		SDL_QueryTexture(indicator, NULL, NULL, &w, &h);
		destinationRect.w = w;
		destinationRect.h = h;
		SDL_RenderCopy(Global::renderer, indicator, NULL, &destinationRect);
	}
}
