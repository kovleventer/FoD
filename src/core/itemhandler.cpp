#include "itemhandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

ItemHandler::ItemHandler() {
	basePath = "data/item/";
	
	translaterInit();
}

ItemHandler::~ItemHandler() {
	//Frees all textures
	stdex::clear_ptr_map(items);
}

void ItemHandler::loadAll() {
	std::vector<std::string> itemList = FilesystemHandler::getFilesInDir(basePath);
	
	//NOTE uses file IO
	std::fstream file;
	
	//File pattern:
	//Name
	//RARITY
	//TYPE
	//'stat' 'value'
	for (unsigned int i = 0; i < itemList.size(); i++) {
		
		file.open(basePath + itemList[i], std::ios::in);
		
		std::string itemName;
		std::string itemRarityString;
		std::string itemTypeString;
		
		//Reads the item's name, rarity and type
		std::getline(file, itemName);
		std::getline(file, itemRarityString);
		std::getline(file, itemTypeString);
		
		ItemRarity itemRarity = translateR(itemRarityString);
		ItemType itemType = translateT(itemTypeString);
		
		int itemPrice;
		file >> itemPrice;
		
		Item* currentItem = new Item(itemName, itemRarity, itemType, itemPrice);
		
		std::string key;
		int value;
		while (file >> key && file >> value) {
			currentItem->stats[key] = value;
		}
		
		currentItem->generateDescription();
		currentItem->texture = Global::resourceHandler->getATexture(TT::ITEM, FilesystemHandler::removeExtension(itemList[i]));
		
		items[FilesystemHandler::removeExtension(itemList[i])] = currentItem;
		
		file.close();
	}
}

Item* ItemHandler::getItem(std::string itemName) {
	if (items.find(itemName) == items.end()) {
		if (itemName != "") {
			std::clog << "Warning! " << itemName << " not found" << std::endl;
		}
		return NULL;
	} else {
		return items[itemName];
	}
}

ItemRarity ItemHandler::translateR(std::string itemRarityString) {
	return translaterSIR[itemRarityString];
}

std::string ItemHandler::translateR(ItemRarity itemRarity) {
	return translaterIRS[itemRarity];
}

ItemType ItemHandler::translateT(std::string itemTypeString) {
	return translaterSIT[itemTypeString];
}

std::string ItemHandler::translateT(ItemType itemType) {
	return translaterITS[itemType];
}

void ItemHandler::translaterInit() {
	//NOTE not compatible with each other
	translaterSIR["COMMON"] = ItemRarity::COMMON;
	translaterSIR["RARE"] = ItemRarity::RARE;
	translaterSIR["LEGENDARY"] = ItemRarity::LEGENDARY;
	translaterSIR["UTIL"] = ItemRarity::UTIL;
	
	translaterIRS[ItemRarity::COMMON] = "Common";
	translaterIRS[ItemRarity::RARE] = "Rare";
	translaterIRS[ItemRarity::LEGENDARY] = "Legendary";
	translaterIRS[ItemRarity::UTIL] = "Util";
	
	
	translaterSIT["MELEE"] = ItemType::MELEE;
	translaterSIT["BOW"] = ItemType::BOW;
	translaterSIT["STAFF"] = ItemType::STAFF;
	translaterSIT["SHIELD"] = ItemType::SHIELD;
	translaterSIT["ARMOR"] = ItemType::ARMOR;
	translaterSIT["CLOAK"] = ItemType::CLOAK;
	translaterSIT["JEWELLERY"] = ItemType::JEWELLERY;
	translaterSIT["ARROW"] = ItemType::ARROW;
	translaterSIT["MAGICSHIELD"] = ItemType::MAGICSHIELD;
	translaterSIT["UTIL"] = ItemType::UTIL;
	
	translaterITS[ItemType::MELEE] = "Melee";
	translaterITS[ItemType::BOW] = "Bow";
	translaterITS[ItemType::STAFF] = "Staff";
	translaterITS[ItemType::SHIELD] = "Shield";
	translaterITS[ItemType::ARMOR] = "Armor";
	translaterITS[ItemType::CLOAK] = "Cloak";
	translaterITS[ItemType::JEWELLERY] = "Jewellery";
	translaterITS[ItemType::ARROW] = "Arrow";
	translaterITS[ItemType::MAGICSHIELD] = "Magicshield";
	translaterITS[ItemType::UTIL] = "Util";
}
