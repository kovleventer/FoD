#pragma once

#include <string>
#include <map>
#include <fstream>
#include <iostream>

#include "item.h"

class ItemHandler {
public:
	ItemHandler();
	~ItemHandler();
	
	void loadAll();
	
	std::map<std::string, Item*> items;
	
	//The translater is used to convert an enum class into a string and vice versa
	//https://stackoverflow.com/questions/7163069/c-string-to-enum
	ItemRarity translateR(std::string itemRarityString);
	std::string translateR(ItemRarity itemRarity);
	
	ItemType translateT(std::string itemTypeString);
	std::string translateT(ItemType itemType);
private:
	std::string basePath;
	
	//Logic behind the translaters
	std::map<std::string, ItemRarity> translaterSIR;
	std::map<ItemRarity, std::string> translaterIRS;
	
	std::map<std::string, ItemType> translaterSIT;
	std::map<ItemType, std::string> translaterITS;
	
	void translaterInit();
};
