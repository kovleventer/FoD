#include "worldobjecthandler.h"

#include "global.h"

/*!
 * @author kovlev
 */

WorldObjectHandler::WorldObjectHandler() {
	worldObjectBasePath = "data/map/wo/";
	impassableBasePath = "data/map/imp/";
	interactiveBasePath = "data/map/int/";
}

WorldObjectHandler::~WorldObjectHandler() {
	//Deleting WorldObjects
	for (unsigned int i = 0; i < worldObjects.size(); i++) {
		delete worldObjects[i];
	}
	
	//Deleting ImpassableWorldObjects
	for (unsigned int i = 0; i < impassables.size(); i++) {
		delete impassables[i];
	}
	
	//Deleting InteractiveWorldObjects
	for (unsigned int i = 0; i < interactives.size(); i++) {
		delete interactives[i];
	}
}

void WorldObjectHandler::loadAll() {
	loadWorldObjects();
	loadImpassableWorldObjects();
	loadInteractiveWorldObjects();
}

void WorldObjectHandler::setOwnershipRelations() {
	for (unsigned int i = 0; i < interactives.size(); i++) {
		interactives[i]->setOwner(Global::npcHandler->getCharacterByName(interactives[i]->tempOwnerHolder));
		Global::npcHandler->getCharacterByName(interactives[i]->tempOwnerHolder)->addOwned(interactives[i]);
	}
}

void WorldObjectHandler::loadWorldObjects() {
	std::vector<std::string> worldObjectNames = FilesystemHandler::getFilesInDir(worldObjectBasePath);
	
	//NOTE uses file IO
	std::fstream file;
	for (unsigned int i = 0; i < worldObjectNames.size(); i++) {
		file.open(worldObjectBasePath + worldObjectNames[i], std::ios::in);
		//File pattern
		//Texture name
		//Position point
		//Scale
		std::string textureName;
		file >> textureName;
		
		int posX, posY;
		file >> posX;
		file >> posY;
		
		double scale;
		file >> scale;
		
		WorldObject* loaded = new WorldObject(textureName, posX, posY);
		loaded->setScale(scale);
		//Error checking for position
		if (Global::map->getTile(posX, posY) == NULL) throw std::runtime_error("Invalid world object position (" + worldObjectNames[i] + ", (" + std::to_string(posX) + ";" + std::to_string(posY) + "))");
		Global::map->getTile(posX, posY)->backgroundEntities.push_back(loaded);
		worldObjects.push_back(loaded);
		
		file.close();
	}
}

void WorldObjectHandler::loadImpassableWorldObjects() {
	std::vector<std::string> impassableNames = FilesystemHandler::getFilesInDir(impassableBasePath);
	
	//NOTE uses file IO
	std::fstream file;
	for (unsigned int i = 0; i < impassableNames.size(); i++) {
		file.open(impassableBasePath + impassableNames[i], std::ios::in);
		//File pattern:
		//Texture name
		//Position point
		// a [b c (a times)]
		// a: number of impassable tiles
		// b: the x coordinate of current impassable tile (relative, so (0,0) means the position tile itself)
		// c: the x coordinate of current impassable tile (relative, so (0,0) means the position tile itself)
		//Scale
		std::string textureName;
		file >> textureName;
		
		int posX, posY;
		file >> posX;
		file >> posY;
		
		int impLen;
		std::vector<Point> impassableTiles;
		file >> impLen;
		for (int i = 0; i < impLen; i++) {
			int x, y;
			file >> x;
			file >> y;
			impassableTiles.push_back(Point(x, y));
		}
		
		double scale;
		file >> scale;
		
		ImpassableWorldObject* loaded = new ImpassableWorldObject(textureName, posX, posY, impassableTiles);
		loaded->setScale(scale);
		//Error checking for position
		if (Global::map->getTile(posX, posY) == NULL) throw std::runtime_error("Invalid world object position (" + impassableNames[i] + ", (" + std::to_string(posX) + ";" + std::to_string(posY) + "))");
		Global::map->getTile(posX, posY)->entities.push_back(loaded);
		impassables.push_back(loaded);
		
		file.close();
	}
}

void WorldObjectHandler::loadInteractiveWorldObjects() {
	std::vector<std::string> interactiveNames = FilesystemHandler::getFilesInDir(interactiveBasePath);
	
	//NOTE uses file IO
	std::fstream file;
	for (unsigned int i = 0; i < interactiveNames.size(); i++) {
		file.open(interactiveBasePath + interactiveNames[i], std::ios::in);
		//File pattern:
		//Name
		//Texture name
		//Owner name
		//Has ItemBuyingMenu?, Has UnitBuyingMenu?, Has Garrison? bools
		//IF HAS ItemBuyingMenu
		//	[itemcount] ["itemname"] itemcount times
		//IF HAS UnitBuyingMenu
		//	[unitcount] ["unitname"] unitcount times
		//Position point
		// a [b c (a times)]
		// a: number of interactive tiles
		// b: the x coordinate of current interactive tile (relative, so (0,0) means the position tile itself)
		// c: the x coordinate of current interactive tile (relative, so (0,0) means the position tile itself)
		//Scale
		std::string name;
		std::getline(file, name);
		
		std::string textureName;
		std::getline(file, textureName);
		
		std::string ownerName;
		std::getline(file, ownerName);
		
		bool hasIBM, hasUBM, hasGar;
		file >> hasIBM;
		file >> hasUBM;
		file >> hasGar;
		
		std::vector<Item*> itemsToSell;
		if (hasIBM) {
			int invLen;
			file >> invLen;
			for (int i = 0; i < invLen; i++) {
				std::string itemName;
				file >> std::quoted(itemName);
				itemsToSell.push_back(Global::itemHandler->getItem(itemName));
			}
		}
		
		std::vector<Unit*> unitsToSell;
		if (hasUBM) {
			int armLen;
			file >> armLen;
			for (int i = 0; i < armLen; i++) {
				std::string unitName;
				file >> std::quoted(unitName);
				unitsToSell.push_back(Global::unitHandler->getUnit(unitName, 1));
			}
		}
		
		std::vector<std::string> unitNames;
		std::vector<int> unitLevels;
		std::vector<int> unitDamages;
		std::vector<int> unitExperiences;
		//Storing 4 * 10 items
		std::vector<std::string> unitInventory;
		
		if (hasGar) {
			for (int i = 0; i < 10; i++) {
				std::string unitName;
				file >> std::quoted(unitName);
				unitNames.push_back(unitName);
			}
			
			for (int i = 0; i < 10; i++) {
				int unitLevel;
				file >> unitLevel;
				unitLevels.push_back(unitLevel);
			}
			
			for (int i = 0; i < 10; i++) {
				int unitDamage;
				file >> unitDamage;
				unitDamages.push_back(unitDamage);
			}
			
			for (int i = 0; i < 10; i++) {
				int unitExperience;
				file >> unitExperience;
				unitExperiences.push_back(unitExperience);
			}
			
			//Loading unit items
			for (int i = 0; i < 40; i++) {
				std::string itemName;
				file >> std::quoted(itemName);
				unitInventory.push_back(itemName);
			}
		}
		
		int posX, posY;
		file >> posX;
		file >> posY;
		
		int impLen;
		std::vector<Point> interactiveTiles;
		file >> impLen;
		for (int i = 0; i < impLen; i++) {
			int x, y;
			file >> x;
			file >> y;
			interactiveTiles.push_back(Point(x, y));
		}
		
		double scale;
		file >> scale;
		
		file.close();
		
		if (!file) {
			//If the file pattern is wrong
			std::clog << "Error! Skipping " << interactiveNames[i] << std::endl;
			continue;
		}
		
		InteractiveWorldObject* loaded = new InteractiveWorldObject(textureName, posX, posY, interactiveTiles, false);
		loaded->setName(name);
		
		loaded->tempOwnerHolder = ownerName;
		
		//Setting interactive properties
		if (hasIBM) {
			ItemMarket* tempItemMarket = new ItemMarket(loaded->getGUI());
			tempItemMarket->getItemBuyingMenu()->setItemList(itemsToSell);
			loaded->getGUI()->addPart({"Market", tempItemMarket});
			
		}
		if (hasUBM) {
			Barracks* tempBarracks = new Barracks(loaded->getGUI());
			tempBarracks->getUnitBuyingMenu()->setUnitList(unitsToSell);
			loaded->getGUI()->addPart({"Barracks", tempBarracks});
		}
		if (hasGar) {
			GarrisonWrapper* tempGarrisonWrapper = new GarrisonWrapper(loaded->getGUI());
			tempGarrisonWrapper->setIsRenderableWhenNotOwnedByPlayer(false);
			for (int i = 0; i < 10; i++) {
				if (unitNames[i] == "") continue;
				
				Unit* currentUnit = Global::unitHandler->getUnit(unitNames[i], unitLevels[i]);
				
				//Adding items to units
				for (int j = 0; j < 4; j++) {
					currentUnit->addItem(Global::itemHandler->getItem(unitInventory[i * 4 + j]));
				}
				
				currentUnit->recalculateInventory();
				
				currentUnit->statsWithItems["currentLife"] -= unitDamages[i];
				currentUnit->statsWithItems["currentExperience"] += unitExperiences[i];
				
				tempGarrisonWrapper->getGarrison()->getArmy()->setUnit(i, currentUnit);
			}
			loaded->getGUI()->addPart({"Garrison", tempGarrisonWrapper});
		}
		
		loaded->setScale(scale);
		//Error checking for position
		if (Global::map->getTile(posX, posY) == NULL) throw std::runtime_error("Invalid world object position (" + interactiveNames[i] + ", (" + std::to_string(posX) + ";" + std::to_string(posY) + "))");
		Global::map->getTile(posX, posY)->entities.push_back(loaded);
		interactives.push_back(loaded);
	}
}
