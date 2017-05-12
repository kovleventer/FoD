#pragma once

#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include "../player/item.h"
#include "../util/filesystemhandler.h"

/*!
 * @author kovlev
 */


/*!
 * @class ItemHandler loads all items which can be reachable later using the getItem method
 */
class ItemHandler {
public:
	ItemHandler();
	~ItemHandler();
	
	void loadAll();
	
	//Gets an item, returns NULL if invalid
	Item* getItem(std::string itemName);
	
	//The translater is used to convert an enum class into a string and vice versa
	//https://stackoverflow.com/questions/7163069/c-string-to-enum
	ItemRarity translateR(std::string itemRarityString);
	std::string translateR(ItemRarity itemRarity);
	
	ItemType translateT(std::string itemTypeString);
	std::string translateT(ItemType itemType);
private:
	std::string basePath;
	
	std::map<std::string, Item*> items;
	
	//Logic behind the translaters
	std::map<std::string, ItemRarity> translaterSIR;
	std::map<ItemRarity, std::string> translaterIRS;
	
	std::map<std::string, ItemType> translaterSIT;
	std::map<ItemType, std::string> translaterITS;
	
	void translaterInit();
};
