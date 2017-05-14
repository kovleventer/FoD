#pragma once

#include <SDL.h>
#include <string>
#include <map>
#include <sstream>
#include <vector>

#include "../core/animatabletexture.h"

/*!
 * @author kovlev
 */


/*!
 * @enum ItemRarity how rare is the item
 * The small colorful triangle displayed in th top left corner of the item indicates this
 * NOTE update itemhandler/translater when change these
 */
enum class ItemRarity {
	COMMON,
	RARE,
	LEGENDARY,
	UTIL
};


/*!
 * @enum ItemType Used by wielding filter
 * Not every unit can wield every item
 * A unit can only wield one item per type
 */
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


/*!
 * @class Item represents an item
 * It has stats and stuff
 * Items are not changeable therefore not instantiatable
 * You should not delete items except on cleanup
 */
class Item {
public:
	Item(std::string n, ItemRarity r, ItemType t, int p);
	
	void generateDescription();
	
	//Getters
	std::string getName();
	std::vector<std::string> getDescription();
	ItemRarity getItemRarity();
	ItemType getItemType();
	int getPrice();
	
	ATexture* texture;
	
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
	
	int price;
};
