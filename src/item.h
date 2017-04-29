#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <map>
#include <sstream>
#include <vector>

//NOTE update itemhandler/translater when change these
enum class ItemRarity {
	COMMON,
	RARE,
	LEGENDARY,
	UTIL
};

//Used by wielding filter
//Not every unit can wield every item
//A unit can only wield one item per type
enum class ItemType {
	MELEE,
	BOW,
	STAFF,
	SHIELD,
	ARMOR,
	CLOAK,
	JEWELLERY,
	ARROW,
	MAGICSHIELD,
	UTIL
};

class Item {
public:
	Item(std::string n, ItemRarity r, ItemType t);
	
	void generateDescription();
	
	//Getters
	std::string getName();
	std::vector<std::string> getDescription();
	ItemRarity getItemRarity();
	ItemType getItemType();
	
	SDL_Texture* texture;
	
	//withIndicator sets if the colorful triangle should be rendered or not
	void render(int x, int y, int w, int h, bool withIndicator);
	void render(SDL_Rect destinationRect, bool withIndicator);
	
	//Stores the item's statistics
	std::map<std::string, int> stats;
	
private:
	void init();
	
	std::string name;
	std::vector<std::string> description;
	
	ItemRarity itemRarity;
	ItemType itemType;
};
